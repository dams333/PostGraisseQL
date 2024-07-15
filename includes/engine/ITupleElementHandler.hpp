#pragma once

#include <string>
#include <cstring>

namespace engine {
	class ITupleElementHandler {
	public:
		typedef enum {
			INT = 0,
			STRING = 1
		} Type;

		virtual Type getType() const = 0;
		virtual ~ITupleElementHandler() = default;
	};
}