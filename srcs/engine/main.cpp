#include "engine/Database.hpp"

int main() {
	try {
		engine::Database db("database");

		db.createTable("table1");
	} catch (const std::exception& e) {
		std::cerr << "================= FATAL =================" << std::endl;
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}