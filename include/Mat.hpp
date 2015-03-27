#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "pointPolygonTest.hpp"

namespace xvWidgets{

	class VideoCapture;//forward declaration

	class Mat : public wxPanel
	{
	public:
		Mat();
		Mat(wxWindow * parent,
			wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxSize(720, 480),
			long style = wxTAB_TRAVERSAL,
			const wxString &name = wxPanelNameStr
		);

		double pointPolygonTest(cv::InputArray contour);
		double pointPolygonTest(cv::InputArray contour, cv::Point2f pt);
		double pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist);

		cv::Mat m_cvMat;
		void paintEvent(wxPaintEvent & evt);

		operator cv::_InputArray() const { return m_cvMat; }
		operator cv::_OutputArray() const { return m_cvMat; }
		void operator<<(const cv::Mat&);

		void Refresh(bool eraseBackground = false, const wxRect *rect = NULL);

		DECLARE_EVENT_TABLE()
	private:
		wxBitmap m_bitmap;
		void createBitmap();
		friend void operator>>(cv::VideoCapture&, Mat&);
		friend void operator>>(VideoCapture&, Mat&);
		
	};

}