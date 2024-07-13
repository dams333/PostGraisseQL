#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include "engine/Table.hpp"
#include "engine/FilesManager.hpp"

namespace engine {
	class Database {
	private:
		std::string folderPath;
		FilesManager *filesManager;
		std::vector<Table*> tables;
	public:
		Database(std::string folderPath);
		Database(const Database& other);

		Database& operator=(const Database& other);

		~Database();

		FilesManager& getFilesManager() { return *filesManager; }

		void createTable(std::string name);
		Table* getTable(std::string name);
	};
};