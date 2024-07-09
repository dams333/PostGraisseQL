#include "engine/File.hpp"

using namespace engine;

File::File(std::string path, uint8_t flags) : _path(path), _flags(flags), _fd(-1) {
	int openFlags = O_SYNC;
	if (flags & File::READ_FLAG && flags & File::WRITE_FLAG) {
		openFlags |= O_RDWR;
	} else {
		if (flags & READ_FLAG) {
			openFlags |= O_RDONLY;
		} else if (flags & File::WRITE_FLAG) {
			openFlags |= O_WRONLY;
		}
	}

	_fd = open(_path.c_str(), openFlags);

	if (_fd == -1) {
		throw std::runtime_error("Failed to open file");
	}
}

File::File(std::string path) : File(path, File::READ_FLAG) {}

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

	return *this;
}

File::~File() {
	if (_fd != -1) {
		close(_fd);
	}
}