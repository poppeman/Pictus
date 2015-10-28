#ifndef ILLATEST_SHARE_FILTERBUFFER_H
#define ILLATEST_SHARE_FILTERBUFFER_H

#include "illa/filter.h"
#include <memory>

namespace Tst
{
	enum
	{
		DefaultValidSize = 4,
		DefaultSubSize = 2,
		DefaultSourceT = 1,
		DefaultSourceL = 1,
		DefaultTargetL = 2,
		DefaultTargetT = 1,
		ClearDWord = 0x00,
		SourcePadding = 0xdc,
		DestinationPadding = 0xcd,
	};

	typedef std::shared_ptr<uint8_t> ByteArray;

	ByteArray						CreateArray(size_t stride, int rows);
	void							FillArray(ByteArray arrayToFill, size_t bytesPerRowToSet, size_t rows, uint8_t value);

	struct BufferInfo
	{
		Filter::FilterBuffer		filterBuffer;
		ByteArray					contents;
	};

	BufferInfo						ConstructBufferInfo(const Geom::SizeInt& size, uint32_t defaultContents);
	BufferInfo						ConstructBufferInfo(const Geom::SizeInt& size, size_t rowPadding, uint32_t defaultContents, uint8_t paddingContents);
	bool							BuffersEqual( const uint32_t* buf1, const uint32_t* buf2, int size );

	bool							CheckPaddingUntouched(BufferInfo bufferToValidate, uint8_t paddingContents);

	/*class TwoValidBuffersFixture
	{
	public:
		TwoValidBuffersFixture();
		~TwoValidBuffersFixture();


	protected:
		BufferInfo					m_biValid1;
		BufferInfo					m_biValid2;

		Geom::RectInt				m_regionDefault;
		Geom::PointInt				m_positionTargetDefault;
	};*/

	class ReferenceRotationFixture
	{
	public:
		ReferenceRotationFixture();
		~ReferenceRotationFixture();

		void Inplace();

	protected:
		Filter::FilterBuffer		m_bufferSource;
		BufferInfo					m_bufferDestination;

		Geom::RectInt				m_regionDefault;
		Geom::PointInt				m_positionTargetDefault;
	};

	class PaddedBuffersFixture
	{
	public:
		PaddedBuffersFixture();
		~PaddedBuffersFixture();

	protected:
		BufferInfo					m_bufferSource;
		BufferInfo					m_bufferDestination;

		Geom::RectInt				m_regionDefault;
		Geom::PointInt				m_positionTargetDefault;
	};
}

#endif // SHARE_FILTERBUFFER_H
