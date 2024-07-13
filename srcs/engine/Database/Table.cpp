#include "engine/Table.hpp"
#include "engine/Debug.hpp"

using namespace engine;

Table* Table::create(std::string folderPath, std::string name, FilesManager *filesManager) {
	std::string fileName = folderPath + "/" + name + ".table";
	File *file = File::create(fileName, File::READ_FLAG | File::WRITE_FLAG);

	return new Table(file, name, filesManager);
}

Table::Table(std::string fileName, std::string name, FilesManager *filesManager) : name(name), filesManager(filesManager) {
	file = new File(fileName, File::READ_FLAG | File::WRITE_FLAG, false);
	#ifdef DEBUG_DB
		std::cout << "DB | Table " << name << " loaded from '" << fileName << "'" << std::endl;
	#endif

	structure.push_back(new IntTupleElementHandler());
	structure.push_back(new StringTupleElementHandler(255));
}

Table::Table(File *file, std::string name, FilesManager *filesManager) : file(file), name(name), filesManager(filesManager) {
	file->appendEmptyPage();
	#ifdef DEBUG_DB
		std::cout << "DB | Table " << name << " created" << std::endl;
	#endif

	structure.push_back(new IntTupleElementHandler());
	structure.push_back(new StringTupleElementHandler(255));
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
	structure = other.structure;
	filesManager = other.filesManager;
	return *this;
}

Table::~Table() {
	delete file;
	for (ITupleElementHandler *handler : structure) {
		delete handler;
	}
}

void Table::insertTuple(std::vector<void *> tuple) {
	if (tuple.size() != structure.size()) {
		throw std::runtime_error("Tuple size does not match table structure");
	}
	size_t tupleSize = 0;
	for (size_t i = 0; i < tuple.size(); i++) {
		switch (structure[i]->getType()) {
			case ITupleElementHandler::INT: {
				int32_t intElem = *reinterpret_cast<int32_t *>(tuple[i]);
				IntTupleElementHandler *intHandler = dynamic_cast<IntTupleElementHandler *>(structure[i]);
				tupleSize += intHandler->getSerializedSize(intElem);
				break;
			}
			case ITupleElementHandler::STRING: {
				std::string stringElem = *reinterpret_cast<std::string *>(tuple[i]);
				StringTupleElementHandler *stringHandler = dynamic_cast<StringTupleElementHandler *>(structure[i]);
				tupleSize += stringHandler->getSerializedSize(stringElem);
				break;
			}
			default: {
				throw std::runtime_error("Unknown tuple element type");
			}
		}
	}
	size_t pageIndex = file->getOrCreatePageForTuple(tupleSize);
	Page *page = filesManager->getMutablePage(file, pageIndex);
	uint8_t *tupleBuffer = new uint8_t[tupleSize];
	size_t offset = 0;
	for (size_t i = 0; i < tuple.size(); i++) {
		switch (structure[i]->getType()) {
			case ITupleElementHandler::INT: {
				int32_t intElem = *reinterpret_cast<int32_t *>(tuple[i]);
				IntTupleElementHandler *intHandler = dynamic_cast<IntTupleElementHandler *>(structure[i]);
				offset += intHandler->serialize(tupleBuffer + offset, intElem);
				break;
			}
			case ITupleElementHandler::STRING: {
				std::string stringElem = *reinterpret_cast<std::string *>(tuple[i]);
				StringTupleElementHandler *stringHandler = dynamic_cast<StringTupleElementHandler *>(structure[i]);
				offset += stringHandler->serialize(tupleBuffer + offset, stringElem);
				break;
			}
			default: {
				throw std::runtime_error("Unknown tuple element type");
			}
		}
	}
	page->insertTuple(tupleBuffer, tupleSize);
	delete[] tupleBuffer;
}