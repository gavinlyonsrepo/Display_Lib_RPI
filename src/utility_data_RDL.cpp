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
