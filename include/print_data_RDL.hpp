/*!
	@file     print_data_RDL.hpp
	@brief    Base class that provides print() and println() for for the display library
			Project Name :: Display_Lib_RPI
	@details Polymorphic print class to print many data types by wrapping write function in the graphics class's
	Using function overloading and templates for the vector definitions. Supports integers, floats,
	characters, character arrays, string objects and vectors.
*/

#pragma once

#include <cinttypes>
#include <cstdio> // for size_t
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <bitset>  // required for Binary conversion
#include <iomanip> // required for std::setprecision 
#include <sstream> // Required for std::ostringstream

/*!
	@brief Polymorphic print class to print many data types by wrapping write function in the graphics class's
	@details Using function overloading and templates for the vector definitions. Supports integers floats
	Characters , character arrays, string objects and vectors.
*/
class Print
{
public:
	Print() : _ErrorFlag(0) {}

	/*! Base number type enum*/
	enum BaseNum : uint8_t{
		RDL_DEC = 10, /**< Format the passed integer in Decimal format */
		RDL_HEX = 16, /**< Format the passed integer in Hexadecimal format  */
		RDL_OCT = 8,  /**< Format the passed integer in Octal format  */
		RDL_BIN = 2   /**< Format the passed integer in Binary format  */
	};

	int getWriteError();
	void clearWriteError();
	/*! @brief define in the sub class */
	virtual size_t write(uint8_t) = 0;
	/*! @brief define in the sub class */
	virtual size_t write(const uint8_t *buffer, size_t size);
	size_t write(const char *str);
	size_t write(const char *buffer, size_t size);
	/*! @brief define in the sub class */
	virtual int availableForWrite();

	size_t print(const char[]);
	size_t print(char);
	size_t print(int, int = RDL_DEC);
	size_t print(unsigned int, int = RDL_DEC);
	size_t print(long, int = RDL_DEC);
	size_t print(unsigned long, int = RDL_DEC);
	size_t print(double, int = 2);
	size_t print(const std::string &);

	size_t println(const char[]);
	size_t println(char);
	size_t println(int, int = RDL_DEC);
	size_t println(unsigned int, int = RDL_DEC);
	size_t println(long, int = RDL_DEC);
	size_t println(unsigned long, int = RDL_DEC);
	size_t println(double, int = 2);
	size_t println(void);
	size_t println(const std::string &s);

	/*!
		@brief Print a vector of any type
		@tparam T Type of elements in the vector
		@param v Vector of elements to print
		@param format Format specifier:
			- For floats/doubles: Number of decimal places (default is 2).
			- For ints/longs: Conversion base (default is RDL_DEC for decimal).
		@return Number of characters written
	*/
	template <typename T>
	size_t print(const std::vector<T> &v,  int format = defaultFormat<T>()) {
		size_t totalWritten = 0;
		for (const auto &element : v) {
			std::string elementStr = formatElement(element, format);
			totalWritten += write(elementStr.c_str(), elementStr.length());

			// Add a separator space if not the last element
			if (&element != &v.back()) {
				totalWritten += write(" ", 1);
			}
		}
		return totalWritten;
	}

	/*!
		@brief Print a vector of elements followed by a newline.
		@tparam T Type of the elements in the vector
		@param vec The vector of elements to print
		@param format Format specifier (e.g., base for integers or precision for floats)
		@return Number of characters printed
	*/
	template <typename T>
	size_t println(const std::vector<T> &vec, int format = defaultFormat<T>()) 
	{
		size_t chars = print(vec, format); // Use default format for the type
		chars += print("\r\n");           // Add a newline
		return chars;
	}

	/*!
		@brief Print a std::array of any type.
		@tparam T Type of elements in the array.
		@tparam N Size of the array.
		@param arr The std::array to print.
		@param format Format specifier:
			- For floats/doubles: Number of decimal places (default is 2).
			- For ints/longs: Conversion base (default is RDL_DEC for decimal).
		@return Number of characters written.
	*/
	template <typename T, size_t N>
	size_t print(const std::array<T, N> &arr, int format = defaultFormat<T>()) 
	{
		return print(std::vector<T>(arr.begin(), arr.end()), format);
	}
	
		/*!
		@brief Print a std::array of elements followed by a newline.
		@tparam T Type of elements in the array.
		@tparam N Size of the array.
		@param arr The std::array to print.
		@param format Format specifier:
			- For floats/doubles: Number of decimal places (default is 2).
			- For ints/longs: Conversion base (default is RDL_DEC for decimal).
		@return Number of characters written.
	*/
	template <typename T, size_t N>
	size_t println(const std::array<T, N> &arr, int format = defaultFormat<T>()) {
		// Call print() to print the array elements
		size_t chars = print(arr, format);
		// Print a newline sequence
		chars += print("\r\n");
		return chars;
	}

	protected:

	void setWriteError(int err = 0);

private:

	int _ErrorFlag; /**< class Error flag , zero no error*/
	size_t printNumber(unsigned long, uint8_t);
	size_t printFloat(double, uint8_t);


/*!
	@brief Called from vector template Format an element based on its type and the format parameter
	@tparam T Type of the element
	@param element The element to format
	@param format Format specifier
	@return Formatted string representation of the element
*/
template <typename T>
std::string formatElement(const T &element, int format) {
	std::ostringstream oss;

	if constexpr (std::is_integral_v<T>) {
		// Handle integral types with base conversion
		switch (format) {
			case RDL_DEC:
				oss << element; // Decimal
				break;
			case RDL_HEX:
				oss << std::hex << std::uppercase << element; // Hexadecimal
				break;
			case RDL_OCT:
				oss << std::oct << element; // Octal
				break;
			case RDL_BIN: {
				// Binary conversion using std::bitset without leading zeros
				std::string binary = std::bitset<sizeof(T) * 8>(element).to_string();
				binary.erase(0, binary.find_first_not_of('0')); // Strip leading zeros
				oss << (binary.empty() ? "0" : binary); // Handle the case for 0
				break;
			}
			default:
				oss << element; // Default to decimal
		}
	} else if constexpr (std::is_floating_point_v<T>) {
		// Handle floating-point types with fixed precision
		oss << std::fixed << std::setprecision(format) << element;
	} else {
		// Handle other types (fallback)
		oss << element;
	}

	return oss.str();
}

	/*!
		@brief Called from vector & string template , Get the default format for a given type
		@tparam T Type of the element
		@return Default format
	*/
	template <typename T>
	static constexpr int defaultFormat() {
		if constexpr (std::is_floating_point_v<T>) {
			return 2; // Default decimal places for floats/doubles
		} else if constexpr (std::is_integral_v<T>) {
			return RDL_DEC; // Default base for integers
		}
		return 0; // Fallback for unsupported types
	}


};
