#pragma once

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <vector>

class Page {
public:
	const static size_t PAGE_SIZE = 4096;
	typedef struct {
		uint8_t pdFlags;
		uint16_t pdLower;
		uint16_t pdUpper;
		uint16_t pdSpecial;
	} Header;
	const static uint8_t FLAG_PAGE_FULL = 0b00000001;
private:
	uint8_t* data;
public:
	Page(int fd, uint32_t page);
	Page(const Page& other);
	Page operator=(const Page& other);
	~Page();

	Header* getHeader() { return (Header*)data; }

	std::vector<uint16_t> getLinePointers();

	uint8_t* getTuple(uint16_t linePointer, uint16_t nextLinePointer);
};