#include "engine/IntTupleElementHandler.hpp"

using namespace engine;

size_t IntTupleElementHandler::parse(const uint8_t* buffer, int32_t &value) {
	memcpy(&value, buffer, sizeInByte);

	return sizeInByte;
}

size_t IntTupleElementHandler::getSerializedSize(int32_t &value) const {
	(void)value;
	return sizeInByte;
}

size_t IntTupleElementHandler::serialize(uint8_t* buffer, int32_t &value) const {
	memcpy(buffer, &value, sizeInByte);

	return sizeInByte;
}