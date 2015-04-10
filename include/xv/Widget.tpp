#include "xv/Widget.hpp"
#include "Image.hpp"

#include <algorithm>
#include <vector>

using namespace xv;

template <class _Tp>
const _Tp Widget<_Tp>::MARGIN = 21;

template <class _Tp>
const cv::Point Widget<_Tp>::OK_POSITION = cv::Point(MARGIN * 2, MARGIN * 3);

template <class _Tp>
const cv::Point Widget<_Tp>::CANCEL_POSITION = cv::Point(MARGIN * 3, MARGIN * 3);

template <class _Tp>
const int Widget<_Tp>::BUTTON_RADIUS = Widget<_Tp>::MARGIN / 2.5;

template <typename _Tp>
const cv::Scalar Widget<_Tp>::FOREGROUND_COLOR(255, 0, 0);

template <typename _Tp>
const cv::Scalar Widget<_Tp>::HIGHLIGHT_COLOR(0, 255, 255);

template <typename _Tp>
const cv::Scalar Widget<_Tp>::AFFIRMATIVE_COLOR(0, 255, 0);

template <typename _Tp>
const cv::Scalar Widget<_Tp>::NEGATIVE_COLOR(0, 0, 255);

template <typename _Tp>
Widget<_Tp>::~Widget()
{
	
}

template <typename _Tp>
_Tp xv::distance<_Tp>(cv::Point_<_Tp> p1, cv::Point_<_Tp> p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return (_Tp)fabs(sqrt(dx*dx + dy*dy));
}

template <typename _Tp>
bool Widget<_Tp>::mouseOverButton(cv::Point_<_Tp> mousePosition,
	cv::Point_<_Tp> buttonPosition)
{
	if (distance<_Tp>(position() + buttonPosition, mousePosition) < BUTTON_RADIUS)
		return true;
	return false;
}

template <class _Tp>
void Widget<_Tp>::onMouseMove(const cv::Point& point)
{
	if (mouseOverButton(point, cv::Point_<_Tp>(OK_POSITION)))
		m_image->setClickMouseCursor();
	else if (mouseOverButton(point, cv::Point_<_Tp>(CANCEL_POSITION)) )
		m_image->setClickMouseCursor();
}

template <class _Tp>
void Widget<_Tp>::onMouseUp(const cv::Point& point)
{
	if (mouseOverButton(point, cv::Point_<_Tp>(OK_POSITION)))
		m_deleted = true;
	else if (mouseOverButton(point, cv::Point_<_Tp>(CANCEL_POSITION)))
		m_deleted = true;
}

template <typename _Tp>
cv::Rect_<_Tp> Widget<_Tp>::getBounds()
{
	return m_bounds;
}

template <typename _Tp>
void Widget<_Tp>::paintButtons(const cv::Mat& image)
{
	cv::Point_<_Tp> pointOK = position() + cv::Point_<_Tp>(OK_POSITION);
	cv::Point_<_Tp> pointCancel = position() + cv::Point_<_Tp>(CANCEL_POSITION);

	float radians = 45 * M_PI / 180;

	//ok button
	cv::circle(
		image,
		pointOK,
		BUTTON_RADIUS,
		Widget::AFFIRMATIVE_COLOR
	);

	cv::Point_<_Tp> offset(0, BUTTON_RADIUS / 2);

	cv::line(
		image,
		offset + cv::Point_<_Tp>(pointOK.x - (int)BUTTON_RADIUS/1.5 * cos(radians), pointOK.y - (int)BUTTON_RADIUS/1.5 * sin(radians)),
		pointOK + offset,
		Widget::AFFIRMATIVE_COLOR
		);

	cv::line(
		image,
		pointOK + offset,
		cv::Point_<_Tp>(pointOK.x + (int)BUTTON_RADIUS * cos(radians), pointOK.y - (int)BUTTON_RADIUS * sin(radians)),
		Widget::AFFIRMATIVE_COLOR
		);


	//cancel button
	cv::line(
		image,
		cv::Point_<_Tp>(pointCancel.x + (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y + (int)BUTTON_RADIUS / 2 * sin(radians)),
		cv::Point_<_Tp>(pointCancel.x - (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y - (int)BUTTON_RADIUS / 2 * sin(radians)),
		Widget::NEGATIVE_COLOR
		);

	cv::line(
		image,
		cv::Point_<_Tp>(pointCancel.x + (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y - (int)BUTTON_RADIUS / 2 * sin(radians)),
		cv::Point_<_Tp>(pointCancel.x - (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y + (int)BUTTON_RADIUS / 2 * sin(radians)),
		Widget::NEGATIVE_COLOR
		);

	cv::circle(
		image,
		pointCancel,
		BUTTON_RADIUS,
		Widget::NEGATIVE_COLOR
		);
}