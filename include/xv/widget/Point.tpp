#include "Point.hpp"
using namespace xv;

template <class _Tp>
const std::string Point_<_Tp>::CLASS_NAME("Point");

#pragma region constructors

template <class _Tp>
Point_<_Tp>::Point_() : cv::Point_<_Tp>() { m_undefined = true; };

template <class _Tp>
Point_<_Tp>::Point_(_Tp _x, _Tp _y) : cv::Point_<_Tp>(_x, _y){};

template <class _Tp>
Point_<_Tp>::Point_(const cv::Point_<_Tp> & pt) : cv::Point_<_Tp>(pt){};

template <class _Tp>
Point_<_Tp>::Point_(const cv::Size_<_Tp>& sz) : cv::Point_<_Tp>(pt){};

template <class _Tp>
Point_<_Tp>::Point_(const cv::Vec<_Tp, 2>& v) : m_position(v){};

#pragma endregion constructors

template <class _Tp>
cv::Point_<_Tp> Point_<_Tp>::position()
{
	return cv::Point_<_Tp>(this->x, this->y);
}

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
	position() = point;
	x = point.x;
	y = point.y;
}

template <typename _Tp>
void Point_<_Tp>::paint(const cv::Mat& image)
{
	Widget::paintButtons(image);
	cv::line(
		image,
		position() - cv::Point_<_Tp>(0, MARGIN),
		position() + cv::Point_<_Tp>(0, MARGIN),
		Widget::FOREGROUND_COLOR
		);

	cv::line(
		image,
		position() - cv::Point_<_Tp>(MARGIN, 0),
		position() + cv::Point_<_Tp>(MARGIN, 0),
		Widget::FOREGROUND_COLOR
		);

	cv::circle(
		image,
		position(),
		3,
		Widget::HIGHLIGHT_COLOR
		);

	std::stringstream ss;
	ss << "(" << position().x << "," << position().y << ")";
	cv::putText(image, ss.str(), position() + cv::Point_<_Tp>(-MARGIN * 2, MARGIN * 2), 1, 1, Widget::HIGHLIGHT_COLOR);

#ifdef	_DEBUG
	_Tp marginOffset = MARGIN * 4;
	std::vector<cv::Point> points = {
		position() + cv::Point_<_Tp>(-marginOffset, -marginOffset),
		position() + cv::Point_<_Tp>(-marginOffset, marginOffset),
		position() + cv::Point_<_Tp>(marginOffset, marginOffset),
		position() + cv::Point_<_Tp>(marginOffset, -marginOffset),
	};
	cv::polylines(image, points, true, Widget::FOREGROUND_COLOR, 1);
#endif

	m_bounds = cv::Rect_<_Tp>(
		position() - cv::Point_<_Tp>(MARGIN * 4, MARGIN * 4),
		position() + cv::Point_<_Tp>(MARGIN * 4, MARGIN * 4)
		);
}

template <typename _Tp>
Point_<_Tp> Point_<_Tp>::UNDEFINED = Point_<_Tp>();