#include "engine/Database.hpp"
#include "engine/Table.hpp"
#include "engine/Page.hpp"

#include "engine/IntTupleElementHandler.hpp"
#include "engine/StringTupleElementHandler.hpp"

using namespace engine;

int main() {
	try {
		Database db("database");

		// db.createTable("table1");
		Table* table = db.getTable("table1");
		FilesManager& filesManager = db.getFilesManager();
		Page* page = filesManager.getMutablePage(table->getFile(), 0);

		uint8_t tuple[10];
		for (size_t i = 0; i < 10; i++) {
			tuple[i] = i;
		}

		if (!page->canTupleFit(10)) {
			std::cerr << "Tuple can't fit" << std::endl;
			return 1;
		}
		page->insertTuple(tuple, 10);
	} catch (const std::exception& e) {
		std::cerr << "================= FATAL =================" << std::endl;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}