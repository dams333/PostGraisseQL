#include "engine/File.hpp"
#include "engine/Debug.hpp"

using namespace engine;

File* File::create(std::string path, uint8_t flags) {
	return new File(path, flags, true);
}

File::File(std::string path, uint8_t flags, bool create) : _path(path), _flags(flags), _fd(-1), _pageCount(0) {
	int openFlags = O_SYNC;
	if (create) {
		openFlags |= O_CREAT | O_TRUNC;
	}
	if (flags & File::READ_FLAG && flags & File::WRITE_FLAG) {
		openFlags |= O_RDWR;
	} else {
		if (flags & READ_FLAG) {
			openFlags |= O_RDONLY;
		} else if (flags & File::WRITE_FLAG) {
			openFlags |= O_WRONLY;
		}
	}

	_fd = open(_path.c_str(), openFlags, S_IRUSR | S_IWUSR);

	if (_fd == -1) {
		throw std::runtime_error("Failed to open file");
	}

	if (create) {
		if (write(_fd, &_pageCount, sizeof(_pageCount)) != sizeof(_pageCount)) {
			throw std::runtime_error("Failed to write page count");
		}
	} else {
		if (flags & File::READ_FLAG) {
			if (read(_fd, &_pageCount, sizeof(_pageCount)) != sizeof(_pageCount)) {
				throw std::runtime_error("Failed to read page count");
			}
		}
	}
}

File::File(std::string path, bool create) : File(path, File::READ_FLAG, create) {}

File::File(const File& file) {
	*this = file;
}

File& File::operator=(const File& file) {
	if (this == &file) {
		return *this;
	}

	if (_fd != -1) {
		close(_fd);
	}

	_path = file._path;
	_flags = file._flags;
	_fd = file._fd;
	_pageCount = file._pageCount;

	return *this;
}

bool File::operator==(const File& file) {
	return _path == file._path;
}

File::~File() {
	if (_fd != -1) {
		if (lseek(_fd, 0, SEEK_SET) == -1) {
			std::cerr << "========================= FATAL ERROR =========================" << std::endl;
			std::cerr << "Cannot delete file" << std::endl;
			std::cerr << "Cannot seek to beginning of file" << std::endl;
			std::cerr << "===============================================================" << std::endl;
		}
		if (write(_fd, &_pageCount, sizeof(_pageCount)) != sizeof(_pageCount)) {
			std::cerr << "========================= FATAL ERROR =========================" << std::endl;
			std::cerr << "Cannot delete file" << std::endl;
			std::cerr << "Cannot write page count" << std::endl;
			std::cerr << "===============================================================" << std::endl;
		}
		close(_fd);
	}
}

void File::_seekToPage(uint32_t page) {
	if (lseek(_fd, sizeof(_pageCount) + page * Page::PAGE_SIZE, SEEK_SET) == -1) {
		throw std::runtime_error("Failed to seek to page");
	}
}

void File::readFromFile(size_t pageIndex, uint8_t* buffer) {
	if (pageIndex >= _pageCount) {
		throw std::runtime_error("Page index out of bounds");
	}
	_seekToPage(pageIndex);
	if (read(_fd, buffer, Page::PAGE_SIZE) != Page::PAGE_SIZE) {
		throw std::runtime_error("Failed to read page");
	}
	#ifdef DEBUG_IO
		std::cout << "IO | Read page " << pageIndex << " from DISK file " << _path << std::endl;
	#endif
}

void File::writeToFile(size_t pageIndex, uint8_t* buffer) {
	if (pageIndex >= _pageCount) {
		throw std::runtime_error("Page index out of bounds");
	}
	_seekToPage(pageIndex);
	if (write(_fd, buffer, Page::PAGE_SIZE) != Page::PAGE_SIZE) {
		throw std::runtime_error("Failed to write page");
	}
	#ifdef DEBUG_IO
		std::cout << "IO | Write page " << pageIndex << " to DISK file " << _path << std::endl;
	#endif
}

void File::appendEmptyPage() {
	_pageCount++;
	if (lseek(_fd, 0, SEEK_END) == -1) {
		throw std::runtime_error("Failed to seek to end of file");
	}
	static const uint8_t *defaultPage = Page::getDefaultPage();
	write(_fd, defaultPage, Page::PAGE_SIZE);
}

size_t File::getOrCreatePageForTuple(size_t tupleSize) {
	for (size_t i = 0; i < _pageCount; i++) {
		Page::Header header;
		_seekToPage(i);
		if (read(_fd, &header, sizeof(header)) != sizeof(header)) {
			throw std::runtime_error("Failed to read page header");
		}
		if (Page::canTupleFitInPage(tupleSize, &header)) {
			return i;
		}
	}
	// TODO: FIX when creating page, it does not works. But at next, when getted, it works
	appendEmptyPage();
	return _pageCount - 1;
}