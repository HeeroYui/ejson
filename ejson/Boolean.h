/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __ETK_JSON_BOOLEAN_H__
#define __ETK_JSON_BOOLEAN_H__

#include <etk/types.h>
#include <ejson/Value.h>

namespace ejson {
	class Boolean : public ejson::Value {
		public:
			/**
			 * @brief basic element of a xml structure
			 */
			Boolean(bool _value=false) :
			  m_value(_value) {
				
			};
			/**
			 * @brief destructor
			 */
			virtual ~Boolean() {
				
			};
		protected:
			bool m_value; //!< value of the node
		public:
			/**
			 * @brief set the value of the node.
			 * @param[in] _value New value of the node.
			 */
			void set(bool _value) {
				m_value = _value;
			};
			/**
			 * @brief get the current element Value.
			 * @return the reference of the string value.
			 */
			bool get() const {
				return m_value;
			};
		public: // herited function :
			virtual bool iParse(const std::string& _data, size_t& _pos, ejson::filePos& _filePos, ejson::Document& _doc);
			virtual bool iGenerate(std::string& _data, size_t _indent) const;
			virtual bool transfertIn(ejson::Value* _obj);
			virtual ejson::Value* duplicate() const;
	};
};

#endif

