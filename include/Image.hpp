#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace xvWidgets{

	class VideoCapture;//forward declaration

	class Image : public wxPanel
	{
	public:
		Image();
		Image(wxWindow * parent,
			wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxSize(720, 480),
			long style = wxTAB_TRAVERSAL,
			const wxString &name = wxPanelNameStr
		);

		cv::Mat m_cvMat;
		void paintEvent(wxPaintEvent&);
		void sizeEvent(wxSizeEvent&);

#pragma region operators
		operator cv::_InputArray() const { return m_cvMat; }
		operator cv::_OutputArray() const { return m_cvMat; }
		void operator<<(const cv::Mat&);
#pragma endregion operators

		void Refresh(bool eraseBackground = false, const wxRect *rect = NULL);

		DECLARE_EVENT_TABLE()
	private:
		wxMutex m_mutex;
		wxBitmap m_bitmap;
		wxImage m_image;
		inline void createBitmap();
		inline void setBestSizeFit(cv::Mat&);
		friend void operator>>(cv::VideoCapture&, Image&);
		friend void operator>>(VideoCapture&, Image&);
		
	};

	typedef Image Mat;
}