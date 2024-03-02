/*!
	@file     print_data_RDL.hpp
	@brief    Base class that provides print() and println() for for the display library
			Project Name :: Display_Lib_RPI
	@note  Port of arduino built-in print class, G Lyons 2022.
*/

#pragma once

#include <cinttypes>
#include <cstdio> // for size_t
#include <cstring>
#include <cmath>
#include <string>

#define RDL_DEC 10
#define RDL_HEX 16
#define RDL_OCT 8
#define RDL_BIN 2

/*!
	@brief Polymorphic print class to print many data types by wrapping write function in the graphics class's
*/
class Print
{
public:
	Print() : write_error(0) {}

	int getWriteError() { return write_error; }
	void clearWriteError() { setWriteError(0); }

	virtual size_t write(uint8_t) = 0;
	size_t write(const char *str) {
	  if (str == nullptr) return 0;
	  return write((const uint8_t *)str, strlen(str));
	}
	virtual size_t write(const uint8_t *buffer, size_t size);
	size_t write(const char *buffer, size_t size) {
	  return write((const uint8_t *)buffer, size);
	}

	// default to zero, meaning "a single write may block"
	// should be overriden by subclasses with buffering
	virtual int availableForWrite() { return 0; }

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
protected:
	void setWriteError(int err = 1) { write_error = err; }
private:
	int write_error; /*! class Error flag */
	size_t printNumber(unsigned long, uint8_t);
	size_t printFloat(double, uint8_t);

};


