#include "engine/File.hpp"

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

File::~File() {
	if (_fd != -1) {
		close(_fd);
	}
}