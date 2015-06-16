#pragma once

#include <opencv2/core.hpp>

#ifdef wxUSE_GUI
#include <wx/image.h>
#include <wx/bitmap.h>
#endif

namespace xv{

#ifdef wxUSE_GUI
	typedef wxImage gui_image_t;
	typedef wxBitmap gui_bitmap_t;
#endif

/*!
 * \brief Class used to convert between image formats
 * \ingroup ImageRepresentation
 */
class ImageTranslator
{
public:
	ImageTranslator();
	virtual ~ImageTranslator();

	/// convert cv::Mat to the native GUI class
	static gui_image_t toNativeGui(const cv::Mat&);
};

}