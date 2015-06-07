#pragma once //header only mode
#include "xv/ImageView.hpp"
#include "xv/Widget.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <vector>

using namespace xv;

const int Widget::LINE_THICKNESS = 2;

const int Widget::MARGIN = 10;

const int Widget::BUTTON_RADIUS = 11;

const cv::Point Widget::OK_POSITION = cv::Point(BUTTON_RADIUS * 2, MARGIN * 3);

const cv::Point Widget::CANCEL_POSITION = cv::Point(BUTTON_RADIUS * 5, MARGIN * 3);

const cv::Scalar Widget::FOREGROUND_COLOR(255, 0, 0);

const cv::Scalar Widget::HIGHLIGHT_COLOR(0, 255, 255);

const cv::Scalar Widget::AFFIRMATIVE_COLOR(0, 255, 0);

const cv::Scalar Widget::NEGATIVE_COLOR(0, 0, 255);

Widget::~Widget()
{
	
}

int xv::distance(cv::Point p1, cv::Point p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return (int)sqrt(dx*dx + dy*dy);
}

void Widget::setPosition(cv::Point position)
{
	m_position = position;
	m_positioned = true;
}

void Widget::center()
{
	if (m_image)
		setPosition(cv::Point(m_image->m_cvMat.size() / 2));
}

cv::Point Widget::position()
{
	return m_position;
}

bool Widget::isMouseOverButton(cv::Point mousePosition,
	cv::Point buttonPosition)
{
	if (m_readonly)
		return false;

	int d = distance(position() + buttonPosition, mousePosition);
	if (d < BUTTON_RADIUS)
		return true;
	return false;
}

void Widget::onMouseMove(const cv::Point& point)
{
	if (m_readonly)
		return;

	if (isMouseOverButton(point, cv::Point(OK_POSITION)))
		m_image->setClickMouseCursor();
	else if (isMouseOverButton(point, cv::Point(CANCEL_POSITION)))
		m_image->setClickMouseCursor();
}

void Widget::onMouseDown(const cv::Point& point)
{
	m_accepting = false;
	m_canceling = false;
	if (isMouseOverButton(point, cv::Point(OK_POSITION))){
		m_accepting = true;
	}else if (isMouseOverButton(point, cv::Point(CANCEL_POSITION))){
		m_canceling = true;
	}
}

void Widget::onMouseUp(const cv::Point& point)
{
	if (m_readonly)
		return;

	if (isMouseOverButton(point, cv::Point(OK_POSITION)) && m_accepting){
		m_undefined = false;
		m_image = NULL;
	}else if (isMouseOverButton(point, cv::Point(CANCEL_POSITION)) && m_canceling){
		m_undefined = true;
		m_image = NULL;
	}
}

bool Widget::contains(const cv::Point& point)
{
	assert(("Widget contour empty", m_contour.size() > 0));
	if( cv::pointPolygonTest(m_contour, point, true) >= -MARGIN )
		return true;
	return false;
}

void Widget::render(const cv::Mat& image)
{
	if (!m_readonly)
		paintButtons(image);
	paint(image);
}

void Widget::paintButtons(const cv::Mat& image)
{
	cv::Point pointOK = position() + cv::Point(OK_POSITION);
	cv::Point pointCancel = position() + cv::Point(CANCEL_POSITION);

	double radians = 45 * M_PI / 180;

	//ok button
	cv::circle(
		image,
		pointOK,
		BUTTON_RADIUS,
		Widget::AFFIRMATIVE_COLOR
	);

	cv::Point offset(0, BUTTON_RADIUS / 2);

	cv::line(
		image,
		offset + cv::Point(pointOK.x - (int)BUTTON_RADIUS/1.5 * cos(radians), pointOK.y - (int)BUTTON_RADIUS/1.5 * sin(radians)),
		pointOK + offset,
		Widget::AFFIRMATIVE_COLOR
		);

	cv::line(
		image,
		pointOK + offset,
		cv::Point(pointOK.x + (int)BUTTON_RADIUS * cos(radians), pointOK.y - (int)BUTTON_RADIUS * sin(radians)),
		Widget::AFFIRMATIVE_COLOR
		);


	//cancel button
	cv::line(
		image,
		cv::Point(pointCancel.x + (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y + (int)BUTTON_RADIUS / 2 * sin(radians)),
		cv::Point(pointCancel.x - (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y - (int)BUTTON_RADIUS / 2 * sin(radians)),
		Widget::NEGATIVE_COLOR
		);

	cv::line(
		image,
		cv::Point(pointCancel.x + (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y - (int)BUTTON_RADIUS / 2 * sin(radians)),
		cv::Point(pointCancel.x - (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y + (int)BUTTON_RADIUS / 2 * sin(radians)),
		Widget::NEGATIVE_COLOR
		);

	cv::circle(
		image,
		pointCancel,
		BUTTON_RADIUS,
		Widget::NEGATIVE_COLOR
		);
}

void Widget::hide()
{
	m_image = NULL;
}