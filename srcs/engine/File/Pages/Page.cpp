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