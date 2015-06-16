#include "xv/Angle.hpp"
#include "xv/Point.hpp"
#include <numeric>

#include <opencv2/imgproc.hpp>

using namespace xv;

Angle Angle::UNDEFINED = Angle();

Angle::Angle()
{
	m_undefined = true;
	setPosition(gui_point_t(0, 0));
	defineContours();
}

Angle::Angle(gui_point_t vertex)
{
	m_undefined = true; ///only vertex is known
	m_vertex = vertex;
	setPosition(gui_point_t(0, 0));
	defineContours();
}

void Angle::paint(const cv::Mat& image)
{
	m_radius = distance(m_vertex, m_pointA);

	m_vertex.paint(image);
	m_pointA.paint(image);
	//m_pointB.paint(image);

	cv::circle(
		image,
		m_vertex,
		m_radius,
		Widget::HIGHLIGHT_COLOR,
		Widget::LINE_THICKNESS
		);

	//closing line between last and first point
	cv::line(
		image,
		m_vertex,
		m_pointA,
		Widget::HIGHLIGHT_COLOR,
		Widget::LINE_THICKNESS
		);

}

void Angle::onMouseDown(const gui_point_t& point)
{
	Widget::onMouseDown(point);

	/// keep vertex for last in case they are cramped
	for (Point *p : { &m_pointA, &m_pointB, &m_vertex }){
		if (p->contains(point)){
			p->onMouseDown(point);
			return;//pass click to the topmost point
		}
	}
}

void Angle::onMouseUp(const gui_point_t& point)
{
	Widget::onMouseUp(point);

	for (Point *p : { &m_pointA, &m_pointB, &m_vertex })
		p->onMouseUp(point);//no need to check for bounds?

	defineContours();
}

void Angle::onMouseMove(const gui_point_t& point)
{
	m_vertex.onMouseMove(point);

	//when the central anchor point is dragged, move the whole widget and return
	if (m_vertex.isDragging()){

		m_pointA.setPosition(m_pointA.position() + (m_vertex - m_position));
		m_pointB.setPosition(m_pointB.position() + (m_vertex - m_position));

		setPosition(m_vertex);
		return;
	}

	for (Point *p : { &m_pointA, &m_pointB, &m_vertex, }){
		p->onMouseMove(point);

		if (p->contains(point)){
			p->setMouseOver(true);
		}else{
			p->setMouseOver(false);
		}
		if (p->isDragging())
			p->onMouseMove(point);
	}


}

void Angle::setMouseOver(bool mouseOver)
{
	Widget::setMouseOver(mouseOver);
	for (Point *p : { &m_pointA, &m_pointB, &m_vertex, } )
		p->setMouseOver(mouseOver);
}

void Angle::defineContours()
{
	/// a square with side length equal to the visible circle diameter
	m_contour.clear();
	m_contour.push_back(m_position + Point(-m_radius, -m_radius));
	m_contour.push_back(m_position + Point(-m_radius, m_radius));
	m_contour.push_back(m_position + Point(m_radius, m_radius));
	m_contour.push_back(m_position + Point(m_radius, -m_radius));
}

void Angle::setPosition(gui_point_t position)
{
	Widget::setPosition(position);
	
	if (m_vertex == Point::UNDEFINED){
		int polygonMargin = MARGIN * 5;
		m_vertex = m_position;
		m_pointA = m_position + Point(0, -polygonMargin);
		m_pointB = m_position + Point(polygonMargin, -polygonMargin);
		defineContours();
	}

	m_vertex.setPosition(m_position);
}

