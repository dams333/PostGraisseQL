#include "engine/Database.hpp"
#include "engine/Table.hpp"
#include "engine/Page.hpp"

#include "engine/IntTupleElementHandler.hpp"
#include "engine/StringTupleElementHandler.hpp"

#include <vector>
#include <string>
#include <iostream>

using namespace engine;

int main() {
	try {
		Database db("database");

		// std::vector<Database::PgStructure> pgStructures;
		// Database::PgStructure index;
		// index.name = "idx";
		// index.type = ITupleElementHandler::INT;
		// index.index = 0;
		// index.size = 0;
		// pgStructures.push_back(index);

		// Database::PgStructure name;
		// name.name = "name";
		// name.type = ITupleElementHandler::STRING;
		// name.index = 1;
		// name.size = 255;
		// pgStructures.push_back(name);

		// Database::PgStructure age;
		// age.name = "age";
		// age.type = ITupleElementHandler::INT;
		// age.index = 2;
		// age.size = 0;
		// pgStructures.push_back(age);

		// db.createTable("user2", pgStructures);


		Table* table = db.getTable("user2");

		for (int i = 0; i < 250; i++) {
			std::vector<void *> tuple;
			tuple.push_back(new int32_t(i));
			tuple.push_back(new std::string("John Doe"));
			tuple.push_back(new int32_t(25));
			table->insertTuple(tuple);
			delete (int32_t*)tuple[0];
			delete (std::string*)tuple[1];
			delete (int32_t*)tuple[2];
		}


		// Table* table = db.getTable("user2");
		// bool restart = true;
		// while (true) {
		// 	std::vector<void *> tuple = table->iterateTuple(restart);
		// 	restart = false;
		// 	if (tuple.empty()) {
		// 		break;
		// 	}
		// 	std::cout << "idx: " << *(int32_t*)tuple[0] << std::endl;
		// 	std::cout << "name: " << *(std::string*)tuple[1] << std::endl;
		// 	std::cout << "age: " << *(int32_t*)tuple[2] << std::endl << std::endl;
		// 	delete (int32_t*)tuple[0];
		// 	delete (std::string*)tuple[1];
		// 	delete (int32_t*)tuple[2];
		// }

	} catch (const std::exception& e) {
		std::cerr << "================= FATAL =================" << std::endl;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}