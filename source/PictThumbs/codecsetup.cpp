#include "StdAfx.h"
#include "codecsetup.h"
#include "illa/codecmgr.h"
#include "illa/f_pcx.h"
#include "illa/f_tga.h"
#include "illa/f_wbmp.h"
#include "illa/f_psd.h"
#include "illa/f_psp.h"
#include "illa/f_tiff.h"
#include "illa/f_xyz.h"

void CodecManagerSetup(Img::CodecFactoryStore* cfs) {
	// With the exception of JPEG, all built-in thumbnail providers are good.
	//	AddCodecFactory(new Img::FactoryBMP());
	//	AddCodecFactory(new Img::FactoryGIF());
	//	AddCodecFactory(new Img::FactoryJPEG());
	//	AddCodecFactory(new Img::FactoryPNG());
	cfs->ReleaseFactories();
	cfs->AddCodecFactory(new Img::FactoryPCX());
	cfs->AddCodecFactory(new Img::FactoryTGA());
	cfs->AddCodecFactory(new Img::FactoryWBMP());
	cfs->AddCodecFactory(new Img::FactoryPSP());
	// Still somewhat flaky, don't enable until it's awesome
	//cfs->AddCodecFactory(new Img::FactoryTIFF());
	cfs->AddCodecFactory(new Img::FactoryPSD());
	cfs->AddCodecFactory(new Img::FactoryXYZ());
}
