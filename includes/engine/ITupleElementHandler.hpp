#pragma once

#include <string>

namespace engine {
	class ITupleElementHandler {
	public:
		typedef enum {
			INT,
			STRING
		} Type;

		virtual Type getType() const = 0;
		virtual ~ITupleElementHandler() = default;
	};
}