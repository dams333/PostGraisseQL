#include "engine/Page.hpp"

using namespace engine;

Page::Page() : _data(new char[Page::PAGE_SIZE]) {}

Page::Page(const Page& page) {
	*this = page;
}

Page& Page::operator=(const Page& page) {
	if (this == &page) {
		return *this;
	}

	delete[] _data;
	_data = new char[Page::PAGE_SIZE];
	memcpy(_data, page._data, Page::PAGE_SIZE);

	return *this;
}

Page::~Page() {
	delete[] _data;
}

Page::Header Page::getDefaultHeader() {
	Header header;
	header.pdFlags = 0b00000000;
	header.pdLower = sizeof(Header);
	header.pdUpper = Page::PAGE_SIZE;
	header.pdSpecial = Page::PAGE_SIZE;

	return header;
}