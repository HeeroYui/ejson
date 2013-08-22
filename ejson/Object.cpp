/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ejson/Object.h>
#include <ejson/Array.h>
#include <ejson/String.h>
#include <ejson/Null.h>
#include <ejson/Number.h>
#include <ejson/Boolean.h>
#include <ejson/debug.h>
#include <ejson/ejson.h>

#undef __class__
#define __class__	"Object"

void ejson::Object::Clear(void)
{
	for (esize_t iii=0; iii<m_value.Size(); ++iii) {
		if (NULL == m_value[iii]) {
			continue;
		}
		delete(m_value[iii]);
		m_value[iii] = NULL;
	}
	m_value.Clear();
}


typedef enum {
	parseName,
	parseMiddle,
	parseValue,
} statusParsing_te;

bool ejson::Object::IParse(const etk::UString& _data, int32_t& _pos, ejson::filePos& _filePos, ejson::Document& _doc)
{
	statusParsing_te mode = parseName;
	etk::UString currentName;
	JSON_PARSE_ELEMENT("start parse : 'Object' ");
	bool standalone = true;
	int32_t startPos = _pos+1;
	if (_data[_pos] != '{' ) { // when the main node call it, it can be start with != '{'
		standalone = false;
		startPos = _pos;
	}
	for (int32_t iii=startPos; iii<_data.Size(); iii++) {
		_filePos.Check(_data[iii]);
		#ifdef ENABLE_DISPLAY_PARSED_ELEMENT
			DrawElementParsed(_data[iii], _filePos);
		#endif
		ejson::filePos tmpPos;
		if(    _data[iii]==' '
		    || _data[iii]=='\t'
		    || _data[iii]=='\n'
		    || _data[iii]=='\r') {
			// white space ==> nothing to do ...
		} else if(_data[iii]=='}') {
			// find end of value:
			_pos=iii; // ==> return the end element type ==> usefull to check end and check if adding element is needed
			return true;
		} else {
			if (mode == parseName) {
				JSON_PARSE_ELEMENT("name START " << '"');
				if (_data[iii]=='"') {
					currentName = "";
					for (iii++; iii<_data.Size(); iii++) {
						_filePos.Check(_data[iii]);
						#ifdef ENABLE_DISPLAY_PARSED_ELEMENT
							DrawElementParsed(_data[iii], _filePos);
						#endif
						if (_data[iii]=='\"') {
							mode = parseMiddle;
							break;
						} else {
							currentName += _data[iii];
						}
					}
				} else if (CheckString(_data[iii]) ) {
					currentName += _data[iii];
					for (iii++; iii<_data.Size(); iii++) {
						_filePos.Check(_data[iii]);
						#ifdef ENABLE_DISPLAY_PARSED_ELEMENT
							DrawElementParsed(_data[iii], _filePos);
						#endif
						if (false==CheckString(_data[iii])) {
							mode = parseMiddle;
							iii--;
							break;
						} else {
							currentName += _data[iii];
						}
					}
				} else {
					EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "element unknow ...");
					_pos = iii;
					return false;
				}
				JSON_PARSE_ELEMENT("name END ");
			} else if (mode == parseMiddle) {
				JSON_PARSE_ELEMENT(" middle ... ");
				if (_data[iii]==':') {
					mode = parseValue;
				} else {
					EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "separator is not ':'");
					return false;
				}
			} else if (mode == parseValue) {
				if (_data[iii]=='{') {
					// find an object:
					JSON_PARSE_ELEMENT("find Object");
					ejson::Object * tmpElement = new ejson::Object();
					if (NULL==tmpElement) {
						EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "Allocation error in object");
						_pos=iii;
						return false;
					}
					tmpElement->IParse(_data, iii, _filePos, _doc);
					AddSub(currentName, tmpElement);
					currentName = "";
				} else if (_data[iii]=='"') {
					// find a string:
					JSON_PARSE_ELEMENT("find String quoted");
					ejson::String * tmpElement = new ejson::String();
					if (NULL==tmpElement) {
						EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "Allocation error in String");
						_pos=iii;
						return false;
					}
					tmpElement->IParse(_data, iii, _filePos, _doc);
					AddSub(currentName, tmpElement);
					currentName = "";
				} else if (_data[iii]=='[') {
					// find a list:
					JSON_PARSE_ELEMENT("find List");
					ejson::Array * tmpElement = new ejson::Array();
					if (NULL==tmpElement) {
						EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "Allocation error in Array");
						_pos=iii;
						return false;
					}
					tmpElement->IParse(_data, iii, _filePos, _doc);
					AddSub(currentName, tmpElement);
					currentName = "";
				} else if(    _data[iii] == 'f'
				           || _data[iii] == 't' ) {
					// find boolean:
					JSON_PARSE_ELEMENT("find Boolean");
					ejson::Boolean * tmpElement = new ejson::Boolean();
					if (NULL==tmpElement) {
						EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "Allocation error in Boolean");
						_pos=iii;
						return false;
					}
					tmpElement->IParse(_data, iii, _filePos, _doc);
					AddSub(currentName, tmpElement);
					currentName = "";
				} else if( _data[iii] == 'n') {
					// find null:
					JSON_PARSE_ELEMENT("find Null");
					ejson::Null * tmpElement = new ejson::Null();
					if (NULL==tmpElement) {
						EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "Allocation error in Boolean");
						_pos=iii;
						return false;
					}
					tmpElement->IParse(_data, iii, _filePos, _doc);
					AddSub(currentName, tmpElement);
					currentName = "";
				} else if(true==CheckNumber(_data[iii])) {
					// find number:
					JSON_PARSE_ELEMENT("find Number");
					ejson::Number * tmpElement = new ejson::Number();
					if (NULL==tmpElement) {
						EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, "Allocation error in Boolean");
						_pos=iii;
						return false;
					}
					tmpElement->IParse(_data, iii, _filePos, _doc);
					AddSub(currentName, tmpElement);
					currentName = "";
				} else if(_data[iii]==',') {
					// find Separator : Restart cycle ...
					mode = parseName;
					currentName = "";
				} else {
					// find an error ....
					EJSON_CREATE_ERROR(_doc, _data, iii, _filePos, etk::UString("Find '") + _data[iii] + "' with no element in the element...");
					// move the curent index
					_pos = iii+1;
					return false;
				}
			}
		}
	}
	_pos = _data.Size();
	if (false==standalone) {
		return true;
	}
	return false;
}
bool ejson::Object::IGenerate(etk::UString& _data, int32_t _indent) const
{
	_data += "{\n";
	for (esize_t iii=0; iii<m_value.Size() ; iii++) {
		AddIndent(_data, _indent);
		_data += "\"";
		_data += m_value.GetKey(iii);
		_data += "\": ";
		m_value.GetValue(iii)->IGenerate(_data, _indent+1);
		if (iii<m_value.Size()-1) {
			_data += ",";
		}
		_data += "\n";
	}
	AddIndent(_data, _indent-1);
	_data += "}";
	return true;
}


