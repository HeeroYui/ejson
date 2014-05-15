/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __ETK_JSON_NULL_H__
#define __ETK_JSON_NULL_H__

#include <etk/types.h>
#include <ejson/Value.h>

namespace ejson {
	class Null : public ejson::Value {
		public:
			/**
			 * @brief basic element of a xml structure
			 */
			Null() { };
			/**
			 * @brief destructor
			 */
			virtual ~Null() { };
		public: // herited function :
			virtual bool iParse(const std::string& _data, size_t& _pos, ejson::filePos& _filePos, ejson::Document& _doc);
			virtual bool iGenerate(std::string& _data, size_t _indent) const;
			virtual bool transfertIn(ejson::Value* _obj);
			virtual ejson::Value* duplicate() const;
	};
};

#endif

