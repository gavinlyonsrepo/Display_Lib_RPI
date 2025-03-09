/*!
	@file examples/misc_test/unit_test_bicolor_class/main.cpp
	@brief Test file for SSD1306_OLED display 
		showing unit testing bi-color graphics class , graphics text and bitmaps.
	@test
		-# Test 805 Unit testing Bitmap  bi-color library
		-# Test 804 Unit testing Text bi-color library
*/

#include "Bitmap_test_data.hpp" // Bitmap test data file
#include "SSD1306_OLED_RDL.hpp"

/// @cond

// Screen related
#define MY_OLED_WIDTH  128
#define MY_OLED_HEIGHT 64
#define FULLSCREEN (MY_OLED_WIDTH * (MY_OLED_HEIGHT/8))
uint8_t screenBuffer[FULLSCREEN];
SSD1306_RDL myOLED(MY_OLED_WIDTH ,MY_OLED_HEIGHT) ; // instantiate  an object
// I2C related
#define OLED_I2C_ADDRESS 0x3C
#define OLED_I2C_DEVICE 1
#define OLED_I2C_FLAGS 0

// =============== Function prototype ================
bool SetupTest(void);
void myTests(void);
void EndTests(void);
void TestReset(void);
bool Test806(void);
bool Test805(void);
bool Test804(void);

// ======================= Main ===================
int main(void)
{
	if(!SetupTest()) return -1;
	myTests();
	EndTests();
	return 0;
}
// ======================= End of main  ===================

// ======================= Function space ===================
bool SetupTest()
{
	printf("Test Begin\r\n\n");
	// Open  on I2C device
	if(myOLED.OLED_I2C_ON(OLED_I2C_DEVICE, OLED_I2C_ADDRESS,OLED_I2C_FLAGS) != rdlib::Success)
	{
		printf("Error 1201:Cannot open I2C device bus\n");
		return false;
	}
	// Check if OLED on Bus
	if(myOLED.OLEDCheckConnection() < 0)
	{
		printf("Error 1202 : Cannot See Device on Bus\n");
		return false;
	}
	delayMilliSecRDL(500);
	myOLED.OLEDbegin(); // initialize the OLED
	myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars, optional just for effect
	delayMilliSecRDL(1000);
	return true;
}


void EndTests()
{
	myOLED.OLEDPowerDown(); //Switch off display
	myOLED.OLED_I2C_OFF(); // Switch off I2C , optional may effect other programs & devices
	printf("OLED Test End\r\n");
}

void myTests()
{
	if (myOLED.OLEDSetBufferPtr(MY_OLED_WIDTH, MY_OLED_HEIGHT, screenBuffer ) != rdlib::Success) return;
	myOLED.OLEDclearBuffer();
	bool result804 = Test804();
	bool result805 = Test805();
	bool result806 = Test806();
	bool ErrorFlag = result804 && result805 && result806;  // Combine results
	printf("=== Final Test Summary ===\n");
	printf("Test804 Text testing result  : %s\n", result804 ? "PASS" : "FAIL");
	printf("Test805 Bitmap testing result: %s\n", result805 ? "PASS" : "FAIL");
	printf("Test806 Shapes testing result: %s\n", result806 ? "PASS" : "FAIL");
	printf("Overall Test result 1-bit    : %s\n\n", ErrorFlag ? "PASS" : "FAIL");
}

void TestReset(void){
	myOLED.OLEDupdate();
	delayMilliSecRDL(3000);
	myOLED.OLEDclearBuffer();
}

