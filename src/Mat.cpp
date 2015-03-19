#include <wx/wxprec.h>
#include <wx/dcbuffer.h>
#include <wx/wx.h>

#include "../include/Mat.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cvWidgets;

BEGIN_EVENT_TABLE(Mat, wxPanel)
EVT_PAINT(Mat::paintEvent)
END_EVENT_TABLE()

Mat::Mat(){}

Mat::Mat(wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) :wxPanel(parent,id,pos,size,style,name)
{

}


void cvWidgets::operator >> (cv::VideoCapture &videoCapture, cvWidgets::Mat &mat){
	videoCapture >> mat.m_cvMat;
	mat.Refresh(false);
	Sleep(33);
}


void Mat::operator<<(const cv::Mat& cvMat)
{

}


void Mat::paintEvent(wxPaintEvent & evt)
{
	if (this->m_cvMat.empty())
		return;

	wxImage image(this->m_cvMat.cols, this->m_cvMat.rows, (unsigned char*)this->m_cvMat.data, true);
	cv::cvtColor(this->m_cvMat, this->m_cvMat, cv::COLOR_BGR2RGB);
	wxBitmap bitmap(image);

	wxBufferedPaintDC dc(this, bitmap);
}

double Mat::pointPolygonTest(cv::InputArray contour){
	return 0.0;
}
double Mat::pointPolygonTest(cv::InputArray contour, cv::Point2f pt){
	return 0.0;
}
double Mat::pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist){
	return 0.0;
}