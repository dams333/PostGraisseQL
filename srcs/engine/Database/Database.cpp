#include "engine/Database.hpp"
#include "engine/Debug.hpp"

using namespace engine;

std::vector<Database::PgStructure> Database::loadStructures(Table *pgStructureTable) {
	std::vector<Database::PgStructure> pgStructures;
	bool restart = true;
	while (true) {
		std::vector<void *> tuple = pgStructureTable->iterateTuple(restart);
		restart = false;
		if (tuple.empty()) {
			break;
		}
		Database::PgStructure pgStructure;
		pgStructure.name = *(std::string*)tuple[0];
		pgStructure.table = *(std::string*)tuple[1];
		pgStructure.index = *(int32_t*)tuple[2];
		pgStructure.type = *(int32_t*)tuple[3];
		pgStructure.size = *(int32_t*)tuple[4];
		pgStructures.push_back(pgStructure);
		delete (std::string*)tuple[0];
		delete (std::string*)tuple[1];
		delete (int32_t*)tuple[2];
		delete (int32_t*)tuple[3];
		delete (int32_t*)tuple[4];
	}
	return pgStructures;
}

Database::Database(std::string folderPath) : folderPath(folderPath), filesManager(new FilesManager()) {	
	std::string pgStructurePath = folderPath + "/" + Table::PG_STRUCTURE_TABLE_NAME + ".table";
	if (!std::filesystem::exists(pgStructurePath)) {
		throw std::runtime_error("Database does not exist");
	}
	Table *pgStructure = new Table(pgStructurePath, Table::PG_STRUCTURE_TABLE_NAME, filesManager);
	
	for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
		if (entry.path().extension() == ".table" && entry.path().stem() != Table::PG_STRUCTURE_TABLE_NAME) {
			tables.push_back(new Table(entry.path().string(), entry.path().stem().string(), filesManager));
		}
	}

	std::vector<Database::PgStructure> pgStructures = loadStructures(pgStructure);
	std::map<std::string, std::vector<Database::PgStructure>> pgStructuresByTable;

	for (const auto& pgStructure : pgStructures) {
		pgStructuresByTable[pgStructure.table].push_back(pgStructure);
	}

	for (auto table : tables) {
		auto it = pgStructuresByTable.find(table->getName());
		if (it == pgStructuresByTable.end()) {
			throw std::runtime_error("Table '" + table->getName() + "' does not have a pg_structure entry");
		}
		std::vector<Database::PgStructure> pgStructures = it->second;
		std::sort(pgStructures.begin(), pgStructures.end(), [](const Database::PgStructure& a, const Database::PgStructure& b) { return a.index < b.index; });
		for (const auto& pgStructure : pgStructures) {
			switch (pgStructure.type) {
				case ITupleElementHandler::INT:
					table->addFieldHandler(new IntTupleElementHandler());
					break;
				case ITupleElementHandler::STRING:
					table->addFieldHandler(new StringTupleElementHandler(pgStructure.size));
					break;
				default:
					throw std::runtime_error("Unknown type");
			}
		}
		#ifdef DEBUG_DB
			std::cout << "DB | Table '" << table->getName() << "' structured (" << pgStructures.size() << " fields)" << std::endl;
		#endif
	}

	tables.push_back(pgStructure);

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

void Database::createTable(std::string name, std::vector<Database::PgStructure> pgStructures) {
	if (std::find_if(tables.begin(), tables.end(), [&name](Table* table) { return table->getName() == name; }) != tables.end()) {
		throw std::runtime_error("Table already exists");
	}
	Table *table = Table::create(folderPath, name, filesManager);
	Table *pgStructureTable = getTable(Table::PG_STRUCTURE_TABLE_NAME);
	for (const auto& pgStructure : pgStructures) {
		switch (pgStructure.type) {
			case ITupleElementHandler::INT:
				table->addFieldHandler(new IntTupleElementHandler());
				break;
			case ITupleElementHandler::STRING:
				table->addFieldHandler(new StringTupleElementHandler(pgStructure.size));
				break;
			default:
				throw std::runtime_error("Unknown type");
		}
		std::vector<void *> tuple;
		tuple.push_back(new std::string(pgStructure.name));
		tuple.push_back(new std::string(name));
		tuple.push_back(new int32_t(pgStructure.index));
		tuple.push_back(new int32_t(pgStructure.type));
		tuple.push_back(new int32_t(pgStructure.size));
		pgStructureTable->insertTuple(tuple);
	}
	tables.push_back(table);
}

Table* Database::getTable(std::string name) {
	auto it = std::find_if(tables.begin(), tables.end(), [&name](Table* table) { return table->getName() == name; });
	if (it == tables.end()) {
		return nullptr;
	}
	return *it;
}