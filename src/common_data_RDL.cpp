/*!
	@file common_data_RDL.cpp
	@brief file to hold data that is common to all displays
*/

#include "../include/common_data_RDL.hpp"

namespace rdlib {
/*!
	@brief  get the library version number
	@return the library version number eg 120 = 1.2.0
*/
uint16_t LibraryVersion(void)
{
	return 241;
}

/*! 
	@brief Prints out Enum label and help message for given enum code from Return_Codes_e
	@param code an enum code from Return_Codes_e
	@return A String containing the error enum and message
*/
std::string ReturnCodetoText(rdlib::Return_Codes_e code) 
{
	if (static_cast<size_t>(code) < rdlib::ReturnCodesStrings.size()) 
	{
		return rdlib::ReturnCodesStrings[static_cast<size_t>(code)];
	}
	fprintf(stderr, "Warning  ReturnCodetoText :Unknown error code passed\r\n" );
	return "UNKNOWN";
}
}
