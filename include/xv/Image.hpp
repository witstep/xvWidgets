#pragma once
/** @defgroup ImageRepresentation Image Representation
* @brief Storage, visualization and conversion of images
*
* Classes in this module are responsible for storing, displaying and converting image data
*/


#include "Widget.hpp"

#ifdef wxUSE_GUI
	#include <wx/image.h>
#endif

namespace xv
{

#ifdef wxUSE_GUI
	typedef wxImage gui_image_t;
#endif

/** @addtogroup ImageRepresentation */
/*@{*/

/** @brief An off-screen image representation native to the selected GUI
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
