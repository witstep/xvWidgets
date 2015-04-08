#include "Point.hpp"
using namespace xv::widget;


template <class _Tp>
const wxString Point_<_Tp>::CLASS_NAME("Point");

template <class _Tp>
const _Tp Point_<_Tp>::CROSSHAIR_OFFSET = 20;

template <class T>
void Point_<T>::onMouseDown(const cv::Point& point)
{
	m_dragging = true;
}

template <class T>
void Point_<T>::onMouseUp(const cv::Point&)
{
	if (!m_dragging)
		return;
	m_dragging = false;
}

template <class T>
void Point_<T>::onMouseMove(const cv::Point& point)
{
	if (!m_dragging)
		return;
	::wxSetCursor(wxCursor(wxCURSOR_HAND));
	m_point = point;
}

template <typename _Tp>
void Point_<_Tp>::paint(const cv::Mat& image)
{
	cv::line(
		image,
		m_point - cv::Point_<_Tp>(0, CROSSHAIR_OFFSET),
		m_point + cv::Point_<_Tp>(0, CROSSHAIR_OFFSET),
		Widget::FOREGROUND_COLOR
		);

	cv::line(
		image,
		m_point - cv::Point_<_Tp>(CROSSHAIR_OFFSET, 0),
		m_point + cv::Point_<_Tp>(CROSSHAIR_OFFSET, 0),
		Widget::FOREGROUND_COLOR
		);

	cv::circle(
		image,
		m_point,
		3,
		Widget::HIGHLIGHT_COLOR
		);

	std::stringstream ss;
	ss << "(" << m_point.x << "," << m_point.y << ")";
	cv::putText(image, ss.str(), m_point + cv::Point_<_Tp>(-CROSSHAIR_OFFSET*2, CROSSHAIR_OFFSET * 2), 1, 1, Widget::HIGHLIGHT_COLOR);

#ifdef	_DEBUG
	_Tp marginOffset = CROSSHAIR_OFFSET * 4;
	std::vector<cv::Point> points = {
		m_point + cv::Point_<_Tp>(-marginOffset, -marginOffset),
		m_point + cv::Point_<_Tp>(-marginOffset, marginOffset),
		m_point + cv::Point_<_Tp>(marginOffset, marginOffset),
		m_point + cv::Point_<_Tp>(marginOffset, -marginOffset),
	};
	cv::polylines(image, points, true, Widget::FOREGROUND_COLOR, 1);
#endif

	cv::circle(
		image,
		m_point + cv::Point_<_Tp>(CROSSHAIR_OFFSET * 2, CROSSHAIR_OFFSET * 3),
		CROSSHAIR_OFFSET/2.5,
		Widget::AFFIRMATIVE_COLOR
		);

	cv::circle(
		image,
		m_point + cv::Point_<_Tp>(CROSSHAIR_OFFSET * 3, CROSSHAIR_OFFSET * 3),
		CROSSHAIR_OFFSET/2.5,
		Widget::NEGATIVE_COLOR
		);
}


template <typename _Tp>
cv::Point_<_Tp> Point_<_Tp>::UNDEFINED = cv::Point_<_Tp>(
	std::numeric_limits<_Tp>::lowest(),
	std::numeric_limits<_Tp>::lowest()
);

template <typename _Tp>
void Point_<_Tp>::initWidget()
{
	
}

/*template <typename _Tp>
Point_<_Tp>::~Point_()
{

}*/