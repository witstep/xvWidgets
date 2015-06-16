#pragma once
/*! 
 *
 * \defgroup ImageRepresentation Image Representation
 * \brief Storage, visualization and conversion of images
 *
 * Classes in this module are responsible for storing, displaying and converting image data.
 *
 * To support the different GUI frameworks, it relies on the **QT_GUI_LIB** and **wxUSE_GUI** pre-processor macros.
 *
 * |            | wxWidgets                                                       | Qt
 * | ---------- | --------------------------------------------------------------- | -------------
 * | off-screen | [wxImage](http://docs.wxwidgets.org/trunk/classwx_image.html)   | [QPixmap] (http://doc.qt.io/qt-5/qpixmap.html)
 * | on-screen  | [wxBitmap](http://docs.wxwidgets.org/trunk/classwx_bitmap.html) | [QImage] (http://doc.qt.io/qt-5/qimage.html)
 *
 * Note that although wxImage and QImage share similar names, they play different roles on each GUI framework.
 */

#include "Widget.hpp"
#include "ImageTranslator.hpp"

namespace xv
{

/*!
 * \brief An off-screen image representation native to the selected GUI
 * \ingroup ImageRepresentation
 */
class Image : public Widget
{

public:
	Image();
	virtual ~Image();
private:

	gui_image_t *m_guiImage;

	virtual void render(const cv::Mat&);

	/// Display widget in output mode (without OK/Cancel buttons)
	virtual void paint(const cv::Mat&);

	/// Check if point is inside the widget
	virtual bool contains(const cv::Point&);

	/// The user clicked the left mouse button over the widget
	virtual void onMouseDown(const cv::Point&);

	/// The user moved the mouse pointer over the widget
	virtual void onMouseMove(const cv::Point&);

	/// The user released the left mouse button over the widget
	virtual void onMouseUp(const cv::Point&);
};
/*@}*/
}
