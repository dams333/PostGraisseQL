#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include "engine/Table.hpp"

namespace engine {
	class Database {
	private:
		std::string folderPath;
		std::vector<Table*> tables;
	public:
		Database(std::string folderPath);
		Database(const Database& other);

		Database& operator=(const Database& other);

		~Database();

		void createTable(std::string name);
	};
};