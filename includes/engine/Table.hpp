#pragma once

#include <string>
#include <iostream>
#include "engine/File.hpp"

namespace engine {
	class Table {
	private:
		File *file;
		std::string name;
	public:
		Table(std::string fileName, std::string name);
		Table(const Table& other);

		Table& operator=(const Table& other);

		~Table();
	};
};