#include "engine/Database.hpp"
#include "engine/Table.hpp"
#include "engine/Page.hpp"

using namespace engine;

int main() {
	try {
		Database db("database");

		// db.createTable("table1");
		Table* table = db.getTable("table1");
		FilesManager& filesManager = db.getFilesManager();
		Page* page = filesManager.getMutablePage(table->getFile(), 0);
		Page::Header* header = page->header();
		uint8_t* data = page->data() + header->pdLower;
		data[0] = 0x42;
		data[1] = 0x43;
		data[2] = 0x44;
	} catch (const std::exception& e) {
		std::cerr << "================= FATAL =================" << std::endl;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}