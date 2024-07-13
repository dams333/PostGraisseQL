#include "Page.hpp"

Page::Page(int fd, uint32_t page) {
	data = new uint8_t[PAGE_SIZE];
	if (lseek(fd, sizeof(uint32_t) + page * PAGE_SIZE, SEEK_SET) < 0) {
		throw std::runtime_error("Failed to seek to page");
	}
	if (read(fd, data, PAGE_SIZE) != PAGE_SIZE) {
		throw std::runtime_error("Failed to read page");
	}
}

Page::Page(const Page& other) {
	*this = other;
}

Page Page::operator=(const Page& other) {
	if (this != &other) {
		delete[] data;
		data = new uint8_t[PAGE_SIZE];
		memcpy(data, other.data, PAGE_SIZE);
	}
	return *this;
}

Page::~Page() {
	delete[] data;
}