#ifndef ILLA_CONFIG_H
#define ILLA_CONFIG_H

	/**
	* Enable or disable strict codecs.
	* Enabling this will cause codecs to fail (gracefully) if there's any kind of error in a file. This is useful for
	* codec development and testing.
	* @warning			Not all codecs care about this setting. It will also reduce compatibility.
	*/
//#define ILLA_ENABLE_STRICTCODECS

namespace Img {
	const int MinBrightness = -200;
	const int MaxBrightness = 200;

	const int MinContrast = 1;
	const int MaxContrast = 200;
	const int ContrastStep = 10;

	const int MinGamma = 1;
	const int MaxGamma = 60;
	const int RedrawDelay = 500;

#ifdef _WIN64
	const size_t MaxSurfaceBytes = (size_t)4096 * (size_t)1024 * (size_t)1024;
#else
	const size_t MaxSurfaceBytes = (size_t)1024 * (size_t)1024 * (size_t)1024;
#endif
	const int MaxSurfaceDim = 100000;
};

#endif
