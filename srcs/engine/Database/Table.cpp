#include "engine/Table.hpp"

using namespace engine;

Table::Table(std::string fileName, std::string name) : name(name) {
	file = new File(fileName);
	std::cout << "Table " << name << " loaded from '" << fileName << "'" << std::endl;
}

Table::Table(const Table& other) {
	*this = other;
}

Table& Table::operator=(const Table& other) {
	if (this == &other) {
		return *this;
	}

	*file = *other.file;
	name = other.name;
	return *this;
}

Table::~Table() {
	delete file;
}