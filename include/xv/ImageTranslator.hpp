#pragma once

#include <opencv2/core.hpp>

#if defined(wxUSE_GUI)
	#include <wx/image.h>
	#include <wx/bitmap.h>
#elif defined(QT_GUI_LIB)
	#include <QImage>
	#include <QPixmap>
#endif

namespace xv{

#if defined(wxUSE_GUI)
	typedef wxImage gui_image_t;
	typedef wxBitmap gui_bitmap_t;
#elif defined(QT_GUI_LIB)
	typedef QPixmap gui_image_t;
	typedef QImage gui_bitmap_t;
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