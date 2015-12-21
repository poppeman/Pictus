cmake_minimum_required (VERSION 2.8)

set(dec_SOURCES
	dec/alpha.c
	dec/buffer.c
	dec/frame.c
	dec/idec.c
	dec/io.c
	dec/quant.c
	dec/tree.c
	dec/vp8.c
	dec/vp8l.c
	dec/webp.c
)

set(dsp_SOURCES
	dsp/alpha_processing.c
	dsp/alpha_processing_sse2.c
	dsp/cpu.c
	dsp/dec.c
	dsp/dec_clip_tables.c
	dsp/dec_mips32.c
	dsp/dec_neon.c
	dsp/dec_sse2.c
	dsp/lossless.c
	dsp/lossless_mips32.c
	dsp/lossless_neon.c
	dsp/lossless_sse2.c
	dsp/upsampling.c
	dsp/upsampling_neon.c
	dsp/upsampling_sse2.c
	dsp/yuv.c
	dsp/yuv_mips32.c
	dsp/yuv_sse2.c
)

set(utils_SOURCES
	utils/bit_reader.c
	utils/bit_writer.c
	utils/color_cache.c
	utils/filters.c
	utils/huffman.c
	utils/huffman_encode.c
	utils/quant_levels.c
	utils/quant_levels_dec.c
	utils/random.c
	utils/rescaler.c
	utils/thread.c
	utils/utils.c
)

add_library(webp STATIC ${dec_SOURCES} ${dsp_SOURCES} ${utils_SOURCES})
target_link_libraries(webp m pthread)
