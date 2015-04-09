#include "Point.hpp"
using namespace xv::widget;


template <class _Tp>
const wxString Point_<_Tp>::CLASS_NAME("Point");

template <class T>
void Point_<T>::onMouseDown(const cv::Point& point)
{
	m_dragging = true;
}

template <class T>
void Point_<T>::onMouseUp(const cv::Point& point)
{
	Widget::onMouseUp(point);
	if (!m_dragging)
		return;
	m_dragging = false;
}

template <class T>
void Point_<T>::onMouseMove(const cv::Point& point)
{
	Widget::onMouseMove(point);
	if (!m_dragging)
		return;
	m_position = point;
}

template <typename _Tp>
void Point_<_Tp>::paint(const cv::Mat& image)
{
	Widget::paintButtons(image);
	cv::line(
		image,
		m_position - cv::Point_<_Tp>(0, MARGIN),
		m_position + cv::Point_<_Tp>(0, MARGIN),
		Widget::FOREGROUND_COLOR
		);

	cv::line(
		image,
		m_position - cv::Point_<_Tp>(MARGIN, 0),
		m_position + cv::Point_<_Tp>(MARGIN, 0),
		Widget::FOREGROUND_COLOR
		);

	cv::circle(
		image,
		m_position,
		3,
		Widget::HIGHLIGHT_COLOR
		);

	std::stringstream ss;
	ss << "(" << m_position.x << "," << m_position.y << ")";
	cv::putText(image, ss.str(), m_position + cv::Point_<_Tp>(-MARGIN * 2, MARGIN * 2), 1, 1, Widget::HIGHLIGHT_COLOR);

#ifdef	_DEBUG
	_Tp marginOffset = MARGIN * 4;
	std::vector<cv::Point> points = {
		m_position + cv::Point_<_Tp>(-marginOffset, -marginOffset),
		m_position + cv::Point_<_Tp>(-marginOffset, marginOffset),
		m_position + cv::Point_<_Tp>(marginOffset, marginOffset),
		m_position + cv::Point_<_Tp>(marginOffset, -marginOffset),
	};
	cv::polylines(image, points, true, Widget::FOREGROUND_COLOR, 1);
#endif

	m_bounds = cv::Rect_<_Tp>(
		m_position - cv::Point_<_Tp>(MARGIN * 4, MARGIN * 4),
		m_position + cv::Point_<_Tp>(MARGIN * 4, MARGIN * 4)
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
