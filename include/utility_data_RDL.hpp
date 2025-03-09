/*!
	@file utility_data_RDL.hpp
	@brief A file to hold some utilities(config file , logging etc) , included in common_data_RDL.

	@details
		Defines the logging(logData) class template for error handling with logging support.
		This file provides a templated exception class that includes error message storage,
		source location tracking, and optional logging to a file.
*/

#pragma once

#include <string>
#include <utility>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <source_location>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <filesystem>
#include <cmath>  // For std::cos, std::sin  std::lerp

/*! @brief Name space for config file related utilities*/
namespace rdlib_config 
{
	void loadConfig();
	bool isLoggingEnabled();
	bool isDebugEnabled();
	std::string getErrorLogPath();
}

/*! @brief Name space for maths related utilities*/
namespace rdlib_maths 
{
	int mapValue(int x, int in_min, int in_max, int out_min, int out_max);
	float sineFromDegrees(float angle);
	float cosineFromDegrees(float angle);
}

/*! @brief Name space for log file related utilities*/
namespace rdlib_log 
{

std::ostream& operator<<(std::ostream& os, const std::source_location& location);

/*!
	@brief Exception class template for handling and logging errors.
	@tparam DATA_T Type of additional data to store with the error.
*/
template<typename DATA_T>
class logData {
	public:
		/*!
			@brief Constructs an OmegaException with an error message and user data.
			@param str The error message.
			@param data Additional user data related to the error.
			@param loc Source location of the error (defaults to current location).
		*/
		logData(std::string str, DATA_T data, const std::source_location& loc =
			std::source_location::current())
			: errorString(std::move(str)), userData(std::move(data)), errorLocation(loc) {
			if (rdlib_config::isLoggingEnabled()) {
				logError();
			}
		}
		/*!
			@brief Retrieves the error message.
			@return A reference to the error message string.
		*/
		std::string& what() { return errorString; }
		/*!
		@brief Retrieves the error message (const version).
		@return A constant reference to the error message string.
		*/
		const std::string& what() const noexcept { return errorString; }
		/*!
			@brief Retrieves the source location of the exception.
			@return A constant reference to the source location.
		*/
		const std::source_location& where() const noexcept { return errorLocation; }
		/*!
			@brief Retrieves the user-defined data.
			@return A reference to the user data.
		*/
		DATA_T& data() { return userData; }
		/*!
			@brief Retrieves the user-defined data (const version).
			@return A constant reference to the user data.
		*/
		const DATA_T& data() const noexcept { return userData; }

private:
	std::string errorString; /**< The error message */
	DATA_T userData; /**< Additional data associated with the error. */
	const std::source_location errorLocation; /**< The source location of the error.*/
	/*!
		@brief Logs the error message to a file.
		If the logfile cannot be opened, an error message is printed to std::cerr.
	*/
	void logError() const {
		std::ofstream logFile(rdlib_config::getErrorLogPath(), std::ios::app);
		if (logFile) {
			// Get current time
			auto now = std::chrono::system_clock::now();
			auto timeT = std::chrono::system_clock::to_time_t(now);
			logFile << "[" << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S") << "]\n"
					<< "Info: " << errorString << "\n"
					<< "Data: " << userData << "\n"
					<< "Location: " << errorLocation << "\n"
					<< "-----------------------------\n";
		} else {
			std::cerr << "Failed to open log file: " << rdlib_config::getErrorLogPath() << "\n";
			perror("Error reason"); // Print system error message
		}
	}
};

}
