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
	std::cout << "pdLower: " << header->pdLower << std::endl;
	std::cout << "\t After header: " << header->pdLower - sizeof(Page::Header) << std::endl;
	std::cout << "pdUpper: " << header->pdUpper << std::endl;
	std::cout << "\tAfter header: " << header->pdUpper - sizeof(Page::Header) << std::endl;
	std::cout << "\tAfter pdLower (free space): " << header->pdUpper - header->pdLower << std::endl;
	std::cout << "pdSpecial: " << header->pdSpecial << std::endl;
	std::cout << "\tSize:" << Page::PAGE_SIZE - header->pdSpecial << std::endl;
	return 0;
}