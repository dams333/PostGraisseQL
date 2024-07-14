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

		// db.createTable("table1");
		Table* table = db.getTable("table1");
		bool restart = true;
		while (true) {
			std::vector<void *> tuple = table->iterateTuple(restart);
			restart = false;
			if (tuple.empty()) {
				break;
			}
			std::cout << "Tuple: ";
			std::cout << *(int*)tuple[0] << " ";
			std::cout << *(std::string*)tuple[1] << " ";
			std::cout << std::endl;
			delete (int*)tuple[0];
			delete (std::string*)tuple[1];
		}
	} catch (const std::exception& e) {
		std::cerr << "================= FATAL =================" << std::endl;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}