ejson::Value* ejson::Object::GetSub(const etk::UString& _named) const
{
	if (false==m_value.Exist(_named)) {
		return NULL;
	}
	return m_value[_named];
}

ejson::Object* ejson::Object::GetSubObject(const etk::UString& _named) const
{
	ejson::Value* tmp = GetSub(_named);
	if (NULL == tmp) {
		return NULL;
	}
	return tmp->ToObject();
}

ejson::String* ejson::Object::GetSubString(const etk::UString& _named) const
{
	ejson::Value* tmp = GetSub(_named);
	if (NULL == tmp) {
		return NULL;
	}
	return tmp->ToString();
}

ejson::Array* ejson::Object::GetSubArray(const etk::UString& _named) const
{
	ejson::Value* tmp = GetSub(_named);
	if (NULL == tmp) {
		return NULL;
	}
	return tmp->ToArray();
}


bool ejson::Object::AddSub(const etk::UString& _name, ejson::Value* _value)
{
	if (NULL == _value) {
		return false;
	}
	if (_name.Size()==0) {
		return false;
	}
	if (m_value.Exist(_name)) {
		ejson::Value* tmp = m_value[_name];
		delete(tmp);
		m_value[_name] = _value;
		return true;
	}
	m_value.Add(_name, _value);
	return true;
}



bool ejson::Object::TransfertIn(ejson::Value* _obj)
{
	if (NULL==_obj) {
		JSON_ERROR("Request transfer on an NULL pointer");
		return false;
	}
	ejson::Object* other = _obj->ToObject();
	if (NULL==other) {
		JSON_ERROR("Request transfer on an element that is not an object");
		return false;
	}
	// remove destination elements
	other->Clear();
	// Copy to the destination
	other->m_value = m_value;
	// remove current:
	m_value.Clear();
	return true;
}

// TODO : Manage error ...
ejson::Value* ejson::Object::Duplicate(void) const
{
	ejson::Object* output = new ejson::Object();
	if (NULL==output) {
		JSON_ERROR("Allocation error ...");
		return NULL;
	}
	for (esize_t iii=0; iii<m_value.Size(); ++iii) {
		ejson::Value* val = m_value.GetValue(iii);
		etk::UString key = m_value.GetKey(iii);
		if (NULL == val) {
			continue;
		}
		output->AddSub(key, val->Duplicate());
	}
	return output;
}
