#pragma once

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

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

	public:
		File(std::string path, bool create = false);
		File(std::string path, uint8_t flags, bool create = false);
		File(const File& file);

		File& operator=(const File& file);

		~File();
	};
};