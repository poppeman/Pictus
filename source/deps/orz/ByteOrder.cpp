#include "ByteOrder.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

namespace Util {
	uint16_t HToNs(uint16_t host) {
		return htons(host);
	}

	uint16_t NToHs(uint16_t network) {
		return ntohs(network);
	}

	uint32_t HToNl(uint32_t host) {
		return htonl(host);
	}

	uint32_t NToHl(uint32_t network) {
		return ntohl(network);
	}
}
