#pragma once
/*!
 * \defgroup Rendering Rendering
 * \brief On screen representation of images and video
 */

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include <list>
#include <functional>
#include <mutex>

#include "Image.hpp"
#include "Point.hpp"

namespace xv{

	class VideoCapture;
	class Widget;
	class Image;

/*!
 * \brief Class used to display images and as container of widgets.
 * \ingroup Rendering
 */
class ImagePanel : public wxPanel
{
	friend Widget;
public:

	/// Default constructor
	ImagePanel();

	/// Constructor inherited from wxPanel
	ImagePanel(wxWindow * parent,
		wxWindowID id = wxID_ANY,
		const wxPoint &pos = wxDefaultPosition,
		const wxSize &size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL,
		const wxString &name = wxPanelNameStr
	);


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

	/// Redraw an image optionally erasing background first or a limited area
	void Refresh(bool eraseBackground = false, const wxRect *rect = NULL);

	/// The scale ratio of the image in relation to the original data
	float getScale(){ return m_scale; };

private:

#if defined(wxUSE_GUI)
	void paintEvent(wxPaintEvent&);
	void sizeEvent(wxSizeEvent&);
	void onEraseBackground(wxEraseEvent&);
#endif

	//the ms threshold since last rendering avoid processing transformation that are not perceptible
	void render(int ms=16);
	/// use to provide a reference to the original image and the image afer processing
	std::function<void(cv::Mat &)>
		m_preProcessCallback = [](cv::Mat &){},
		m_postProcessCallback = [](cv::Mat &){};

	void onMouseMove(wxMouseEvent& evt);
	void onMouseDown(wxMouseEvent& evt);
	void onMouseUp(wxMouseEvent& evt);

	bool containsWidget(Widget*);

	//std::list<Widget*> m_widgets;
	std::chrono::steady_clock::time_point m_lastPaintTime;
	std::mutex m_mutex;
	Image m_image;
	wxSizer* m_sizer;
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

	DECLARE_EVENT_TABLE()
};

}