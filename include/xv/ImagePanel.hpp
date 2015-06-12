#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include <list>
#include <functional>

#include "Point.hpp"

namespace xv{

	class VideoCapture;
	class Widget;
	class Image;

	/** @brief Class used to display images and as container of widgets. */
	class ImagePanel : public wxPanel
	{
		friend Widget;
	public:
		/// the color used to pad the frame when the aspect ratio doesn't fit
		/// the container
		static const cv::Scalar PADDING_COLOR;

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
		operator cv::_InputOutputArray() const { return m_cvMat; }

		/// Conversation operator to cv::Mat
		operator cv::Mat() const { return m_cvMat; }

		/// Conversation operator to cv::Mat reference
		operator cv::Mat&() { return m_cvMat; }

		/// Display a cv::Mat
		void operator<<(const cv::Mat&);
#pragma endregion operators

		/// Redraw an image optionally erasing background first or a limited area
		void Refresh(bool eraseBackground = false, const wxRect *rect = NULL);

		/// The scale ratio of the image in relation to the original data
		float getScale(){ return m_scale; };

		/// Immediatelly remove widgets not contained in the image
		void purge();

	private:

		cv::Mat 
			m_cvMat,
			m_renderMat;
		void paintEvent(wxPaintEvent&);
		void sizeEvent(wxSizeEvent&);
		void onEraseBackground(wxEraseEvent&);

		void render(int ms=16);
		/// use to provide a reference to the original image and the image afer processing
		std::function<void(cv::Mat &)>
			m_preProcessCallback = [](cv::Mat &){},
			m_postProcessCallback = [](cv::Mat &){};

		void setClickMouseCursor(){
			wxSetCursor(wxCURSOR_HAND);
		};
		void onMouseMove(wxMouseEvent& evt);
		void onMouseDown(wxMouseEvent& evt);
		void onMouseUp(wxMouseEvent& evt);
		gui_point_t getPixelInterpolation(gui_point_t);

		bool containsWidget(Widget*);

		std::list<Widget*> m_widgets;
		std::chrono::steady_clock::time_point m_lastPaintTime;
		wxMutex m_mutex;
		wxBitmap m_bitmap;
		wxImage m_image;
		//wxImage m_image;
		wxSizer* m_sizer;
		inline void createBitmap();
		void setBestSizeFit();

		friend void operator>>(cv::VideoCapture &videoCapture, ImagePanel &imagePanel){
			imagePanel.m_mutex.Lock();
			videoCapture >> imagePanel.m_cvMat;
			imagePanel.m_mutex.Unlock();
			imagePanel.render();
		}

		/// use the xv::Image to display a cv::Mat
		friend void operator<<(ImagePanel &imagePanel, cv::Mat& mat){
			imagePanel.m_mutex.Lock();
			imagePanel.m_cvMat = mat.clone();
			imagePanel.m_mutex.Unlock();
			imagePanel.render();
		};

		double m_scale = 1;
		int m_hBorder = 0;
		int m_vBorder=0;

		DECLARE_EVENT_TABLE()
	};

}