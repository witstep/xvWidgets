#pragma once;
#include <wx/dcbuffer.h>
#include "xv/VideoCapture.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <wx/settings.h>
#include "xv/Widget.hpp"
#include "xv/ImageView.hpp"
using namespace xv;

BEGIN_EVENT_TABLE(ImageView, wxPanel)
	EVT_PAINT(ImageView::paintEvent)
	EVT_SIZE(ImageView::sizeEvent)
	EVT_ERASE_BACKGROUND(ImageView::onEraseBackground)
	EVT_LEFT_DOWN(ImageView::onMouseDown)
	EVT_LEFT_UP(ImageView::onMouseUp)
	EVT_MOTION(ImageView::onMouseMove)
END_EVENT_TABLE()

const cv::Scalar ImageView::PADDING_COLOR(0, 0, 0);

void ImageView::onMouseDown(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	gui_point_t point = getPixelInterpolation(gui_point_t(evtPoint.x, evtPoint.y));
	for (auto &w : m_widgets){
		if (!w->isReadOnly() && w->contains(point)){
			w->onMouseDown(point);
			return;//click only passed to the topmost widget
		}
	}
}

void ImageView::onMouseUp(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	gui_point_t point = getPixelInterpolation(gui_point_t(evtPoint.x, evtPoint.y));

	for (std::list<xv::Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end();){
		(*i)->onMouseUp(point);//no need to check for bounds
		if (!(*i)->m_image){
			m_mutex.Lock();//assure the GUI thread is not iterating
			i = m_widgets.erase(i);
			m_mutex.Unlock();
		}
		else///if an element was erased above, there's no need to advance
			i++;
	}
}

void ImageView::purge()
{
	for (std::list<xv::Widget*>::iterator i = m_widgets.begin(); i != m_widgets.end();){
		if (!(*i)->m_image){
			m_mutex.Lock();//assure the GUI thread is not iterating
			i = m_widgets.erase(i);
			m_mutex.Unlock();
		}
		else///if an element was erased above, there's no need to advance
			i++;///if an element was erased above, there's no need to advance
	}
	render();
}

void ImageView::onMouseMove(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	gui_point_t point = getPixelInterpolation(gui_point_t(evtPoint.x, evtPoint.y));

	for (auto &w : m_widgets){
		if (!w->isReadOnly() && w->contains(point)){
			w->setMouseOver(true);
		} else{
			w->setMouseOver(false);
		}
		//not checking for bounds because the widget may have been dragged away
		w->onMouseMove(point); 
	}

	render(99);//render if not rendered in the last 99 ms
}

gui_point_t ImageView::getPixelInterpolation(gui_point_t point)
{
	int x = point.x / m_scale - m_hBorder / m_scale;
	int y = point.y / m_scale - m_vBorder / m_scale;

	if (x > m_cvMat.cols - 1)
		x = m_cvMat.cols - 1;
	if (x < 0)
		x = 0;

	if (y > m_cvMat.rows - 1)
		y = m_cvMat.rows - 1;
	if (y < 0)
		y = 0;

	return gui_point_t(x, y);
}

void ImageView::onEraseBackground(wxEraseEvent&)
{
	//avoids flickering
}

ImageView::ImageView() : wxPanel()
{
}


ImageView::ImageView(wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) :wxPanel(parent,id,pos,size,style,name)
{
	SetBackgroundColour(*wxBLACK);
}

void ImageView::operator<<(const cv::Mat& cvMat)
{

}

void ImageView::paintEvent(wxPaintEvent & evt)
{
	m_mutex.Lock();
	wxBufferedPaintDC dc(this, m_bitmap);
	m_mutex.Unlock();
	m_lastPaintTime = std::chrono::steady_clock::now();
}

void ImageView::createBitmap()
{
	m_mutex.Lock();
	//create a blank bitmap
	if (m_cvMat.empty()){
		int w, h;
		GetSize(&w, &h);
		if (w < 1 || h < 1)
			return;
		m_renderMat = cv::Mat(h, w, CV_8UC3, PADDING_COLOR);
	}else{
		m_renderMat = m_cvMat.clone();

		if (m_widgets.size() > 0 )
		for (auto &w : m_widgets)
			w->render(m_renderMat);
		cv::cvtColor(m_renderMat, m_renderMat, cv::COLOR_BGR2RGB);
		setBestSizeFit();
	}
	m_image = wxImage(m_renderMat.cols, m_renderMat.rows, (unsigned char*)m_renderMat.data, true);
	m_bitmap = wxBitmap(m_image);
	m_mutex.Unlock();
}

void ImageView::sizeEvent(wxSizeEvent& evt){
	createBitmap();
	Refresh();
}

void ImageView::setBestSizeFit()
{
	int cols, newCols, rows, newRows;
	m_hBorder = 0; m_vBorder = 0; m_scale = 1;
	this->DoGetSize(&cols, &rows);
	if (m_renderMat.cols == cols && m_renderMat.rows == rows)
		return; //no resize required

	float aspectRatio = (float)m_renderMat.cols / m_renderMat.rows;
	float vFactor = (float)rows / m_renderMat.rows;
	float hFactor = (float)cols / m_renderMat.cols;

	if (vFactor > hFactor){
		newCols = cols;
		newRows = cols / aspectRatio;
		m_vBorder = (rows - newRows) / 2;//padding
	} else{
		newCols = rows * aspectRatio;
		newRows = rows;
		m_hBorder = (cols - newCols) / 2;//padding
	}

	m_scale = (float) newCols / m_cvMat.cols;

	if (newCols < 1 || newRows < 1)
		return;//too small to draw

	cv::resize(m_renderMat, m_renderMat, cv::Size(newCols, newRows));
	cv::copyMakeBorder(m_renderMat, m_renderMat, m_vBorder, m_vBorder, m_hBorder, m_hBorder, cv::BORDER_ISOLATED, PADDING_COLOR);
}

void ImageView::Refresh(bool eraseBackground, const wxRect *rect)
{
	wxPanel::Refresh(eraseBackground);

}

void ImageView::render(int ms)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastPaintTime).count() < ms)
		return;//no need to render at more than ~60 fps (every 16ms)
	createBitmap();
	Refresh();
}


/// Simple rendering/display of widget without user input
void ImageView::operator << (Widget &widget){
	if (std::find(this->m_widgets.begin(), this->m_widgets.end(), &widget) == this->m_widgets.end()){
		if (!widget.m_positioned)//center widget with undefined value in the image
			widget.center();
		widget.m_readonly = true;
		this->m_widgets.push_back(&widget);
		widget.m_image = this;
		render();
	}
	else{
		assert(("Widget already added to the Image", false));
	}
};

///widgets for user input
void ImageView::operator >> (Widget &widget){
	if (std::find(this->m_widgets.begin(), this->m_widgets.end(), &widget) == this->m_widgets.end()){
		if (!widget.m_positioned)//center widget with undefined value in the image
			widget.setPosition(gui_point_t(m_cvMat.rows / 2, m_cvMat.cols/2));
		this->m_widgets.push_back(&widget);
		widget.m_image = this;
		render();
	}
	else{
		assert(("Widget already added to the Image", false));
	}
};
