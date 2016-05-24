/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ostream>

namespace ejson {
	/**
	 * @brief Type of the JSON elements.
	 */
	enum class valueType {
		unknow, //!< might be an error ...
		value, //!< might be an error ...
		document, //!< all the file main access
		array, //!< te element [ ... ]
		object, //!< the element { ... }
		string,  //!< the element "..."
		number, //!< The element 1111.2222
		null, //!< the element null
		boolean, //!< the element true or false
	};
	//! @not_in_doc
	std::ostream& operator <<(std::ostream& _os, enum valueType _obj);
}
