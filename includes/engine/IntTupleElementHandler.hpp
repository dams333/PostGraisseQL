#pragma once

#include <cstdint>
#include "engine/ITupleElementHandler.hpp"

namespace engine {
	class IntTupleElementHandler : public ITupleElementHandler {
	private:
		static const uint8_t sizeInByte = 4;
	public:
		IntTupleElementHandler() = default;
		~IntTupleElementHandler() = default;

		Type getType() const { return INT; }

		size_t parse(const uint8_t* buffer, int32_t &value);
		size_t getSerializedSize(int32_t &value) const;
		size_t serialize(uint8_t* buffer, int32_t &value) const;
	};
};