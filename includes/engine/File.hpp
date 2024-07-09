#pragma once

#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

class File {
public:
	const static uint8_t READ_FLAG = 0b00000001;
	const static uint8_t WRITE_FLAG = 0b00000010;

private:
	std::string _path;
	uint8_t _flags;
	int _fd;

public:
	File(std::string path);
	File(std::string path, uint8_t flags);
	File(const File& file);

	File& operator=(const File& file);

	~File();
};
