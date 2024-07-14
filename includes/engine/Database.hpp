#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <map>

#include "engine/Table.hpp"
#include "engine/FilesManager.hpp"

namespace engine {
	class Database {
	public:
		typedef struct {
			std::string name;
			std::string table;
			int32_t index;
			int32_t type;
			int32_t size;
		} PgStructure;
	private:
		std::string folderPath;
		FilesManager *filesManager;
		std::vector<Table*> tables;

		std::vector<PgStructure> loadStructures(Table *pgStructureTable);
	public:
		Database(std::string folderPath);
		Database(const Database& other);

		Database& operator=(const Database& other);

		~Database();

		FilesManager& getFilesManager() { return *filesManager; }

		void createTable(std::string name, std::vector<Database::PgStructure> pgStructures);
		Table* getTable(std::string name);
	};
};