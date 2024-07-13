#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include "engine/ITupleElementHandler.hpp"

namespace engine {
	class StringTupleElementHandler : public ITupleElementHandler {
	private:
		uint32_t maxLength;
		uint8_t sizeOfLengthInByte;
	public:
		StringTupleElementHandler(uint32_t maxLength);
		StringTupleElementHandler(StringTupleElementHandler& other);
		StringTupleElementHandler& operator=(StringTupleElementHandler& other);
		~StringTupleElementHandler() = default;

		Type getType() const { return STRING; }

		size_t parse(const uint8_t* buffer, std::string &value);
		size_t getSerializedSize(std::string &value) const;
		size_t serialize(uint8_t* buffer, std::string &value) const;
	};
};