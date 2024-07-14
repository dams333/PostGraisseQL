#pragma once

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "engine/Page.hpp"

/*

File structure

	+---------------------------------------------------+
	| pageCount (4 bytes) | page1 | page2 | ... | pageN |
	+---------------------------------------------------+

*/

namespace engine {
	class File {
	public:
		const static uint8_t READ_FLAG = 0b00000001;
		const static uint8_t WRITE_FLAG = 0b00000010;

		static File* create(std::string path, uint8_t flags);

	private:
		std::string _path;
		uint8_t _flags;
		int _fd;
		uint32_t _pageCount;

		void _seekToPage(uint32_t page);

	public:
		File(std::string path, bool create = false);
		File(std::string path, uint8_t flags, bool create = false);
		File(const File& file);

		File& operator=(const File& file);
		bool operator==(const File& file);

		~File();

		std::string getPath() const { return _path; }
		uint32_t getPageCount() const { return _pageCount; }

		void readFromFile(size_t pageIndex, uint8_t* buffer);
		void writeToFile(size_t pageIndex, uint8_t* buffer);

		void appendEmptyPage();
		
		size_t getOrCreatePageForTuple(size_t tupleSize);
	};
};