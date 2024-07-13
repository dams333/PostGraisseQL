#include "engine/FilesManager.hpp"

using namespace engine;

FilesManager::FilesManager(): _bufferPointer(0), _usedBuffers(0) {
	_pageDescriptors = new PageDescriptor[PAGE_COUNT];
	_buffers = new uint8_t*[PAGE_COUNT];
	for (size_t i = 0; i < PAGE_COUNT; i++) {
		_buffers[i] = new uint8_t[Page::PAGE_SIZE];
		_pageDescriptors[i].file = nullptr;
		_pageDescriptors[i].pageIndex = 0;
		_pageDescriptors[i].flags = 0;
	}
}

FilesManager::FilesManager(const FilesManager& filesManager) {
	*this = filesManager;
}

FilesManager& FilesManager::operator=(const FilesManager& filesManager) {
	(void)filesManager;
	throw std::runtime_error("FilesManager::operator= not implemented");
}

FilesManager::~FilesManager() {
	for (size_t i = 0; i < PAGE_COUNT; i++) {
		if (_pageDescriptors[i].flags & PAGE_DIRTY) {
			_pageDescriptors[i].file->writeToFile(_pageDescriptors[i].pageIndex, _buffers[i]);
		}
		delete[] _buffers[i];
	}
	delete[] _buffers;
	delete[] _pageDescriptors;
}

Page *FilesManager::_useBuffer(File *file, size_t pageIndex, uint8_t flags, size_t bufferIndex) {
	if (_pageDescriptors[bufferIndex].flags & PAGE_DIRTY) {
		_pageDescriptors[bufferIndex].file->writeToFile(_pageDescriptors[bufferIndex].pageIndex, _buffers[bufferIndex]);
	}
	_pageDescriptors[bufferIndex].file = file;
	_pageDescriptors[bufferIndex].pageIndex = pageIndex;
	_pageDescriptors[bufferIndex].flags = flags;
	file->readFromFile(pageIndex, _buffers[bufferIndex]);
	return new Page(_buffers[bufferIndex]);
}

Page *FilesManager::_getPage(File *file, size_t pageIndex, uint8_t flags) {
	// Check if page is already in cache
	for (size_t i = 0; i < PAGE_COUNT; i++) {
		if (_pageDescriptors[i].file == file && _pageDescriptors[i].pageIndex == pageIndex) {
			_pageDescriptors[i].flags |= flags;
			return new Page(_buffers[i]);
		}
	}

	// check if there is a non used buffer
	if (_usedBuffers < PAGE_COUNT) {
		return _useBuffer(file, pageIndex, flags, _usedBuffers++);
	}

	// find first non referenced buffer
	while (true) {
		if (!(_pageDescriptors[_bufferPointer].flags & PAGE_REFERENCED)) {
			return _useBuffer(file, pageIndex, flags, _bufferPointer);
		}
		_pageDescriptors[_bufferPointer].flags &= ~PAGE_REFERENCED;
		_bufferPointer = (_bufferPointer + 1) % PAGE_COUNT;
	}
}

Page *FilesManager::getPage(File *file, size_t pageIndex) {
	return _getPage(file, pageIndex, PAGE_REFERENCED);
}

Page *FilesManager::getMutablePage(File *file, size_t pageIndex) {
	return _getPage(file, pageIndex, PAGE_REFERENCED | PAGE_DIRTY);
}