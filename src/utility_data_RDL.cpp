/*!
	@file utility_data_RDL.cpp
	@brief A file to hold some utilities(maths, config, file logging etc) , included in common_data_RDL.

	@details
		Implements the operator<< function for std::source_location.
		This file provides an overloaded stream insertion operator to format and output
		source location information, aiding in debugging and logging.
		Implements the configuration file functionality 
*/

#include "utility_data_RDL.hpp"


namespace rdlib_maths
{

/**
 * @brief Maps a value from one range to another using linear interpolation.
 * This function takes an input value within a specified range and maps it
 * to a corresponding value in a different output range using std::lerp.
 * @param x The input value to map.
 * @param in_min The lower bound of the input range.
 * @param in_max The upper bound of the input range.
 * @param out_min The lower bound of the output range.
 * @param out_max The upper bound of the output range.
 * @return The mapped value in the output range.
 */
int mapValue(int x, int in_min, int in_max, int out_min, int out_max) 
{
	return static_cast<int>(std::lerp(out_min, out_max, (x - in_min) / static_cast<double>(in_max - in_min)));
}


/**
 * @brief Computes the cosine of an angle given in degrees.
 * This function converts the input angle from degrees to radians and then calculates 
 * the cosine of that angle using the standard C++ `cos` function.
 * @param angle The angle in degrees.
 * @return The cosine value of the given angle.
 */
float cosineFromDegrees(float angle)
{
	float radians = angle / 360.0f * 2.0f *  std::numbers::pi;
	return std::cos(radians);
}

/**
 * @brief Computes the sine of an angle given in degrees.
 * This function converts the input angle from degrees to radians and then calculates 
 * the sine of that angle using the standard C++ `sin` function.
 * @param angle The angle in degrees.
 * @return The sine value of the given angle.
 */
float sineFromDegrees(float angle)
{
	float radians = angle / 360.0f * 2.0f *  std::numbers::pi;
	return std::sin(radians);
}

/*!
	@brief This function is designed to return a color in the 16-bit 
		RGB format based on the input value, creating a smooth transition 
		between colors in different stages.
	@param value input value 1-127
	@return 16 bit color
	@details The gradient is calculated by 
		cycling through different color transitions as val increases, 
		allowing for smooth visual effects such as changing hues 
		or creating rainbow-like effects on an LCD. 1-127 will
		This will create a gradient color between blue and red based on the value.
*/
uint16_t generateColor(uint8_t value)
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t segment = value / 32;
	switch(segment) 
	{
		case 0: red = 0; green = 2 * (value % 32); blue = 31; break;
		case 1: red = 0; green = 63; blue = 31 - (value % 32); break;
		case 2: red = value % 32; green = 63; blue = 0; break;
		case 3: red = 31; green = 63 - 2 * (value % 32); blue = 0; break;
	}
	return (red << 11) + (green << 5) + blue;
}

/*!
	@brief: Blend two RGB565 colors using 8-bit mix factor
	@param c1 first 16-bit color (RGB565)
	@param c2 second 16-bit color (RGB565)
	@param amount blend level (0–255), 0=c1, 255=c2
	@return mixed 16-bit RGB565 color
	@details Performs channel-wise interpolation in 565 space.
			c1 blended towardc2 using linear scaling.
*/
uint16_t blend565(uint16_t c1, uint16_t c2, uint8_t amount)
{
	if (amount == 0)   return c1;
	if (amount == 255) return c2;

	// Extract RGB components
	uint32_t r1 = (c1 >> 11) & 0x1F;
	uint32_t g1 = (c1 >> 5)  & 0x3F;
	uint32_t b1 =  c1        & 0x1F;

	uint32_t r2 = (c2 >> 11) & 0x1F;
	uint32_t g2 = (c2 >> 5)  & 0x3F;
	uint32_t b2 =  c2        & 0x1F;

	// Blend each channel
	r1 = r1 + ((r2 - r1) * amount) / 255;
	g1 = g1 + ((g2 - g1) * amount) / 255;
	b1 = b1 + ((b2 - b1) * amount) / 255;

	// Recombine into RGB565
	return (r1 << 11) | (g1 << 5) | b1;
}

}


namespace rdlib_log 
{
/*!
	@brief Overloaded stream insertion operator for std::source_location.
	@param os Output stream.
	@param location Source location containing file name, line, column, and function name.
	@return Reference to the output stream.
*/
std::ostream& operator<<(std::ostream& os, const std::source_location& location) {
	os << location.file_name() << "(" << location.line() << ":" << location.column()
	   << "), function '" << location.function_name() << "'";
	return os;
}

}

namespace rdlib_config 
{

bool LOGGING_ENABLED = false; /**< is logging enabled ? */
bool  DEBUG_ENABLED = false; /**< is global debug enabled ? */
std::string ERROR_LOG_PATH = "/tmp/error.log"; /**< path to the log file */

/*!
 * @brief This function loads the config file into program
 * @details if config file does not exist it creates it 
 * at ($HOME)/.config/rdlib_config/config.cfg.
 * if ($HOME) not set uses pwd. Defaults debug and logging
 * settings to false. config file sets 3 settings 
 * -# LOGGING_ENABLED=false
 * -# DEBUG_ENABLED=false
 * -# ERROR_LOG_PATH="/tmp/error.log"
 */
void loadConfig() {
	
	std::string homeDir = std::getenv("HOME") ? std::getenv("HOME") : ".";
	// Define the configuration directory and file path
	std::string configDir = homeDir + "/.config/rdlib_config/";
	std::string configPath = configDir + "config.cfg";
	// Ensure that the configuration directory exists
	std::filesystem::create_directories(configDir);
	// Attempt to open the existing configuration file
	std::ifstream configFile(configPath);
	if (!configFile) 
	{
		// If the file doesn't exist, create a new one with default settings
		std::ofstream newFile(configPath);
		if (newFile) 
		{
			newFile << "LOGGING_ENABLED=false\n";
			newFile << "DEBUG_ENABLED=false\n";
			newFile << "ERROR_LOG_PATH=/tmp/rdlib_log.log\n";
		}
		configFile.open(configPath);
	}
	// Read configuration file line by line
	std::string line;
	while (std::getline(configFile, line)) 
	{
		std::istringstream ss(line);
		std::string key, value;
		// Parse the line into key-value pairs (format: key=value)
		if (std::getline(ss, key, '=') && std::getline(ss, value)) 
		{
			if (key == "LOGGING_ENABLED") {
				LOGGING_ENABLED = (value == "true");
			} else if (key == "DEBUG_ENABLED") {
				DEBUG_ENABLED = (value == "true");
			} else if (key == "ERROR_LOG_PATH") {
				ERROR_LOG_PATH = value;
			}
		}
	}
}

/*!
 * @brief retrieve the logging enabled setting , true for logging on
 * @return If logging is enabled 
*/
bool isLoggingEnabled() {
		return rdlib_config::LOGGING_ENABLED;
}
/*!
 * @brief retrieve the debug enabled setting , true for debug on
 * @return If debug is enabled 
*/
bool isDebugEnabled() {
		return rdlib_config::DEBUG_ENABLED;
}
/*!
 * @brief retrieve the log file path setting
 * @return path to the log file
 */
std::string getErrorLogPath() {
		return rdlib_config::ERROR_LOG_PATH;
	}

}
