/*! 
 * @brief Utility to find bitmap file header size  and pixel data offset
 * for a given bitmap file
 * */

#include <iostream>
#include <fstream>

#pragma pack(push, 1)  // Ensure no padding in the struct
struct BMPFileHeader {
	char signature[2];   // "BM"
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t pixelDataOffset;
};
#pragma pack(pop)

void readBMPHeader(const char* filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Error: Cannot open file\n";
		return;
	}

	BMPFileHeader header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));

	std::cout << "Signature: " << header.signature[0] << header.signature[1] << "\n";
	std::cout << "File Size: " << header.fileSize << " bytes\n";
	std::cout << "Pixel Data Offset: " << header.pixelDataOffset << " bytes\n";

	file.close();
}

int main() {
	readBMPHeader("16pic1_96x64.bmp");  // Change to your BMP file
	return 0;
}
