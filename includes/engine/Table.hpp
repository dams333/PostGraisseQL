#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "engine/File.hpp"
#include "engine/ITupleElementHandler.hpp"
#include "engine/IntTupleElementHandler.hpp"
#include "engine/StringTupleElementHandler.hpp"
#include "engine/FilesManager.hpp"

/*
	For now, all tables use this format:
	|index: int(32)|data: string(255)|
*/

namespace engine {
	class Table {
	public:
		static const std::string PG_STRUCTURE_TABLE_NAME;
		static Table* create(std::string folderPath, std::string name, FilesManager *filesManager);

	private:
		File *file;
		std::string name;
		std::vector<ITupleElementHandler *> structure;
		FilesManager *filesManager;
	
	public:
		Table(std::string fileName, std::string name, FilesManager *filesManager);
		Table(File *file, std::string name, FilesManager *filesManager);
		Table(const Table& other);

		Table& operator=(const Table& other);

		~Table();

		std::string getName() const { return name; };
		File* getFile() const { return file; };

		void addFieldHandler(ITupleElementHandler *handler) { structure.push_back(handler); };

		void insertTuple(std::vector<void *> tuple);
		std::vector<void *> iterateTuple(bool restart);
	};
};