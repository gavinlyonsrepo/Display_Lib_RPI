/*!
	@file examples/ssd1306/UTILS_TEST/main.cpp
	@author Gavin Lyons
	@brief Test file to test : return code enum printing , logging and config file functionality
	Project Name: Display_Lib_RPI
	@test 
		-# Test 1050 Config file load and read
		-# Test 1051 enum return code printing
		-# Test 1052 logging to log file 
		-# Test 1053 debug flag
*/

#include <lgpio.h>
#include "SSD1306_OLED_RDL.hpp"


// Screen Size
#define MY_OLED_WIDTH  128
#define MY_OLED_HEIGHT 64
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))

// I2C
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_DEVICE 1
#define OLED_I2C_FLAGS 0

// instantiate a OLED object
SSD1306_RDL myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT) ;

// =============== Function prototype ================
bool SetupTest(void);
void TestLoop(void);
void EndTest(void);

// ======================= Main ===================
int main(void)
{
	std::cout << "Utility Test Begin" << std::endl;
	std::cout << "Display_Lib_RPI library Version Number :: " << rdlib::LibraryVersion() << std::endl;
	std::cout << "lgpio library Version Number :: " << lguVersion() << std::endl;
	std::cout << std::endl;
	// Test 1050 load the config file, Will be created with default values if missing
	rdlib_config::loadConfig();
	//  print out values read
	std::cout << "Config file data ::"<<  std::endl;
	std::cout << "logging enabled: " << (rdlib_config::isLoggingEnabled() ? "true" : "false" )<< std::endl;
	std::cout << "debug enabled: " << (rdlib_config::isDebugEnabled() ? "true" : "false") << std::endl;
	std::cout << "logging file path: " << rdlib_config::getErrorLogPath() << std::endl;
	std::cout << std::endl;

	if(!SetupTest()) return -1;
	TestLoop();
	EndTest();
	return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
bool SetupTest()
{
	// Open  on I2C device
	if(myOLED.OLED_I2C_ON(OLED_I2C_DEVICE, OLED_I2C_ADDRESS,OLED_I2C_FLAGS) != rdlib::Success)
	{
		std::cerr << "Error 1201:Cannot open I2C device bus" << std::endl;
		return false;
	}
	// Check if OLED on Bus
	if(myOLED.OLEDCheckConnection() < 0)
	{
		std::cerr << "Error 1202 : Cannot See Device on Bus" << std::endl;
		return false;
	}
	delayMilliSecRDL(500);
	myOLED.OLEDbegin(); // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars, optional just for effect
	delayMilliSecRDL(1000);
	return true;
}

void EndTest()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	std::cout << "Utility Test End" << std::endl;
}

void TestLoop()
{
	// print "test"
	uint8_t  screenBuffer[FULLSCREEN];
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer ) != rdlib::Success) return;
	myOLED.OLEDclearBuffer();
	myOLED.setFont(font_default);
	myOLED.setCursor(10, 10);
	myOLED.print("Test");
	myOLED.OLEDupdate();
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
	int data = 45;
	rdlib_log::logData<int> error1("Logging Test 1", data);

	float data1 = 3.1456;
	rdlib_log::logData<float> error2("Logging Test 2",data1);

	std::string data2 = "my string";
	rdlib_log::logData<std::string> error3("Logging Test 3",data2);
	
	// Test 1053 test debug 
	if (rdlib_config::isDebugEnabled())
	{
		std::cout << "debug info here" << std::endl ;
	}else {
		std::cout <<"debug is disabled" << std::endl ;
	}
	std::cout << std::endl;
}

// ============== EOF =========
