#include "xv/Point.hpp"
#include <opencv2/imgproc.hpp>
using namespace xv;

const std::string Point::CLASS_NAME("Point");

Point Point::UNDEFINED = Point();

#pragma region constructors

Point::Point() : gui_point_t() {
	m_undefined = true;
};

Point::Point(int _x, int _y) : gui_point_t(_x, _y)
{
	setPosition(*this);
};

Point::Point(const gui_point_t &pt) : gui_point_t(pt) {
	setPosition(pt);
};

#pragma endregion constructors

void Point::onMouseDown(const gui_point_t& point)
{
	m_dragging = true;
}

void Point::onMouseUp(const gui_point_t& point)
{
	Widget::onMouseUp(point);
	if (!m_dragging)
		return;
	m_dragging = false;
}

void Point::onMouseMove(const gui_point_t& point)
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
		Point(position() - Point(0, crosshairLen)),
		Point(position() + Point(0, crosshairLen)),
		Widget::FOREGROUND_COLOR
		);

	cv::line(
		image,
		Point(position() - Point(crosshairLen, 0)),
		Point(position() + Point(crosshairLen, 0)),
		Widget::FOREGROUND_COLOR
		);

	cv::circle(
		image,
		Point(position()),
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

void Point::setPosition(gui_point_t position)
{
	Widget::setPosition(position);

	setX(position.x());
	setY(position.y());

	m_contour = {
		position + gui_point_t(-MARGIN, -MARGIN),
		position + gui_point_t(-MARGIN, +MARGIN),
		position + gui_point_t(+MARGIN, +MARGIN),
		position + gui_point_t(+MARGIN, -MARGIN),
	};
}