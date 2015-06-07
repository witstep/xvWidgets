#include "xv/Point.hpp"
#include <opencv2/imgproc.hpp>
using namespace xv;

const std::string Point::CLASS_NAME("Point");

Point Point::UNDEFINED = Point();

#pragma region constructors

Point::Point() : cv::Point() {
	m_undefined = true;
};

Point::Point(int _x, int _y) : cv::Point(_x, _y)
{
	setPosition(cv::Point(_x, _y));
};

Point::Point(const cv::Point & pt) : cv::Point(pt) {
	setPosition(pt);
};

Point::Point(const cv::Size_<int>& sz) : cv::Point(sz){};

Point::Point(const cv::Vec<int, 2>& v) : cv::Point(v){};

#pragma endregion constructors

void Point::onMouseDown(const cv::Point& point)
{
	m_dragging = true;
}

void Point::onMouseUp(const cv::Point& point)
{
	Widget::onMouseUp(point);
	if (!m_dragging)
		return;
	m_dragging = false;
}

void Point::onMouseMove(const cv::Point& point)
{
	Widget::onMouseMove(point);
	if (!m_dragging)
		return;
	setPosition(point);
}

void Point::paint(const cv::Mat& image)
{
	int crosshairLen = MARGIN * 3;
	cv::line(
		image,
		position() - cv::Point(0, crosshairLen),
		position() + cv::Point(0, crosshairLen),
		Widget::FOREGROUND_COLOR
		);

	cv::line(
		image,
		position() - cv::Point(crosshairLen, 0),
		position() + cv::Point(crosshairLen, 0),
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
	cv::putText(image, ss.str(), position() + cv::Point(-MARGIN * 2, MARGIN * 2), 1, 1, Widget::HIGHLIGHT_COLOR);

	_Tp marginOffset = MARGIN * 4;
	std::vector<cv::Point> points = {
		position() + cv::Point(-marginOffset, -marginOffset),
		position() + cv::Point(-marginOffset, marginOffset),
		position() + cv::Point(marginOffset, marginOffset),
		position() + cv::Point(marginOffset, -marginOffset),
	};
	cv::polylines(image, points, true, Widget::FOREGROUND_COLOR, 1);
#endif

}

void Point::setPosition(cv::Point position)
{
	Widget::setPosition(position);
	x = position.x;
	y = position.y;

	m_contour = {
		position + cv::Point(-MARGIN, -MARGIN),
		position + cv::Point(-MARGIN, +MARGIN),
		position + cv::Point(+MARGIN, +MARGIN),
		position + cv::Point(+MARGIN, -MARGIN),
	};
}