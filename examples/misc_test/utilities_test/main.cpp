/*!
	@file examples/misc_test/utilities_test/main.cpp
	@author Gavin Lyons
	@brief Test file to test : return code enum printing , logging and 
		config file functionality requires ch1115 spi (optional)
	Project Name: Display_Lib_RPI
	@test 
		-# Test 1050 Config file load and read
		-# Test 1051 enum return code printing
		-# Test 1052 logging to log file 
		-# Test 1053 debug flag
*/

#include "ERM1_CH1115_OLED_RDL.hpp"

/// @cond

//GPIO
const uint8_t RES = 25; // GPIO pin number pick any you want
const uint8_t DC = 24; // GPIO pin number pick any you want
int  GPIO_CHIP_DEVICE = 0; // GPIO chip device number usually 0

// Screen
const uint8_t MY_OLED_WIDTH  = 128;
const uint8_t MY_OLED_HEIGHT = 64;
#define myScreenSize (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8)) // 1024 bytes = 128 * 64/8
const uint8_t OLEDcontrast = 0x80; //Constrast 00 to FF , 0x80 is default.

// SPi
// Hardware SPI setup
int HWSPI_DEVICE = 0; // A SPI device, >= 0. which SPI interface to use
int HWSPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int HWSPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int HWSPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

ERMCH1115 myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT, RES, DC); // instantiate an object

// =============== Function prototype ================
bool Setup(void);
void TestLoop(void);
void EndTest(void);

// ======================= Main ===================
int main(void)
{
	std::cout << "Utility Test Begin" << std::endl;
	std::cout << std::endl;
	// Test 1050 load the config file, Will be created with default values if missing
	rdlib_config::loadConfig();
	//  print out values read
	std::cout << "Config file data ::"<<  std::endl;
	std::cout << "logging enabled: " << (rdlib_config::isLoggingEnabled() ? "true" : "false" )<< std::endl;
	std::cout << "debug enabled: " << (rdlib_config::isDebugEnabled() ? "true" : "false") << std::endl;
	std::cout << "logging file path: " << rdlib_config::getErrorLogPath() << std::endl;
	std::cout << std::endl;

	if(!Setup()) return -1;
	TestLoop();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
bool Setup(void)
{
	printf("OLED Begin\r\n");
	printf("lgpio library Version Number :: %i\r\n",lguVersion());
	printf("Display_LIB_RPI Library version number :: %u\r\n", rdlib::LibraryVersion()); 
	delayMilliSecRDL(50);

	if(myOLED.OLEDbegin(OLEDcontrast, HWSPI_DEVICE, HWSPI_CHANNEL, HWSPI_SPEED, HWSPI_FLAGS, GPIO_CHIP_DEVICE ) != rdlib::Success) // initialize the OLED
	{
		printf("Error 1202: Setup : Cannot start spi \r\n");
		return false;
	}
	delayMilliSecRDL(50);
	myOLED.OLEDFillScreen(0x0F); //splash screen bars
	delayMilliSecRDL(1500);
	return true;
}

void EndTest(void)
{
	myOLED.OLEDPowerDown();
	myOLED.OLEDSPIoff();
	printf("OLED End\r\n");
}


void TestLoop()
{
	// Buffer setup, Define a buffer to cover whole screen
	uint8_t screenBuffer[myScreenSize]; 
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer) != rdlib::Success) return;
	myOLED.OLEDclearBuffer(); // Clear buffer
	myOLED.setFont(font_default);
	myOLED.setCursor(20, 20);
	myOLED.print("Hello World.");
	myOLED.OLEDupdate();  //write to active buffer
	delayMilliSecRDL(1000);

	// Test 1051 Print out one return code enum and comment.
	rdlib::Return_Codes_e code = rdlib::WrongFont;
	std::cout << "One Enum Label: :" << rdlib::ReturnCodetoText(code) << std::endl;
	std::cout << std::endl;

	// Print out all return code enums and comments.
	std::cout << "All return codes list :"<<  std::endl;
	for (auto& code : rdlib::ReturnCodesStrings )
	{
		std::cout << code << std::endl;
	}
	std::cout << std::endl;

	// Test 1052 log data ( note logging is false by default, so config file will have to be modified
	
	// Test 1053 test debug 
	if (rdlib_config::isLoggingEnabled())
	{
		std::cout << "logging is enabled" << std::endl ;
	}else {
		std::cout <<"logging is disabled" << std::endl ;
	}
	int data = 45;
	rdlib_log::logData<int> error1("Logging Test 1", data);

	float data1 = 3.1456;
	rdlib_log::logData<float> error2("Logging Test 2",data1);

	std::string data2 = "my string";
	rdlib_log::logData<std::string> error3("Logging Test 3",data2);
	
	// Test 1053 test debug 
	if (rdlib_config::isDebugEnabled())
	{
		std::cout << "debug is enabled" << std::endl ;
	}else {
		std::cout <<"debug is disabled" << std::endl ;
	}
	std::cout << std::endl;
}

// ============== EOF =========

/// @endcond
