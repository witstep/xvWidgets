#include "Point.hpp"
using namespace xv;

template <class _Tp>
const std::string Point_<_Tp>::CLASS_NAME("Point");

template <typename _Tp>
Point_<_Tp> Point_<_Tp>::UNDEFINED = Point_<_Tp>();

#pragma region constructors

template <class _Tp>
Point_<_Tp>::Point_() : cv::Point_<_Tp>() {
	m_undefined = true;
};

template <class _Tp>
Point_<_Tp>::Point_(_Tp _x, _Tp _y) : cv::Point_<_Tp>(_x, _y)
{
	setPosition(cv::Point_<_Tp>(_x, _y));
};

template <class _Tp>
Point_<_Tp>::Point_(const cv::Point_<_Tp> & pt) : cv::Point_<_Tp>(pt) {
	setPosition(pt);
};

template <class _Tp>
Point_<_Tp>::Point_(const cv::Size_<_Tp>& sz) : cv::Point_<_Tp>(pt){};

template <class _Tp>
Point_<_Tp>::Point_(const cv::Vec<_Tp, 2>& v) : m_position(v){};

#pragma endregion constructors

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
	setPosition(point);
}

template <typename _Tp>
void Point_<_Tp>::paint(const cv::Mat& image)
{
	_Tp crosshairLen = MARGIN * 3;
	cv::line(
		image,
		position() - cv::Point_<_Tp>(0, crosshairLen),
		position() + cv::Point_<_Tp>(0, crosshairLen),
		Widget::FOREGROUND_COLOR
		);

	cv::line(
		image,
		position() - cv::Point_<_Tp>(crosshairLen, 0),
		position() + cv::Point_<_Tp>(crosshairLen, 0),
		Widget::FOREGROUND_COLOR
		);

	cv::circle(
		image,
		position(),
		3,
		Widget::HIGHLIGHT_COLOR
		);


#ifdef	_XDEBUG
	std::stringstream ss;
	ss << "(" << position().x << "," << position().y << ")";
	cv::putText(image, ss.str(), position() + cv::Point_<_Tp>(-MARGIN * 2, MARGIN * 2), 1, 1, Widget::HIGHLIGHT_COLOR);

	_Tp marginOffset = MARGIN * 4;
	std::vector<cv::Point> points = {
		position() + cv::Point_<_Tp>(-marginOffset, -marginOffset),
		position() + cv::Point_<_Tp>(-marginOffset, marginOffset),
		position() + cv::Point_<_Tp>(marginOffset, marginOffset),
		position() + cv::Point_<_Tp>(marginOffset, -marginOffset),
	};
	cv::polylines(image, points, true, Widget::FOREGROUND_COLOR, 1);
#endif

}

template <typename _Tp>
void Point_<_Tp>::setPosition(cv::Point_<_Tp> position)
{
	Widget::setPosition(position);
	x = position.x;
	y = position.y;

	m_contour = {
		position + cv::Point_<_Tp>(-MARGIN, -MARGIN),
		position + cv::Point_<_Tp>(-MARGIN, +MARGIN),
		position + cv::Point_<_Tp>(+MARGIN, +MARGIN),
		position + cv::Point_<_Tp>(+MARGIN, -MARGIN),
	};
}