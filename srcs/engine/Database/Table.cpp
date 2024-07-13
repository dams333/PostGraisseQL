#include "engine/Table.hpp"
#include "engine/Debug.hpp"

using namespace engine;

Table* Table::create(std::string folderPath, std::string name) {
	std::string fileName = folderPath + "/" + name + ".table";
	File *file = File::create(fileName, File::READ_FLAG | File::WRITE_FLAG);

	return new Table(file, name);
}

Table::Table(std::string fileName, std::string name) : name(name) {
	file = new File(fileName, File::READ_FLAG | File::WRITE_FLAG, false);
	#ifdef DEBUG_DB
		std::cout << "DB | Table " << name << " loaded from '" << fileName << "'" << std::endl;
	#endif
}

Table::Table(File *file, std::string name) : file(file), name(name) {
	file->appendEmptyPage();
	#ifdef DEBUG_DB
		std::cout << "DB | Table " << name << " created" << std::endl;
	#endif
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