//  Bitmap Unit testing test
bool Test805(void)
{

	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		rdlib::Success,
		rdlib::BitmapScreenBounds, rdlib::BitmapScreenBounds,
		rdlib::BitmapLargerThanScreen, rdlib::BitmapDataEmpty,
		rdlib::BitmapHorizontalSize,  rdlib::BitmapSize, rdlib::BitmapVerticalSize, rdlib::BitmapSize
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test variables
	myOLED.setFont(font_gll);
	char testString5[] = "Error     Check     Bitmap";
	bool errorFlag = false;

	// === Tests, Perform function calls and store return values ===
	printf("=== START Test 805 Bitmap Unit testing. ===\r\n");
	// Print message + sanity check for success
	myOLED.setFont(font_default);
	returnValues.push_back(myOLED.writeCharString(0, 0, testString5)); 
	delayMilliSecRDL(2000);
	myOLED.OLEDFillScreen(0x00, 0);
	myOLED.OLEDclearBuffer();
	myOLED.setDrawBitmapAddr(false);
	returnValues.push_back(myOLED.drawBitmap(140, 0, SignalIconHa, 16, 8, myOLED.BLACK, myOLED.WHITE));
	returnValues.push_back(myOLED.drawBitmap(0, 90, SignalIconHa, 16, 8, myOLED.BLACK, myOLED.WHITE));
	returnValues.push_back(myOLED.drawBitmap(0, 0, SignalIconHa, 140, 90, myOLED.BLACK, myOLED.WHITE));
	returnValues.push_back(myOLED.drawBitmap(0, 0, emptyBitmap, 16, 8, myOLED.BLACK, myOLED.WHITE));
	returnValues.push_back(myOLED.drawBitmap(0, 0, SignalIconHa, 19, 8, myOLED.BLACK, myOLED.WHITE));
	returnValues.push_back(myOLED.drawBitmap(0, 0, SignalIconHa, 32, 8, myOLED.BLACK, myOLED.WHITE));
	myOLED.setDrawBitmapAddr(true);
	returnValues.push_back(myOLED.drawBitmap(0, 0, SignalIconVa, 16, 11, myOLED.BLACK, myOLED.WHITE));
	returnValues.push_back(myOLED.drawBitmap(0, 0, SignalIconVa, 32, 8, myOLED.BLACK, myOLED.WHITE));
	//== SUMMARY SECTION===
	printf("\nUnit testing Summary.\n");
	// Check return values against expected errors
	for (size_t i = 0; i < returnValues.size(); ++i) {
		if (i >= expectedErrors.size() || returnValues[i] != expectedErrors[i]) {
			errorFlag = true;
			printf("Unexpected error code: %d at test case %zu (expected: %d)\n", 
				returnValues[i], i + 1, (i < expectedErrors.size() ? expectedErrors[i] : -1));
		}
	}

	// Print all expectedErrors for summary
	for (uint8_t value : expectedErrors ) 
	{
		printf("%d ", value);
	}
	printf("\n");
	// Print all returnValues for summary
	for (uint8_t value : returnValues) 
	{
		printf("%d ", value);
	}
	if (errorFlag == true ){
		printf("\nUnit testing Bitmap has FAILED.\n");
	}else{
		printf("\nUnit testing Bitmap has PASSED.\n");
	}
	printf("=== End of Test 805 Bitmap Unit testing. ===\r\n");
	return !errorFlag;
}


bool Test804(void)
{
	// Unit testing
	printf("==== Test 804 Start unit testing ====\n");
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		3, rdlib::CharFontASCIIRange, 3, rdlib::CharFontASCIIRange, 
		5, rdlib::Success, 0, rdlib::CharArrayNullptr,
		rdlib::Success, rdlib::CharFontASCIIRange, rdlib::CharFontASCIIRange,
		rdlib::CharFontASCIIRange, rdlib::CharScreenBounds, rdlib::CharScreenBounds,
		rdlib::CharScreenBounds, rdlib::CharScreenBounds, rdlib::CharArrayNullptr
	};
	
	// Vector to store return values
	std::vector<uint8_t> returnValues; 

	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = "-;A";
	bool errorFlag = false;
	myOLED.setFont(font_gll);

	// (1) Print statement test, gll font lower case letters
	// We check print class error flag as print statement does not return errors 
	// it returns num of characters attempted to print. 
	myOLED.setCursor(40,40);
	returnValues.push_back(myOLED.print("ABc")); // Print AB , return 3 (num of characters)
	returnValues.push_back(myOLED.getWriteError()); // return error
	returnValues.push_back(myOLED.print("abC"));  // print C , return 3 (num of characters)
	returnValues.push_back(myOLED.getWriteError()); // return error
	returnValues.push_back(myOLED.print("12345")); // print 12345 , return 5 (num of characters)
	returnValues.push_back(myOLED.getWriteError()); // return pass
	returnValues.push_back(myOLED.print(nullptr));  // return 0 (num of characters)
	returnValues.push_back(myOLED.getWriteError());  // return error
	TestReset();
	printf("========\r\n");
	// (2) writeChar + writeCharString
	// gll lower case + ] character out of font bounds
	returnValues.push_back(myOLED.writeChar(32, 0, '!')); //success
	returnValues.push_back(myOLED.writeCharString(5,  5, testlowercase)); //throw gll font error 2
	TestReset();
	// Numeric extended bounds ; , A errors
	myOLED.setFont(font_sixteenSeg);
	returnValues.push_back(myOLED.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myOLED.writeChar(32, 0, ',')); //throw error 2
	TestReset();
	printf("========\r\n");
	// screen out of bounds
	myOLED.setFont(font_default);
	returnValues.push_back(myOLED.writeChar(0, 140, 'e')); //throw error 1
	returnValues.push_back(myOLED.writeChar(140, 0, 'f')); //throw error 1
	TestReset();
	myOLED.setFont(font_orla);
	returnValues.push_back(myOLED.writeChar(0, 140, 'A')); //throw error 1
	returnValues.push_back(myOLED.writeChar(140, 0, 'B')); //throw error 1
	TestReset();

	returnValues.push_back(myOLED.writeCharString(5, 5, nullptr)); //throw error 

	//== SUMMARY SECTION===
	printf("\nUnit testing Summary.\n");
	// Check return values against expected errors
	for (size_t i = 0; i < returnValues.size(); ++i) {
		if (i >= expectedErrors.size() || returnValues[i] != expectedErrors[i]) {
			errorFlag = true;
			printf("Unexpected error code: %d at test case %zu (expected: %d)\n", 
				returnValues[i], i + 1, (i < expectedErrors.size() ? expectedErrors[i] : -1));
		}
	}
		// Print all expectedErrors for summary
	for (uint8_t value : expectedErrors ) 
	{
		printf("%d ", value);
	}
	printf("\n");
	// Print all returnValues for summary
	for (uint8_t value : returnValues) 
	{
		printf("%d ", value);
	}
	if (errorFlag == true ){
		printf("\nUnit testing Text has FAILED.\n");
	}else{
		printf("\nUnit testing Text has PASSED.\n");
	}
	printf("\n=== End of Test 804 ===\r\n");
	return !errorFlag;
}

