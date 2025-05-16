/*!
	@file examples/misc_test/unit_test_color16_class/main.cpp
	@brief Library test file, st7735, unit testing 16-bt color graphics class , bitmap and texts
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function
	@test
	-# Test 803 Unit testing bitmap functions, results to console
	-# Test 802 Unit testing text functions, results to console
	-# Test 801 Unit testing graphics results to console
*/

// Section ::  libraries
#include "Bitmap_test_data.hpp"
#include "ST7735_TFT_LCD_RDL.hpp"

/// @cond

// Section :: Defines

// Section :: Globals
ST7735_TFT myTFT;
int8_t RST_TFT  = 25; // Reset GPIO
int8_t DC_TFT   = 24; // DC GPIO
int  GPIO_CHIP_DEV = 0; // GPIO chip device number

uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
uint16_t TFT_WIDTH = 128;// Screen width in pixels
uint16_t TFT_HEIGHT= 128; // Screen height in pixels

int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
int SPI_SPEED =  1000000; // The speed of serial communication in bits per second.
int SPI_FLAGS = 0; // last 2 LSB bits define SPI mode, see readme, mode 0 for this device

//  Section ::  Function Headers

uint8_t Setup(void); // setup + user options
bool Test803(void);
bool Test802(void);
bool Test801(void);
void DisplayReset(void);
void EndTests(bool);

//  Section ::  MAIN loop

int main(void)
{

	if(Setup() != 0)return -1;
	bool result801 = Test801();
	bool result802 = Test802();
	bool result803 = Test803();
	bool ErrorFlag = result801 && result802 && result803;  // Combine results
	
	printf("=== Final Test Summary ===\n");
	printf("Test801 Shapes testing result: %s\n", result801 ? "PASS" : "FAIL");
	printf("Test802 Text   testing result: %s\n", result802 ? "PASS" : "FAIL");
	printf("Test803 Bitmap testing result: %s\n", result803 ? "PASS" : "FAIL");
	printf("Overall Test result 16-bit   : %s\n\n", ErrorFlag ? "PASS" : "FAIL");
	EndTests(ErrorFlag);
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space

uint8_t Setup(void)
{
	printf("Start Test \n");
// ** USER OPTION 1 GPIO HW SPI **
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
//*********************************************
// ** USER OPTION 2 Screen Setup **
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************
// ** USER OPTION 3 PCB_TYPE + SPI settings**
	// pass enum to param1 ,4 choices,see README
	if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SPI_DEV, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rdlib::Success)
	{
		return 3;
	}
//*****************************
	delayMilliSecRDL(50);
	return 0;
}

bool Test801(void)
{
	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		rdlib::Success, rdlib::Success,
		rdlib::ShapeScreenBounds,  rdlib::ShapeScreenBounds, //drawDotGrid
		rdlib::ShapeScreenBounds,  rdlib::ShapeScreenBounds, //drawFastVLine
		rdlib::ShapeScreenBounds,  rdlib::ShapeScreenBounds, //drawFastHLine
		rdlib::ShapeScreenBounds,  rdlib::ShapeScreenBounds, //fillRectangle
		rdlib::GenericError, rdlib::GenericError      //drawPolygon
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test variables
	char testString5[] = "Error Check, results to console";
	bool errorFlag = false;

	// === Tests===
	printf("\n=== Test 801 graphics shapes Unit testing. Expecting errors ===\r\n");
	// Perform function calls and store return values

	// Print message + sanity check for success
	myTFT.setFont(font_default);
	returnValues.push_back(myTFT.writeCharString(5, 55, testString5)); 
	delayMilliSecRDL(5000);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	returnValues.push_back(myTFT.drawPolygon(50, 50, 5, 25, 0, true, myTFT.RDLC_WHITE));
	//drawDotGrid
	returnValues.push_back(myTFT.drawDotGrid(130, 0, TFT_WIDTH, TFT_HEIGHT, 5, myTFT.RDLC_GREEN));
	returnValues.push_back(myTFT.drawDotGrid(0, 130, TFT_WIDTH, TFT_HEIGHT, 5, myTFT.RDLC_GREEN));
	delayMilliSecRDL(500);
	//drawFastVLine
	returnValues.push_back(myTFT.drawFastVLine(130, 1,TFT_HEIGHT, myTFT.RDLC_LBLUE));
	returnValues.push_back(myTFT.drawFastVLine(1, 130, TFT_HEIGHT, myTFT.RDLC_LBLUE));
	delayMilliSecRDL(500);
	//drawFastHLine
	returnValues.push_back(myTFT.drawFastHLine(130, 1,TFT_WIDTH, myTFT.RDLC_LBLUE));
	returnValues.push_back(myTFT.drawFastHLine(1, 130, TFT_WIDTH, myTFT.RDLC_LBLUE));
	delayMilliSecRDL(500);
	//fillRectangle
	returnValues.push_back(myTFT.fillRectangle(130, 1, TFT_WIDTH, TFT_HEIGHT, myTFT.RDLC_GREEN));
	returnValues.push_back(myTFT.fillRectangle(1, 130, TFT_WIDTH, TFT_HEIGHT, myTFT.RDLC_GREEN));
	delayMilliSecRDL(500);
	//drawPolygon
	returnValues.push_back(myTFT.drawPolygon(50, 50, 14, 45, 45, false, myTFT.RDLC_WHITE));
	returnValues.push_back(myTFT.drawPolygon(50, 50, 1, 50, 45, true, myTFT.RDLC_WHITE));
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
	printf("=== End of Test 801 ===\r\n\n");
	return !errorFlag;
}

