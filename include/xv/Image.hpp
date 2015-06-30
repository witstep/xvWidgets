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
 * | off-screen | [wxImage](http://docs.wxwidgets.org/trunk/classwx_image.html)   | [QPixmap](http://doc.qt.io/qt-5/qpixmap.html)
 * | on-screen  | [wxBitmap](http://docs.wxwidgets.org/trunk/classwx_bitmap.html) | [QImage](http://doc.qt.io/qt-5/qimage.html)
 *
 * Note that although wxImage and QImage share similar names, they play different roles in each GUI framework.
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
	friend ImagePanel;
public:
	/// the color used to pad the frame when the aspect ratio doesn't fit
	/// the container
	static const cv::Scalar PADDING_COLOR;

	Image();
	virtual ~Image();

	///load data into the image container
	inline void setImage(gui_image_t data){ m_guiImage = data; };

	/// returns stored image data
	inline gui_image_t getImage(){ return m_guiImage; };
	inline gui_bitmap_t& getBitmap(){ return m_bitmap; };

	/// Fits the image to a new size using black borders to mantain aspect ratio
	void setBestSizeFit(int w, int h);

	/// Maps a pixel position in the original image to the transformed image,
	/// taking scale and padding into consideration
	gui_point_t Image::getPixelInterpolation(gui_point_t point);

	/// Makes the stored image ready for display
	void createBitmap(int w, int h);

	/// to-do: refactor
	cv::Mat
		m_cvMat,     //the original image
		m_renderMat; //the image transformed for display*/
private:
	/// For thread safe updating of image data and handling of children list
	std::mutex m_mutex;

	/// The off-screen image container
	gui_image_t m_guiImage;

	/// The on-screen image container
	gui_bitmap_t m_bitmap;

	virtual void render(const cv::Mat&);

	/// Display widget in output mode (without OK/Cancel buttons)
	virtual void paint(const cv::Mat&);

	/// Check if point is inside the widget
	virtual bool contains(const gui_point_t&);

	/// horizontal padding used to mantain aspect ratio
	int m_hBorder = 0;

	/// vertical padding used to mantain aspect ratio
	int m_vBorder = 0;

	/// Scale of the display image in relation to original
	double m_scale = 1;

};

}
