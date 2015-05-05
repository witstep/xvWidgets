#include <wx/dcbuffer.h>
#include "VideoCapture.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace xv;

BEGIN_EVENT_TABLE_TEMPLATE1(Image_, wxPanel,_Tp)
	EVT_PAINT(Image_::paintEvent)
	EVT_SIZE(Image_::sizeEvent)
	EVT_ERASE_BACKGROUND(Image_::onEraseBackground)
	EVT_LEFT_DOWN(Image_::onMouseDown)
	EVT_LEFT_UP(Image_::onMouseUp)
	EVT_MOTION(Image_::onMouseMove)
END_EVENT_TABLE()

template <typename _Tp>
void Image_<_Tp>::onMouseDown(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	cv::Point point = getPixelInterpolation(cv::Point(evtPoint.x, evtPoint.y));
	for (auto &w : m_widgets){
		if (w->contains(point)){
			w->onMouseDown(point);
			return;//click only passed to the topmost widget
		}
	}
}

template <typename _Tp>
void Image_<_Tp>::onMouseUp(wxMouseEvent& evt)
{
	wxPoint evtPoint(evt.GetPosition());
	cv::Point point = getPixelInterpolation(cv::Point(evtPoint.x, evtPoint.y));

	for (std::list<xv::Widget<_Tp>*>::iterator i = m_widgets.begin(); i != m_widgets.end();){
		(*i)->onMouseUp(point);//no need to check for bounds
		if (!(*i)->m_image){
			m_mutex.Lock();//assure the GUI thread is not iterating
			i = m_widgets.erase(i);
			m_mutex.Unlock();
		}
		else
			i++;
	}
}

template <typename _Tp>
void Image_<_Tp>::onMouseMove(wxMouseEvent& evt)
{
	//if (!evt.Dragging()) return;//to-do: handle mouse-up outside of the app

	wxPoint evtPoint(evt.GetPosition());
	cv::Point point = getPixelInterpolation(cv::Point(evtPoint.x, evtPoint.y));

	for (auto &w : m_widgets){
		if (w->contains(point)){
			w->setMouseOver(true);
		} else{
			w->setMouseOver(false);
		}
		//not checking for bounds because the widget may have been dragged away
		w->onMouseMove(point); 
	}

	Refresh();
}

template <typename _Tp>
cv::Point Image_<_Tp>::getPixelInterpolation(cv::Point point)
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

	return cv::Point(x, y);
}

template <typename _Tp>
void Image_<_Tp>::onEraseBackground(wxEraseEvent&)
{
	//avoids flickering
}

template <typename _Tp>
Image_<_Tp>::Image_() : wxPanel()
{
}

template <typename _Tp>
Image_<_Tp>::Image_(wxWindow *parent,
	wxWindowID id,
	const wxPoint &pos,
	const wxSize &size,
	long style,
	const wxString &name
	) :wxPanel(parent,id,pos,size,style,name)
{
	this->SetMinSize(wxSize(20, 20));
	this->SetBackgroundColour(*wxBLACK);
}

template <typename _Tp>
void Image_<_Tp>::operator<<(const cv::Mat& cvMat)
{

}

template <typename _Tp>
void Image_<_Tp>::paintEvent(wxPaintEvent & evt)
{
	m_mutex.Lock();
	wxBufferedPaintDC dc(this, m_bitmap);
	m_mutex.Unlock();
	m_lastPaintTime = std::chrono::steady_clock::now();
}

template <typename _Tp>
void Image_<_Tp>::createBitmap()
{
	m_mutex.Lock();
	//create a blank bitmap
	if (m_cvMat.empty()){
		int w, h;
		GetSize(&w, &h);
		if (w < 1 || h < 1)
			return;
		m_renderMat = cv::Mat(h, w, CV_8UC3, cv::Scalar(0, 0, 0));
	}else{
		m_preProcessCallback(m_cvMat); /// apply any additional pre-defined processing steps
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

template <typename _Tp>
void Image_<_Tp>::sizeEvent(wxSizeEvent& evt){
	createBitmap();
	Refresh();
}

template <typename _Tp>
void Image_<_Tp>::setBestSizeFit()
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
	cv::copyMakeBorder(m_renderMat, m_renderMat, m_vBorder, m_vBorder, m_hBorder, m_hBorder, cv::BORDER_ISOLATED, cv::Scalar(0, 0, 0));
}

template <typename _Tp>
void Image_<_Tp>::Refresh(bool eraseBackground, const wxRect *rect)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastPaintTime).count() < 33)
		return;//no need to render at more than 30 fps
	createBitmap();
	wxPanel::Refresh(eraseBackground);
}