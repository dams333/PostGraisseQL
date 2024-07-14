#include "engine/Page.hpp"

using namespace engine;

Page::Page(): _data(nullptr) {}

Page::Page(uint8_t* data): _data(data) {}

Page::Page(const Page& page) {
	*this = page;
}

Page& Page::operator=(const Page& page) {
	if (this == &page) {
		return *this;
	}

	_data = page._data;

	return *this;
}

Page::~Page() {}

uint8_t *Page::getDefaultPage() {
	static uint8_t defaultPage[Page::PAGE_SIZE] = {0};
	Header *header = (Header*)defaultPage;
	header->pdFlags = 0;
	header->pdLower = sizeof(Header);
	header->pdUpper = Page::PAGE_SIZE;
	header->pdSpecial = Page::PAGE_SIZE;
	return defaultPage;
}

uint8_t *Page::data() {
	if (_data == nullptr) {
		throw std::runtime_error("Page data is null");
	}
	return _data;
}

size_t getFreeSpaceSize(Page::Header* header) {
	return header->pdUpper - header->pdLower;
}

//TODO: implements lock page when full
bool Page::canTupleFitInPage(size_t tupleSize, Header* header) {
	if (header->pdFlags & FLAG_PAGE_FULL) {
		return false;
	}
	size_t usedSpace = tupleSize + LINE_POINTER_SIZE;
	size_t freeSpace = getFreeSpaceSize(header);
	if (freeSpace < usedSpace) {
		return false;
	}
	size_t freeSpaceAfterInsert = freeSpace - usedSpace;
	if (freeSpaceAfterInsert < MIN_FREE_SPACE_AFTER_INSERT) {
		return false;
	}
	return true;
}

void Page::insertTuple(const uint8_t* tuple, size_t tupleSize) {
	// not optimal, we dont have to call insertTuple if canTupleFit is false
	// TODO: remove this check, just used for programming
	if (!canTupleFitInPage(tupleSize, header())) {
		throw std::runtime_error("Tuple does not fit in page");
	}
	Header *header = (Header*)_data;
	uint16_t *linePointer = (uint16_t*)(_data + header->pdLower);
	uint8_t *tupleDataPointer = _data + header->pdUpper - tupleSize;
	memcpy(tupleDataPointer, tuple, tupleSize);
	*linePointer = tupleDataPointer - _data;
	header->pdLower += LINE_POINTER_SIZE;
	header->pdUpper -= tupleSize;
}

uint8_t *Page::iterateTuple(size_t tupleIndex) {
	Header *header = (Header*)_data;
	size_t linePointersCount = (header->pdLower - sizeof(Header)) / LINE_POINTER_SIZE;
	if (tupleIndex >= linePointersCount) {
		return nullptr;
	}
	uint16_t *linePointers = (uint16_t*)(_data + sizeof(Header));
	uint16_t linePointer = linePointers[tupleIndex];
	uint16_t nextLinePointer = tupleIndex == 0 ? header->pdSpecial : linePointers[tupleIndex - 1];
	size_t tupleSize = nextLinePointer - linePointer;
	uint8_t *tuple = new uint8_t[tupleSize];
	memcpy(tuple, _data + linePointer, tupleSize);
	return tuple;
}