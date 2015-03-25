#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/dcbuffer.h>

#include "../include/Mat.hpp"
#include "../include/VideoCapture.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cvWidgets;

BEGIN_EVENT_TABLE(Mat, wxPanel)
EVT_PAINT(Mat::paintEvent)
END_EVENT_TABLE()


Mat::Mat() : wxPanel()
{
}

Mat::Mat(wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) :wxPanel(parent,id,pos,size,style,name)
{

}

void cvWidgets::operator >> (cv::VideoCapture &videoCapture, cvWidgets::Mat &mat)
{
	videoCapture >> mat.m_cvMat;
}

void cvWidgets::operator >> (cvWidgets::VideoCapture &videoCapture, cvWidgets::Mat &mat)
{
	videoCapture.m_videoCapture >> mat.m_cvMat;
}

void Mat::operator<<(const cv::Mat& cvMat)
{

}

void Mat::paintEvent(wxPaintEvent & evt)
{
	if (this->m_cvMat.empty())
		return;

	wxBufferedPaintDC dc(this, m_bitmap);
}

void Mat::createBitmap()
{
	cv::Mat mat;
	cv::cvtColor(m_cvMat, mat, cv::COLOR_BGR2RGB);
	wxImage m_image = wxImage(mat.cols, mat.rows, (unsigned char*)mat.data, true);
	m_bitmap = wxBitmap(m_image);
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

void Mat::Refresh(bool eraseBackground, const wxRect *rect)
{
	createBitmap();
	wxPanel::Refresh(eraseBackground);
}