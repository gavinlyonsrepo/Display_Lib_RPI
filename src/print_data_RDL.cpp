/*!
	@file    print_data_RDL.cpp
	@brief   Base class that provides print() and println() for the display library
	@note  Port of arduino built-in print class, G Lyons 2022.
*/

#include "../include/print_data_RDL.hpp"

// ==== Public Methods ====

// = print =

/*! 
	@brief default implementation: may be overridden 
	@param buffer of ints
	@param size
*/
size_t Print::write(const uint8_t *buffer, size_t size)
{
	size_t n = 0;
	while (size--) {
	if (write(*buffer++)) n++;
	else break;
	}
	return n;
}

/*!
	@brief print an array
	@param str An array of characters
*/
size_t Print::print(const char str[])
{
	return write(str);
}

/*!
	@brief print an character
	@param c the character to print
*/
size_t Print::print(char c)
{
	return write(c);
}

/*!
	@brief print an integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::print(int n, int base)
{
	return print((long) n, base);
}

/*!
	@brief print an unsigned integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::print(unsigned int n, int base)
{
	return print((unsigned long) n, base);
}

/*!
	@brief print an long  integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::print(long n, int base)
{
	if (base == 0)
	{
		return write(n);
	} else if (base == 10)
	{
		if (n < 0)
		{
			int t = print('-');
			n = -n;
			return printNumber(n, 10) + t;
		}
		return printNumber(n, 10);
	} else
	{
		return printNumber(n, base);
	}
}

/*!
	@brief print an unsigned long integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::print(unsigned long n, int base)
{
	if (base == 0) return write(n);
	else return printNumber(n, base);
}

/*!
	@brief print an double 
	@param n integer to print
	@param digits number of digits to print
	@details print(3.7468,2) = will print 3.75 on display
*/
size_t Print::print(double n, int digits)
{
	return printFloat(n, digits);
}

/*!
	@brief print an C++ string object
	@param s string object
*/
size_t Print::print(const std::string &s) {
	return write(s.c_str(), s.length());
}

// = println =

/*!
	@brief Goto to new line
*/
size_t Print::println(void)
{
	return write("\r\n");
}

/*!
	@brief print an array followed by new line
	@param c An array of characters
*/
size_t Print::println(const char c[])
{
	size_t n = print(c);
	n += println();
	return n;
}

/*!
	@brief print an character followed by new line
	@param c the character to print
*/
size_t Print::println(char c)
{
	size_t n = print(c);
	n += println();
	return n;
}

/*!
	@brief print an integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::println(int num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an unsigned integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::println(unsigned int num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an long  integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::println(long num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an unsigned long integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
*/
size_t Print::println(unsigned long num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an double 
	@param num float to print followed by new line
	@param digits number of digits to print
	@details print(3.7468,2) = will print 3.75 on display
*/
size_t Print::println(double num, int digits)
{
	size_t n = print(num, digits);
	n += println();
	return n;
}

/*!
	@brief print an C++ string object followed by new line
	@param s string object
*/
size_t Print::println(const std::string &s) {
	size_t n = print(s);
	n += println();
	return n;
}



// ==== Private Methods ====

/*!
 *@brief Used internally to parse and print number
 *@param n Number to parse
 *@param base to print to Binary hexadecimal etc
 */
size_t Print::printNumber(unsigned long n, uint8_t base)
{
	char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];

	*str = '\0';

	// prevent crash if called with base == 1
	if (base < 2) base = 10;

	do {
	char c = n % base;
	n /= base;

	*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n);

	return write(str);
}

/*!
 *@brief Used internally to parse float
 *@param number The floating point number
 *@param digits Number of precision digits to print
 */
size_t Print::printFloat(double number, uint8_t digits)
{
	size_t n = 0;

	if (std::isnan(number)) return print("nan");
	if (std::isinf(number)) return print("inf");
	if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
	if (number <-4294967040.0) return print ("ovf");  // constant determined empirically

	// Handle negative numbers
	if (number < 0.0)
	{
	 n += print('-');
	 number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
	rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	n += print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
	n += print('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
	remainder *= 10.0;
	unsigned int toPrint = (unsigned int)(remainder);
	n += print(toPrint);
	remainder -= toPrint;
	}

	return n;
}
