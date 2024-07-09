#include "engine/Database.hpp"

using namespace engine;

Database::Database(std::string folderPath) : folderPath(folderPath) {	
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.path().extension() == ".table") {
			tables.push_back(new Table(entry.path().string(), entry.path().stem().string()));
		}
	}

	std::cout << "Database loaded from '" << folderPath << "'" << std::endl;
}

Database::Database(const Database& other) {
	*this = other;
}

Database& Database::operator=(const Database& other) {
	if (this == &other) {
		return *this;
	}

	folderPath = other.folderPath;
	tables = other.tables;
	return *this;
}

Database::~Database() {
	for (auto table : tables) {
		delete table;
	}
}

void Database::createTable(std::string name) {
	if (std::find_if(tables.begin(), tables.end(), [&name](Table* table) { return table->getName() == name; }) != tables.end()) {
		throw std::runtime_error("Table already exists");
	}
	tables.push_back(Table::create(folderPath, name));
}