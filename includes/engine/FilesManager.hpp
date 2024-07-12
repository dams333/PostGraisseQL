#pragma once

#include "engine/File.hpp"
#include "engine/Page.hpp"

namespace engine {
	class FilesManager {
	private:
		static const size_t PAGE_COUNT = 16;
		static const uint8_t PAGE_REFERENCED = 0b00000001;
		static const uint8_t PAGE_DIRTY = 0b00000010;
		typedef struct {
			File *file;
			size_t pageIndex;
			uint8_t flags;
		} PageDescriptor;

		uint8_t **_buffers;
		PageDescriptor *_pageDescriptors;
		size_t _bufferPointer;
		size_t _usedBuffers;

		Page *_useBuffer(File *file, size_t pageIndex, uint8_t flags, size_t bufferIndex);
		Page *_getPage(File *file, size_t pageIndex, uint8_t flags);

	public:
		FilesManager();
		FilesManager(const FilesManager& filesManager);
		FilesManager& operator=(const FilesManager& filesManager);
		~FilesManager();

		Page *getPage(File *file, size_t pageIndex);
		Page *getMutablePage(File *file, size_t pageIndex);
	};
}