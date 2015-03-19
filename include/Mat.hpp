#pragma once
#include "wx/panel.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "pointPolygonTest.hpp"

namespace cvWidgets{

	class Mat : public wxPanel
	{
	public:
		Mat();
		Mat(wxWindow * parent,
			wxWindowID id = wxID_ANY,
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL,
			const wxString &name = wxPanelNameStr
		);

		double pointPolygonTest(cv::InputArray contour);
		double pointPolygonTest(cv::InputArray contour, cv::Point2f pt);
		double pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist);

		cv::Mat m_cvMat;
		void paintEvent(wxPaintEvent & evt);
		void operator<<(const cv::Mat&);
		DECLARE_EVENT_TABLE()
	private:
		friend void operator>>(cv::VideoCapture&, Mat&);
		
	};

}