#pragma once
/*!
 * \defgroup Rendering Rendering
 * \brief On screen representation of images and video
 */

#if defined(wxUSE_GUI)
	#include <wx/wx.h>
	#include <wx/panel.h>
	#include <wx/sizer.h>
#elif defined(QT_GUI_LIB)
	#include <QtWidgets/QWidget>
#endif

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include <list>
#include <functional>
#include <mutex>

#include "Image.hpp"
#include "Point.hpp"

namespace xv{

#if defined(wxUSE_GUI)
	typedef wxPanel gui_panel_t;
	typedef wxMouseEvent& gui_mouseEvent_t;
#elif defined(QT_GUI_LIB)
	typedef QWidget gui_panel_t;
	typedef QMouseEvent* gui_mouseEvent_t;
#endif

	class VideoCapture;
	class Widget;
	class Image;

/*!
 * \brief Class used to display images and as container of widgets.
 * \ingroup Rendering
 */
class ImagePanel : public gui_panel_t
{
	friend Widget;
public:

	/// Default constructor
	ImagePanel();

#if defined(wxUSE_GUI)
	/// Constructor inherited from wxPanel
	ImagePanel(wxWindow * parent,
		wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL,
		const wxString &name = wxPanelNameStr
	);

	/// Redraw an image optionally erasing background first or a limited area
	void Refresh(bool eraseBackground = false, const wxRect *rect = NULL);
#elif defined(QT_GUI_LIB)
	void Refresh();
#endif

#pragma region operators

	/// Simple rendering/display of widget without user input
	void operator << (Widget &widget);

	///widgets for user input
	void operator >> (Widget &widget);

	/// Conversation operator for using the class as argument in the OpenCV API
	operator cv::_InputOutputArray() const { return m_image.m_cvMat; }

	/// Conversation operator to cv::Mat
	operator cv::Mat() const { return m_image.m_cvMat; }

	/// Conversation operator to cv::Mat reference
	operator cv::Mat&() { return m_image.m_cvMat; }

	/// Display a cv::Mat
	void operator<<(const cv::Mat&);
#pragma endregion operators

	/// The scale ratio of the image in relation to the original data
	float getScale(){ return m_scale; };

private:

#if defined(wxUSE_GUI)
	void paintEvent(wxPaintEvent&);
	void sizeEvent(wxSizeEvent&);
	void onEraseBackground(wxEraseEvent&);
	DECLARE_EVENT_TABLE()
#endif

	//the ms threshold since last rendering avoid processing transformation that are not perceptible
	void render(int ms=16);
	/// use to provide a reference to the original image and the image afer processing
	std::function<void(cv::Mat &)>
		m_preProcessCallback = [](cv::Mat &){},
		m_postProcessCallback = [](cv::Mat &){};

	void onMouseMove(gui_mouseEvent_t evt);
	void onMouseDown(gui_mouseEvent_t evt);
	void onMouseUp(gui_mouseEvent_t evt);

	bool containsWidget(Widget*);

	//std::list<Widget*> m_widgets;
	std::chrono::steady_clock::time_point m_lastPaintTime;
	std::mutex m_mutex;
	Image m_image;
	inline void createBitmap();

	friend void operator>>(cv::VideoCapture &videoCapture, ImagePanel &imagePanel){
		imagePanel.m_mutex.lock();
		videoCapture >> imagePanel.m_image.m_cvMat; // to-do: refactor
		imagePanel.m_mutex.unlock();
		imagePanel.render();
	}

	/// use the xv::Image to display a cv::Mat
	friend void operator<<(ImagePanel &imagePanel, cv::Mat& mat){
		imagePanel.m_mutex.lock();
		imagePanel.m_image.m_cvMat = mat.clone(); // to-do: refactor
		imagePanel.m_mutex.unlock();
		imagePanel.render();
	};

	double m_scale = 1;
	int m_hBorder = 0;
	int m_vBorder=0;

};

}