#ifndef PICTUS_CLIPBOARD_H
#define PICTUS_CLIPBOARD_H

#include <memory>
#include "illa/image.h"

namespace App
{
	bool ImageToClipboard(std::shared_ptr<Img::Image> source, Img::Properties props);
}

#endif
