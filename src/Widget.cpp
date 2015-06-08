#pragma once //header only mode
#include "xv/ImageView.hpp"
#include "xv/Widget.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <vector>

#ifdef wxUSE_GUI
#include <wx/region.h>
#endif

using namespace xv;

const int Widget::LINE_THICKNESS = 2;

const int Widget::MARGIN = 10;

const int Widget::BUTTON_RADIUS = 11;

const gui_point_t Widget::OK_POSITION = gui_point_t(BUTTON_RADIUS * 2, MARGIN * 3);

const gui_point_t Widget::CANCEL_POSITION = gui_point_t(BUTTON_RADIUS * 5, MARGIN * 3);

const cv::Scalar Widget::FOREGROUND_COLOR(255, 0, 0);

const cv::Scalar Widget::HIGHLIGHT_COLOR(0, 255, 255);

const cv::Scalar Widget::AFFIRMATIVE_COLOR(0, 255, 0);

const cv::Scalar Widget::NEGATIVE_COLOR(0, 0, 255);

Widget::~Widget()
{
	
}

int xv::distance(gui_point_t p1, gui_point_t p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return (int)sqrt(dx*dx + dy*dy);
}

void Widget::setPosition(gui_point_t position)
{
	m_position = position;
	m_positioned = true;
}

void Widget::center()
{
	if (m_image)
		setPosition(gui_point_t(m_image->m_cvMat.rows / 2, m_image->m_cvMat.cols/2));
}

gui_point_t Widget::position()
{
	return m_position;
}

bool Widget::isMouseOverButton(gui_point_t mousePosition,
	gui_point_t buttonPosition)
{
	if (m_readonly)
		return false;

	int d = distance(position() + buttonPosition, mousePosition);
	if (d < BUTTON_RADIUS)
		return true;
	return false;
}

void Widget::onMouseMove(const gui_point_t& point)
{
	if (m_readonly)
		return;

	if (isMouseOverButton(point, gui_point_t(OK_POSITION)))
		m_image->setClickMouseCursor();
	else if (isMouseOverButton(point, gui_point_t(CANCEL_POSITION)))
		m_image->setClickMouseCursor();
}

void Widget::onMouseDown(const gui_point_t& point)
{
	m_accepting = false;
	m_canceling = false;
	if (isMouseOverButton(point, gui_point_t(OK_POSITION))){
		m_accepting = true;
	}
	else if (isMouseOverButton(point, gui_point_t(CANCEL_POSITION))){
		m_canceling = true;
	}
}

void Widget::onMouseUp(const gui_point_t& point)
{
	if (m_readonly)
		return;

	if (isMouseOverButton(point, gui_point_t(OK_POSITION)) && m_accepting){
		m_undefined = false;
		m_image = NULL;
	}
	else if (isMouseOverButton(point, gui_point_t(CANCEL_POSITION)) && m_canceling){
		m_undefined = true;
		m_image = NULL;
	}
}

bool Widget::contains(const gui_point_t& point)
{
	assert(("Widget contour empty", m_contour.size() > 0));
#ifdef wxUSE_GUI
	gui_point_t p = point - gui_point_t(MARGIN,MARGIN);
	if (
		wxRegion(m_contour.size(), &m_contour[0]).Contains(p.x,p.y,MARGIN*2,MARGIN*2) != wxOutRegion
	)
#endif
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
	Point pointOK = position() + OK_POSITION;
	Point pointCancel = position() + CANCEL_POSITION;

	double radians = 45 * M_PI / 180;

	//ok button
	cv::circle(
		image,
		pointOK,
		BUTTON_RADIUS,
		Widget::AFFIRMATIVE_COLOR
	);

	Point offset(0, BUTTON_RADIUS / 2);

	cv::line(
		image,
		Point(offset + Point(pointOK.x - (int)BUTTON_RADIUS/1.5 * cos(radians), pointOK.y - (int)BUTTON_RADIUS/1.5 * sin(radians))),
		Point(pointOK + offset),
		Widget::AFFIRMATIVE_COLOR
		);

	cv::line(
		image,
		Point(pointOK + offset),
		Point(pointOK.x + (int)BUTTON_RADIUS * cos(radians), pointOK.y - (int)BUTTON_RADIUS * sin(radians)),
		Widget::AFFIRMATIVE_COLOR
		);


	//cancel button
	cv::line(
		image,
		Point(pointCancel.x + (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y + (int)BUTTON_RADIUS / 2 * sin(radians)),
		Point(pointCancel.x - (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y - (int)BUTTON_RADIUS / 2 * sin(radians)),
		Widget::NEGATIVE_COLOR
		);

	cv::line(
		image,
		Point(pointCancel.x + (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y - (int)BUTTON_RADIUS / 2 * sin(radians)),
		Point(pointCancel.x - (int)BUTTON_RADIUS / 2 * cos(radians), pointCancel.y + (int)BUTTON_RADIUS / 2 * sin(radians)),
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