#include "engine/StringTupleElementHandler.hpp"

using namespace engine;

StringTupleElementHandler::StringTupleElementHandler(uint32_t maxLength) {
	this->maxLength = maxLength;
	if (maxLength <= 0xFF) {
		sizeOfLengthInByte = 1;
	} else if (maxLength <= 0xFFFF) {
		sizeOfLengthInByte = 2;
	} else if (maxLength <= 0xFFFFFF) {
		sizeOfLengthInByte = 3;
	} else {
		sizeOfLengthInByte = 4;
	}
}

StringTupleElementHandler::StringTupleElementHandler(StringTupleElementHandler& other) {
	*this = other;
}

StringTupleElementHandler& StringTupleElementHandler::operator=(StringTupleElementHandler& other) {
	maxLength = other.maxLength;
	sizeOfLengthInByte = other.sizeOfLengthInByte;
	return *this;
}

size_t StringTupleElementHandler::parse(const uint8_t* buffer, std::string &value) {
	uint32_t length = 0;
	memcpy(&length, buffer, sizeOfLengthInByte);
	buffer += sizeOfLengthInByte;
	value.assign((char*)buffer, length);
	return sizeOfLengthInByte + length;
}

size_t StringTupleElementHandler::getSerializedSize(std::string &value) const {
	return sizeOfLengthInByte + value.size();
}

size_t StringTupleElementHandler::serialize(uint8_t* buffer, std::string &value) const {
	uint32_t length = value.size();
	memcpy(buffer, &length, sizeOfLengthInByte);
	buffer += sizeOfLengthInByte;
	memcpy(buffer, value.c_str(), length);
	return sizeOfLengthInByte + length;
}