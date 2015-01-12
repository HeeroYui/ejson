/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */


#include <ejson/Null.h>
#include <ejson/debug.h>
#include <ejson/ejson.h>

#undef __class__
#define __class__	"Null"

std::shared_ptr<ejson::Null> ejson::Null::create() {
	return std::shared_ptr<ejson::Null>(new ejson::Null());
}


bool ejson::Null::iParse(const std::string& _data, size_t& _pos, ejson::filePos& _filePos, ejson::Document& _doc) {
	JSON_PARSE_ELEMENT("start parse : 'Null' ");
	if (_pos+3 >= _data.size()){
		EJSON_CREATE_ERROR(_doc, _data, _pos, _filePos, "can not parse null !!! ");
		return false;
	}
	if(    _data[_pos] != 'n'
	    || _data[_pos+1] != 'u'
	    || _data[_pos+2] != 'l'
	    || _data[_pos+3] != 'l' ) {
		EJSON_CREATE_ERROR(_doc, _data, _pos, _filePos, "Not a corect 'null' element");
		return false;
	}
	_pos+=4;
	_filePos+=4;
	return true;
}


bool ejson::Null::iGenerate(std::string& _data, size_t _indent) const {
	_data += "null";
	return true;
}


bool ejson::Null::transfertIn(std::shared_ptr<ejson::Value> _obj) {
	if (_obj == nullptr) {
		JSON_ERROR("Request transfer on an nullptr pointer");
		return false;
	}
	std::shared_ptr<ejson::Null> other = _obj->toNull();
	if (other == nullptr) {
		JSON_ERROR("Request transfer on an element that is not an Null");
		return false;
	}
	return true;
}

std::shared_ptr<ejson::Value> ejson::Null::duplicate() const {
	std::shared_ptr<ejson::Null> output = ejson::Null::create();
	if (output == nullptr) {
		JSON_ERROR("Allocation error ...");
		return nullptr;
	}
	return output;
}


