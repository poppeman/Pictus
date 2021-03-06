#ifndef ORZ_SHARED_BYTEORDER_H
#define ORZ_SHARED_BYTEORDER_H

#include <cstdint>

namespace Util {
	uint16_t HToNs(uint16_t host);
	uint16_t NToHs(uint16_t networkShort);

	uint32_t HToNl(uint32_t hostShort);
	uint32_t NToHl(uint32_t networkShort);
}

#endif
