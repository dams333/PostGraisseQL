#include "engine/Table.hpp"

using namespace engine;

Table* Table::create(std::string folderPath, std::string name) {
	std::string fileName = folderPath + "/" + name + ".table";
	File *file = File::create(fileName, File::READ_FLAG | File::WRITE_FLAG);

	std::cout << "Table " << name << " created" << std::endl;

	return new Table(file, name);
}

Table::Table(std::string fileName, std::string name) : name(name) {
	file = new File(fileName, File::READ_FLAG | File::WRITE_FLAG, false);
	std::cout << "Table " << name << " loaded from '" << fileName << "'" << std::endl;
}

Table::Table(File *file, std::string name) : file(file), name(name) {}

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