#pragma once

#include "Widget.hpp"

#ifdef wxUSE_GUI
	#include <wx/image.h>
#endif

namespace xv
{

#ifdef wxUSE_GUI
	typedef wxImage gui_image_t;
#endif

/** @brief Class that provides storage for images and conversion between the GUI and vision libraries' internal formats */

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
}
