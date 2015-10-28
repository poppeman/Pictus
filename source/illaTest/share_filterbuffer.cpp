#include "share_filterbuffer.h"
#include <UnitTest++/UnitTest++.h>
#include <cstring>

namespace Tst
{
	using namespace Geom;

	static uint32_t c_SourceBuffer[] =
	{
		ClearDWord, ClearDWord, ClearDWord, ClearDWord,
		ClearDWord, 1, 2, ClearDWord,
		ClearDWord, 3, 4, ClearDWord,
		ClearDWord, ClearDWord, ClearDWord, ClearDWord
	};

	bool BuffersEqual( const uint32_t* buf1, const uint32_t* buf2, int size )
	{
		for (int i = 0; i < size; ++i)
			if (buf1[i] != buf2[i]) return false;

		return true;
	}

	Tst::BufferInfo ConstructBufferInfo( const Geom::SizeInt& size, uint32_t defaultContents )
	{
		return ConstructBufferInfo(size, 0, defaultContents, 0);
	}

	BufferInfo ConstructBufferInfo( const Geom::SizeInt& size, size_t rowPadding, uint32_t defaultContents, uint8_t paddingContents )
	{
		BufferInfo info;
		size_t stride = size.Width * 4 + rowPadding;
		info.contents.reset(new uint8_t[stride * size.Height], std::default_delete<uint8_t[]>());
		info.filterBuffer.Construct(size, 4, info.contents.get(), stride);

		uint8_t* scanStartPtr = info.contents.get();

		for (int y = 0; y < size.Height; ++y)
		{
			uint32_t* scanPtr = reinterpret_cast<uint32_t*>(scanStartPtr);

			for (int x = 0; x < size.Width; ++x)
				*(scanPtr + x) = defaultContents;

			memset(scanStartPtr + size.Width * 4, paddingContents, rowPadding);

			scanStartPtr += stride;
		}

		return info;
	}


	bool CheckPaddingUntouched( BufferInfo bufferToValidate, uint8_t paddingContents )
	{
		size_t dataPixels = bufferToValidate.filterBuffer.Dimensions.Width * 4;
		size_t padding = bufferToValidate.filterBuffer.Stride - dataPixels;
		uint8_t* scanStartPtr = bufferToValidate.contents.get() + dataPixels;

		for (int y = 0; y < bufferToValidate.filterBuffer.Dimensions.Height; ++y)
		{
			for (size_t x = 0; x < padding; ++x)
				if (*(scanStartPtr + x) != paddingContents) return false;

			scanStartPtr += bufferToValidate.filterBuffer.Stride;
		}

		return true;
	}

	ReferenceRotationFixture::ReferenceRotationFixture()
		:m_regionDefault(Geom::PointInt(0, 0), Geom::SizeInt(DefaultValidSize, DefaultValidSize))
	{
		m_bufferSource.Construct(Geom::SizeInt(DefaultValidSize, DefaultValidSize), 4, reinterpret_cast<uint8_t*>(c_SourceBuffer), 4 * 4);
		m_bufferDestination = ConstructBufferInfo(Geom::SizeInt(DefaultValidSize, DefaultValidSize), DestinationPadding);
	}

	ReferenceRotationFixture::~ReferenceRotationFixture()
	{

	}

	void ReferenceRotationFixture::Inplace()
	{
		m_bufferSource = m_bufferDestination.filterBuffer;

		for (int y = 0; y < DefaultValidSize; ++y)
			for (int x = 0; x < DefaultValidSize; ++x)
				*reinterpret_cast<uint32_t*>(m_bufferSource.BufferData + 4 * x + y * m_bufferSource.Stride) = c_SourceBuffer[x + y * DefaultValidSize];
	}

	PaddedBuffersFixture::PaddedBuffersFixture()
	{
		m_bufferSource = ConstructBufferInfo(SizeInt(DefaultValidSize, DefaultValidSize), 5, ClearDWord, SourcePadding);
		m_bufferDestination = ConstructBufferInfo(SizeInt(DefaultValidSize, DefaultValidSize), 7, ClearDWord, DestinationPadding);
		m_regionDefault = RectInt(PointInt(0, 0), SizeInt(DefaultValidSize, DefaultValidSize));
	}

	PaddedBuffersFixture::~PaddedBuffersFixture()
	{

	}
}