bool Test803(void)
{
	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		rdlib::Success,
		rdlib::BitmapScreenBounds, rdlib::BitmapScreenBounds, rdlib::BitmapDataEmpty, //icon
		rdlib::BitmapScreenBounds, rdlib::BitmapScreenBounds, rdlib::BitmapDataEmpty, rdlib::BitmapSize, //sprite
		rdlib::BitmapScreenBounds, rdlib::BitmapScreenBounds, rdlib::BitmapDataEmpty, rdlib::BitmapHorizontalSize, //1-bit bitmap
		rdlib::BitmapScreenBounds, rdlib::BitmapScreenBounds, rdlib::BitmapDataEmpty, rdlib::BitmapSize, //16-bit bitmap
		rdlib::BitmapScreenBounds, rdlib::BitmapScreenBounds, rdlib::BitmapDataEmpty, rdlib::BitmapSize //24-bit bitmap
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test variables
	char testString5[] = "Error Check, results to console";
	bool errorFlag = false;

	// === Tests===
	printf("\n=== Test 803 Bitmap Unit testing. Expecting errors ===\r\n");
	// Perform function calls and store return values

	// Print message + sanity check for success
	myTFT.setFont(font_default);
	returnValues.push_back(myTFT.writeCharString(5, 55, testString5)); 
	delayMilliSecRDL(5000);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	//TFTdrawIcon
	returnValues.push_back(myTFT.drawIcon(350, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, SignalIconVa));
	returnValues.push_back(myTFT.drawIcon(180, 350, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, SignalIconVa));
	returnValues.push_back(myTFT.drawIcon(40, 40, 16, myTFT.RDLC_BLACK, myTFT.RDLC_WHITE, emptyBitmap));
	delayMilliSecRDL(1000);
	//TFTdrawSprite
	returnValues.push_back(myTFT.drawSprite(350, 50, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE));
	returnValues.push_back(myTFT.drawSprite(40, 350, SpriteTest16, 32, 32, myTFT.RDLC_LBLUE));
	returnValues.push_back(myTFT.drawSprite(40, 180, emptyBitmap, 32, 32, myTFT.RDLC_LBLUE));
	returnValues.push_back(myTFT.drawSprite(40, 40, SpriteTest16, 48, 32, myTFT.RDLC_LBLUE, false));
	delayMilliSecRDL(1000);
	//TFTdrawBitmap
	returnValues.push_back(myTFT.drawBitmap(350, 65, 128, 128, myTFT.RDLC_WHITE, myTFT.RDLC_GREEN, BackupMenuBitmap));
	returnValues.push_back(myTFT.drawBitmap(50, 350, 128, 128, myTFT.RDLC_WHITE, myTFT.RDLC_GREEN, BackupMenuBitmap));
	returnValues.push_back(myTFT.drawBitmap(50, 65, 128, 128, myTFT.RDLC_WHITE, myTFT.RDLC_GREEN,  emptyBitmap));
	returnValues.push_back(myTFT.drawBitmap(20, 20, 70, 128, myTFT.RDLC_WHITE, myTFT.RDLC_GREEN, BackupMenuBitmap));
	delayMilliSecRDL(1000);
	//TFTdrawBitmap16
	returnValues.push_back(myTFT.drawBitmap16(350, 50, SpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16(40, 350, SpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16(40, 180,  emptyBitmap, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16(40, 40,  SpriteTest16, 50, 32));
	delayMilliSecRDL(1000);
	//TFTdrawBitmap24
	returnValues.push_back(myTFT.drawBitmap24(340, 50, SpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap24(40, 310, SpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap24(40, 180,  emptyBitmap, 32, 32));
	returnValues.push_back(myTFT.drawBitmap24(40, 40,  SpriteTest16, 50, 32));
	delayMilliSecRDL(1000);
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
		printf("\nUnit testing Bitmap has FAILED.\n");
	}else{
		printf("\nUnit testing Bitmap has PASSED.\n");
	}
	printf("=== End of Test 803 ===\r\n\n");
	return !errorFlag;
}

bool Test802(void)
{
	// Unit testing
	printf("==== Test 802 Start Text Unit testing ====\n");
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	// Define the expected return values, note the numeric are 
	// from  print which returns num of characters it attempted to print
	std::vector<uint8_t> expectedErrors = 
	{
		3, rdlib::CharFontASCIIRange, 3, rdlib::CharFontASCIIRange, 5, rdlib::Success, 
		0, rdlib::CharArrayNullptr,
		rdlib::Success, rdlib::CharFontASCIIRange, rdlib::CharFontASCIIRange,
		rdlib::CharFontASCIIRange, rdlib::CharScreenBounds, rdlib::CharScreenBounds,
		rdlib::CharScreenBounds, rdlib::CharScreenBounds, rdlib::CharArrayNullptr
	};
	
	// Vector to store return values
	std::vector<uint8_t> returnValues; 

	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = "-;A";
	bool errorFlag = false;
	myTFT.setFont(font_gll);

	// (1) Print statement test, gll font lower case letters
	// We check print class error flag as print statement does not return errors it returns num of characters
	myTFT.setCursor(40,40);
	returnValues.push_back(myTFT.print("ABc")); // Print AB , return 3 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return error
	returnValues.push_back(myTFT.print("abC"));  // print C , return 3 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return error
	returnValues.push_back(myTFT.print("12345")); // print 12345 , return 5 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return pass
	returnValues.push_back(myTFT.print(nullptr));  // return 0 (num of characters)
	returnValues.push_back(myTFT.getWriteError());  // return error
	DisplayReset();
	printf("========\r\n");
	// (2) writeChar + writeCharString
	// gll lower case + ] character out of font bounds
	returnValues.push_back(myTFT.writeChar(32, 0, '!')); //success
	returnValues.push_back(myTFT.writeCharString(5,  5, testlowercase)); //throw gll font error 2
	DisplayReset();
	// Numeric extended bounds ; , A errors
	myTFT.setFont(font_sixteenSeg);
	returnValues.push_back(myTFT.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myTFT.writeChar(32, 0, ',')); //throw error 2
	DisplayReset();
	printf("========\r\n");
	// screen out of bounds
	myTFT.setFont(font_default);
	returnValues.push_back(myTFT.writeChar(0, 400, 'e')); //throw error 1
	returnValues.push_back(myTFT.writeChar(400, 0, 'f')); //throw error 1
	DisplayReset();
	myTFT.setFont(font_orla);
	returnValues.push_back(myTFT.writeChar(0, 400, 'A')); //throw error 1
	returnValues.push_back(myTFT.writeChar(400, 0, 'B')); //throw error 1
	DisplayReset();
	
	returnValues.push_back(myTFT.writeCharString(5, 5, nullptr)); //throw error 
	
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
	printf("=== End of Test 802 ===\r\n");
	return !errorFlag;
}

void EndTests(bool errorFlag)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(font_retro);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	errorFlag ? myTFT.fillScreen(myTFT.RDLC_GREEN) : myTFT.fillScreen(myTFT.RDLC_RED);
	delayMilliSecRDL(5000);
	myTFT.TFTPowerDown(); // Power down device
	std::cout << "End Test" << std::endl;
}

void DisplayReset(void)
{
	delayMilliSecRDL(5000);
	myTFT.fillScreen(myTFT.RDLC_BLACK);
}
/// @endcond
// *************** EOF ****************
