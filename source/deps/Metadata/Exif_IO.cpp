#include "Exif_IO.h"
#include <boost/algorithm/string.hpp>

namespace Metadata {
	namespace Exif {
		uint16_t ReadUint16(IO::FileReader::Ptr ms, ByteOrder o, size_t position)
		{
			// Assumes host with LSB endianness
			ms->Seek(position, IO::SeekMethod::Begin);
			if (o == ByteOrder::Motorola) {
				// Network byte order.
				return IO::ReadNet16(ms);
			}
			return IO::ReadLE16(ms);
		}

		uint16_t ReadUint16(IO::FileReader::Ptr ms, ByteOrder o)
		{
			return ReadUint16(ms, o, ms->Position());
		}

		uint32_t ReadUint32(IO::FileReader::Ptr ms, ByteOrder o)
		{
			// Assumes host with LSB endianness
			if (o == ByteOrder::Motorola) {
				// Network byte order.
				return IO::ReadNet32(ms);
			}
			return IO::ReadLE32(ms);
		}

		uint16_t FirstUint16(uint32_t data, ByteOrder o)
		{
			if (o == ByteOrder::Motorola) {
				data = data >> 16;
			}
			return data & 0xffff;
		}

		uint16_t SecondUint16(uint32_t data, ByteOrder o)
		{
			if (o == ByteOrder::Intel) {
				data = data >> 16;
			}
			return data & 0xffff;
		}

		std::string ReadAscii(IO::FileReader::Ptr ms, size_t offset, size_t components)
		{
			ms->Seek(offset, IO::SeekMethod::Begin);
			std::vector<char> bytes(components + 1);
			ms->ReadFull(&bytes[0], components);
			bytes[components] = 0;

			// Olympus cameras seems to pad various strings with spaces at the end.
			return boost::trim_right_copy(std::string{ &bytes[0] });
		}

		Metadata::Rational ReadRational(IO::FileReader::Ptr ms, ByteOrder o, size_t position)
		{
			// Assumes unsigned rationals
			ms->Seek(position, IO::SeekMethod::Begin);
			auto num = ReadUint32(ms, o);
			auto nom = ReadUint32(ms, o);
			return Metadata::Rational(num, nom);
		}

	}
}