bool Test806(void)
{
	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		rdlib::Success, rdlib::Success,
		rdlib::ShapeScreenBounds,  rdlib::ShapeScreenBounds, //drawDotGrid
		rdlib::ShapeScreenBounds,  rdlib::ShapeScreenBounds, //drawFastVLine
		rdlib::ShapeScreenBounds,  rdlib::ShapeScreenBounds, //drawFastHLine
		rdlib::GenericError, rdlib::GenericError      //drawPolygon
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test variables
	char testString5[] = "Error Check, results to console";
	bool errorFlag = false;

	// === Tests===
	printf("\n=== Test 806 graphics shapes Unit testing. Expecting errors ===\r\n");
	// Perform function calls and store return values

	// Print message + sanity check for success
	myOLED.setFont(font_default);
	returnValues.push_back(myOLED.writeCharString(5, 55, testString5)); 
	delayMilliSecRDL(5000);
	myOLED.fillScreen(myOLED.BLACK);
	returnValues.push_back(myOLED.drawPolygon(50, 50, 5, 25, 0, true, myOLED.WHITE));
	//drawDotGrid
	returnValues.push_back(myOLED.drawDotGrid(130, 0, MY_OLED_WIDTH, MY_OLED_HEIGHT, 5,myOLED.WHITE));
	returnValues.push_back(myOLED.drawDotGrid(0, 130, MY_OLED_WIDTH, MY_OLED_HEIGHT, 5, myOLED.WHITE));
	delayMilliSecRDL(500);
	//drawFastVLine
	returnValues.push_back(myOLED.drawFastVLine(130, 1, MY_OLED_HEIGHT, myOLED.WHITE));
	returnValues.push_back(myOLED.drawFastVLine(1, 130, MY_OLED_HEIGHT, myOLED.WHITE));
	delayMilliSecRDL(500);
	//drawFastHLine
	returnValues.push_back(myOLED.drawFastHLine(130, 1,MY_OLED_WIDTH, myOLED.WHITE));
	returnValues.push_back(myOLED.drawFastHLine(1, 130, MY_OLED_WIDTH, myOLED.WHITE));
	delayMilliSecRDL(500);
	//drawPolygon
	returnValues.push_back(myOLED.drawPolygon(50, 50, 12, 45, 45, false, myOLED.WHITE));
	returnValues.push_back(myOLED.drawPolygon(50, 50, 1, 50, 45, true, myOLED.WHITE));
	delayMilliSecRDL(500);
	//== SUMMARY SECTION===
	printf("Unit testing Summary.\n");
	// Check return values against expected errors
	for (size_t i = 0; i < returnValues.size(); ++i) {
		if (i >= expectedErrors.size() || returnValues[i] != expectedErrors[i]) {
			errorFlag = true;
			printf("Unexpected error code: %d at test case %zu (expected: %d)\n", 
				returnValues[i], i + 1, (i < expectedErrors.size() ? expectedErrors[i] : -1));
		}
	}
	// Print all expectedErrors for summary
	for (uint8_t value : expectedErrors ) 
	{
		printf("%d ", value);
	}
	printf("\n");
	// Print all returnValues for summary
	for (uint8_t value : returnValues) 
	{
		printf("%d ", value);
	}
	if (errorFlag == true ){
		printf("\nUnit testing graphics shapes has FAILED.\n");
	}else{
		printf("\nUnit testing graphics shapes has PASSED.\n");
	}
	printf("=== End of Test 806 ===\r\n\n");
	return !errorFlag;
}

/// @endcond
