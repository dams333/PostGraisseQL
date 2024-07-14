#include "engine/Database.hpp"
#include "engine/Debug.hpp"

using namespace engine;

Database::Database(std::string folderPath) : folderPath(folderPath), filesManager(new FilesManager()) {	
	std::string pgStructurePath = folderPath + "/" + Table::PG_STRUCTURE_TABLE_NAME + ".table";
	if (!std::filesystem::exists(pgStructurePath)) {
		throw std::runtime_error("Database does not exist");
	}
	Table *pgStructure = new Table(pgStructurePath, Table::PG_STRUCTURE_TABLE_NAME, filesManager);
	tables.push_back(pgStructure);
	
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.path().extension() == ".table" && entry.path().stem() != Table::PG_STRUCTURE_TABLE_NAME) {
			tables.push_back(new Table(entry.path().string(), entry.path().stem().string(), filesManager));
		}
	}

	//TODO: Load structures from pg_structure

	#ifdef DEBUG_DB
		std::cout << "DB | Database loaded from '" << folderPath << "'" << std::endl;
	#endif
}

Database::Database(const Database& other) {
	*this = other;
}

Database& Database::operator=(const Database& other) {
	if (this == &other) {
		return *this;
	}

	filesManager = other.filesManager;
	folderPath = other.folderPath;
	tables = other.tables;
	return *this;
}

Database::~Database() {
	delete filesManager;
	for (auto table : tables) {
		delete table;
	}
}

void Database::createTable(std::string name) {
	if (std::find_if(tables.begin(), tables.end(), [&name](Table* table) { return table->getName() == name; }) != tables.end()) {
		throw std::runtime_error("Table already exists");
	}
	tables.push_back(Table::create(folderPath, name, filesManager));
}

Table* Database::getTable(std::string name) {
	auto it = std::find_if(tables.begin(), tables.end(), [&name](Table* table) { return table->getName() == name; });
	if (it == tables.end()) {
		return nullptr;
	}
	return *it;
}