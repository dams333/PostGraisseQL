#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "Page.hpp"

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <path to file>" << std::endl;
		return 1;
	}
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		std::cerr << "Failed to open file: " << argv[1] << std::endl;
		return 1;
	}
	uint32_t pageCount;
	if (read(fd, &pageCount, sizeof(pageCount)) != sizeof(pageCount)) {
		std::cerr << "Failed to read page count" << std::endl;
		return 1;
	}
	if (argc == 2) {
		std::cout << "Page count: " << pageCount << std::endl;
		return 0;
	}
	uint32_t pageIdx = std::stoi(argv[2]);
	if (pageIdx >= pageCount) {
		std::cerr << "Invalid page number" << std::endl;
		return 1;
	}
	Page page = Page(fd, pageIdx);

	Page::Header* header = page.getHeader();
	std::cout << "======== Header (size: " << sizeof(Page::Header) << ") ========" << std::endl;
	std::cout << "pdFlags: " << (int)header->pdFlags << std::endl;
	std::cout << "\tPage full: " << (header->pdFlags & Page::FLAG_PAGE_FULL ? "yes" : "no") << std::endl;
	std::cout << "pdLower: " << header->pdLower << std::endl;
	std::cout << "\t After header: " << header->pdLower - sizeof(Page::Header) << std::endl;
	std::cout << "pdUpper: " << header->pdUpper << std::endl;
	std::cout << "\tAfter header: " << header->pdUpper - sizeof(Page::Header) << std::endl;
	std::cout << "\tAfter pdLower (free space): " << header->pdUpper - header->pdLower << std::endl;
	std::cout << "pdSpecial: " << header->pdSpecial << std::endl;
	std::cout << "\tSize:" << Page::PAGE_SIZE - header->pdSpecial << std::endl;

	std::vector<uint16_t> linePointers = page.getLinePointers();
	std::cout << "======== Line pointers (size: " << linePointers.size() << ") ========" << std::endl;
	for (size_t i = 0; i < linePointers.size(); i++) {
		std::cout << "Line pointer " << i << ": " << linePointers[i] << std::endl;
	}

	for (size_t i = 0; i < linePointers.size(); i++) {
		uint16_t linePointer = linePointers[i];
		uint16_t nextLinePointer = i == 0 ? header->pdSpecial : linePointers[i - 1];
		std::cout << "======== Tuple " << i << " ========" << std::endl;
		std::cout << "Begin: " << linePointer << std::endl;
		std::cout << "End: " << nextLinePointer << std::endl;
		std::cout << "Size: " << nextLinePointer - linePointer << std::endl;
		uint8_t* tuple = page.getTuple(linePointer, nextLinePointer);
		std::cout << "Value: ";
		for (size_t j = 0; j < nextLinePointer - linePointer; j++) {
			std::cout << (int)tuple[j] << " ";
		}
		std::cout << std::endl;
		free(tuple);
	}
	return 0;
}