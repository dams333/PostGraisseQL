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
		std::vector<void *> tuple;
		tuple.push_back(new int(42));
		tuple.push_back(new std::string("Hello, world!"));
		table->insertTuple(tuple);
		delete (int*)tuple[0];
		delete (std::string*)tuple[1];
	} catch (const std::exception& e) {
		std::cerr << "================= FATAL =================" << std::endl;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}