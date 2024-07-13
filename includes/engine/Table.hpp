#pragma once

#include <string>
#include <iostream>

#include "engine/File.hpp"

/*
	For now, all tables use this format:
	|index: int(32)|data: string(255)|
*/

namespace engine {
	class Table {
	public:
		static Table* create(std::string folderPath, std::string name);

	private:
		File *file;
		std::string name;
	
	public:
		Table(std::string fileName, std::string name);
		Table(File *file, std::string name);
		Table(const Table& other);

		Table& operator=(const Table& other);

		~Table();

		std::string getName() const { return name; };
		File* getFile() const { return file; };
	};
};