#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/dcbuffer.h>

#include "Image.hpp"
#include "../include/VideoCapture.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace xvWidgets;

BEGIN_EVENT_TABLE(Image, wxPanel)
	EVT_PAINT(Image::paintEvent)
	EVT_SIZE(Image::sizeEvent)
END_EVENT_TABLE()

Image::Image() : wxPanel()
{
}

Image::Image(wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) :wxPanel(parent,id,pos,size,style,name)
{
	this->SetBackgroundColour(*wxBLACK);
}

void xvWidgets::operator >> (cv::VideoCapture &videoCapture, xvWidgets::Image &image)
{
	image.m_mutex.Lock();
	videoCapture >> image.m_cvMat;
	image.m_mutex.Unlock();
}

void xvWidgets::operator >> (xvWidgets::VideoCapture &videoCapture, xvWidgets::Image &image)
{
	image.m_mutex.Lock();
	videoCapture.m_videoCapture >> image.m_cvMat;
	image.m_mutex.Unlock();
}

void Image::operator<<(const cv::Mat& cvMat)
{

}

void Image::paintEvent(wxPaintEvent & evt)
{
	if (this->m_cvMat.empty())
		return;

	m_mutex.Lock();
	wxBufferedPaintDC dc(this, m_bitmap);
	m_mutex.Unlock();
}

void Image::createBitmap()
{
	if (m_cvMat.empty())
		return;
	cv::Mat mat;
	m_mutex.Lock();
	cv::cvtColor(m_cvMat, mat, cv::COLOR_BGR2RGB);
	setBestSizeFit(mat);
	
	m_image = wxImage(mat.cols, mat.rows, (unsigned char*)mat.data, true);
	m_bitmap = wxBitmap(m_image);
	m_mutex.Unlock();
}

void Image::sizeEvent(wxSizeEvent& evt){}

void Image::setBestSizeFit(cv::Mat& mat)
{
	int cols, newCols, rows, newRows ,hBorder=0, vBorder=0;
	this->DoGetSize(&cols, &rows);

	if (mat.cols == cols && mat.rows == rows)
		return; //no resize required
	
	float aspectRatio = (float) mat.cols / mat.rows;
	float vFactor = (float) rows / mat.rows;
	float hFactor = (float) cols / mat.cols;

	if (vFactor > hFactor){
		newCols = cols;
		newRows = cols / aspectRatio;
		vBorder = (rows - newRows) / 2;//padding
	} else{
		newCols = rows * aspectRatio;
		newRows = rows;
		hBorder = (cols - newCols) / 2;//padding
	}

	if (newCols < 1 || newRows < 1)
		return;//too small to draw

	cv::resize(mat, mat, cv::Size(newCols, newRows));
	cv::copyMakeBorder(mat, mat, vBorder, vBorder, hBorder, hBorder, cv::BORDER_ISOLATED, cv::Scalar(0, 0, 0));
}

void Image::Refresh(bool eraseBackground, const wxRect *rect)
{
	createBitmap();
	wxWindow::Refresh(eraseBackground);
}