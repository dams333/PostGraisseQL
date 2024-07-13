#pragma once

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <cstdint>

/*

Generic page format

 * +----------------+---------------------------------+
 * | PageHeader     | linp1 linp2 linp3 ...           |
 * +-----------+----+---------------------------------+
 * | ... linpN |									  |
 * +-----------+--------------------------------------+
 * |		   ^ pdLower							  |
 * |												  |
 * |			 v pdUpper							  |
 * +-------------+------------------------------------+
 * |			 | tupleN ...                         |
 * +-------------+------------------+-----------------+
 * |	   ... tuple3 tuple2 tuple1 | "special space" |
 * +--------------------------------+-----------------+
 *									^ pd_special

  PageHeader = {
	pdFlags: 1 byte,
	pdLower: 2 bytes, //point to end of line pointers AKA start of free space
	pdUpper: 2 bytes, //point to begin of tuples AKA end of free space
	pdSpecial: 2 bytes //point to end of tuples AKA start of special space (use case depend on page type)
  } => 7 bytes

  linePointers = array of 2 bytes pointers to tuples
  linpN = 2 bytes //point to tupleN

*/

namespace engine {
	class Page {
	public:
		const static size_t PAGE_SIZE = 4096;
		typedef struct {
			uint8_t pdFlags;
			uint16_t pdLower;
			uint16_t pdUpper;
			uint16_t pdSpecial;
		} Header;

		static uint8_t *getDefaultPage();

	private:
		uint8_t *_data;

	public:
		Page();
		Page(uint8_t* data);
		Page(const Page& page);

		Page& operator=(const Page& page);

		~Page();

		uint8_t* data();
		Header* header() const { return (Header*)_data; };
	};
};
