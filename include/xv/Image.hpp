#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/sizer.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include <list>
#include <functional>

#include "xv/Widget.hpp"

namespace xv{

	class VideoCapture;//forward declaration

	template <typename _Tp>
	class Image_ : public wxPanel
	{
	public:
		/// the color used to pad the frame when the aspect ratio doesn't fit
		/// the container
		static const cv::Scalar PADDING_COLOR;

		friend Widget<_Tp>;
		Image_();
		Image_(wxWindow * parent,
			wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL,
			const wxString &name = wxPanelNameStr
		);

		cv::Mat m_cvMat, m_renderMat;
		void paintEvent(wxPaintEvent&);
		void sizeEvent(wxSizeEvent&);
		void onEraseBackground(wxEraseEvent&);

#pragma region operators
		operator cv::_InputOutputArray() const { return m_cvMat; }
		operator cv::Mat() const { return m_cvMat; }
		operator cv::Mat&() { return m_cvMat; }
		friend void operator >> (Image_ &image, Widget<_Tp> &widget);
		void operator<<(const cv::Mat&);
#pragma endregion operators

		void Refresh(bool eraseBackground = false, const wxRect *rect = NULL);
		int getScale(){ return m_scale; };
	private:
		void render();
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
		cv::Point getPixelInterpolation(cv::Point);

		bool containsWidget(Widget<_Tp>*);

		std::list<Widget<_Tp>*> m_widgets;
		std::chrono::steady_clock::time_point m_lastPaintTime;
		wxMutex m_mutex;
		wxBitmap m_bitmap;
		wxImage m_image;
		wxSizer* m_sizer;
		inline void createBitmap();
		void setBestSizeFit();

		friend void operator>>(cv::VideoCapture &videoCapture, Image_<_Tp> &image){
			image.m_mutex.Lock();
			videoCapture >> image.m_cvMat;
			image.m_mutex.Unlock();
			image.render();
		}

		friend void operator>>(VideoCapture &videoCapture, Image_<_Tp> &image){
			image.m_mutex.Lock();
			videoCapture.m_videoCapture >> image.m_cvMat;
			image.m_mutex.Unlock();
			image.render();
		};

		/// use the xv::Image to display a cv::Mat
		friend void operator<<(Image_<_Tp> &image,cv::Mat& mat){
			image.m_mutex.Lock();
			image.m_cvMat = mat.clone();
			image.m_mutex.Unlock();
			image.render();
		};

		float m_scale = 1;
		int m_hBorder = 0;
		int m_vBorder=0;

		DECLARE_EVENT_TABLE()
	};

	typedef Image_<int> Image;
}

#include "Image.tpp"