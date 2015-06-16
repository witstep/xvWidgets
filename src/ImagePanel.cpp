#pragma once;
#include <wx/dcbuffer.h>
#include "xv/VideoCapture.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <wx/settings.h>
#include "xv/Widget.hpp"
#include "xv/ImageTranslator.hpp"
#include "xv/ImagePanel.hpp"
using namespace xv;

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::paintEvent)
	EVT_SIZE(ImagePanel::sizeEvent)
	EVT_ERASE_BACKGROUND(ImagePanel::onEraseBackground)
	EVT_LEFT_DOWN(ImagePanel::onMouseDown)
	EVT_LEFT_UP(ImagePanel::onMouseUp)
	EVT_MOTION(ImagePanel::onMouseMove)
END_EVENT_TABLE()

void ImagePanel::onMouseDown(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	gui_point_t point = m_image.getPixelInterpolation(gui_point_t(evtPoint.x, evtPoint.y));
	m_image.onMouseDown(point);

}

void ImagePanel::onMouseUp(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	gui_point_t point = m_image.getPixelInterpolation(gui_point_t(evtPoint.x, evtPoint.y));
	m_image.onMouseUp(point);
}

void ImagePanel::onMouseMove(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	gui_point_t point = m_image.getPixelInterpolation(gui_point_t(evtPoint.x, evtPoint.y));
	m_image.onMouseMove(point);
	render(99);//render if not rendered in the last 99 ms*/
}


void ImagePanel::onEraseBackground(wxEraseEvent&)
{
	//avoids flickering
}

ImagePanel::ImagePanel() : wxPanel()
{
}

ImagePanel::ImagePanel(wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) :wxPanel(parent,id,pos,size,style,name)
{
	SetBackgroundColour(*wxBLACK);
}

void ImagePanel::operator<<(const cv::Mat& cvMat)
{

}

void ImagePanel::paintEvent(wxPaintEvent & evt)
{
	m_mutex.lock();
	wxBufferedPaintDC dc(this, m_image.getBitmap());
	m_mutex.unlock();
	m_lastPaintTime = std::chrono::steady_clock::now();
}

void ImagePanel::createBitmap()
{
	m_mutex.lock();
	int w, h;
	GetSize(&w, &h);
	m_image.createBitmap(w,h);
	m_mutex.unlock();
}

void ImagePanel::sizeEvent(wxSizeEvent& evt){
	createBitmap();
	Refresh();
}

void ImagePanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	wxPanel::Refresh(eraseBackground);

}

void ImagePanel::render(int ms)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastPaintTime).count() < ms)
		return;//no need to render at more than ~60 fps (every 16ms)
	createBitmap();
	Refresh();
}

/// Simple rendering/display of widget without user input
void ImagePanel::operator << (Widget &widget){
	m_image.addChild( widget );
	render();
};

///widgets for user input
void ImagePanel::operator >> (Widget &widget){
	m_image.addChild( widget, false );
	render();
};
