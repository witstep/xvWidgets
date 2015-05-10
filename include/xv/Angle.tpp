#include "Angle.hpp"
#include "xv/widget/Point.hpp"
#include "Image.hpp"
#include <numeric>

using namespace xv;

template <class _Tp>
Angle_<_Tp>::Angle_()
{
	m_undefined = true;

	m_points.push_back(&m_vertex);
	m_points.push_back(&m_pointA);
	m_points.push_back(&m_pointB);

	defineContours();
}

template <class _Tp>
Angle_<_Tp>::Angle_(cv::Point_<_Tp> vertex)
{
	m_undefined = true; ///only vertex is known
	m_vertex = vertex;
	m_points.push_back(&m_vertex);
	m_points.push_back(&m_pointA);
	m_points.push_back(&m_pointB);
}

template <class _Tp>
void Angle_<_Tp>::paint(const cv::Mat& image)
{
	m_vertex.paint(image);
	m_pointA.paint(image);
	//m_pointB.paint(image);

	cv::circle(
		image,
		m_vertex,
		distance(m_vertex, m_pointA),
		Widget::HIGHLIGHT_COLOR
		);

	//closing line between last and first point
	cv::line(
		image,
		m_vertex,
		m_pointA,
		Widget::HIGHLIGHT_COLOR
		);

}

template <class T>
void Angle_<T>::onMouseDown(const cv::Point& point)
{
	if (m_vertex.contains(point)){
		m_vertex.onMouseDown(point);
		return;
	}

	for (Point_<T> *p : { &m_vertex, &m_pointA, &m_pointB }){
		if (p->contains(point)){
			p->onMouseDown(point);
			return;//pass click to the topmost point
		}
	}
}

template <class T>
void Angle_<T>::onMouseUp(const cv::Point& point)
{
	Widget::onMouseUp(point);

	for (Point_<T> *p : m_points)
		p->onMouseUp(point);//no need to check for bounds?

	defineContours();
}

template <class T>
void Angle_<T>::onMouseMove(const cv::Point& point)
{
	m_vertex.onMouseMove(point);

	//when the central anchor point is dragged, move the whole widget and return
	if (m_vertex.isDragging()){

		m_pointA.setPosition(m_pointA.position() + (m_vertex - m_position));
		m_pointB.setPosition(m_pointB.position() + (m_vertex - m_position));

		setPosition(m_vertex);
		return;
	}

	for (Point_<T>* p : m_points){
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

template <class _Tp>
void Angle_<_Tp>::setMouseOver(bool mouseOver)
{
	Widget::setMouseOver(mouseOver);
	for (Point_<_Tp> *p : m_points)
		p->setMouseOver(mouseOver);
}

template <class _Tp>
void Angle_<_Tp>::defineContours()
{
	m_contour.clear();
	for (auto p : m_points)
	m_contour.push_back(*p);
}

template <typename _Tp>
void Angle_<_Tp>::setPosition(cv::Point_<_Tp> position)
{
	Widget::setPosition(position);

	
	if (m_vertex == Point_<_Tp>::UNDEFINED){
		int polygonMargin = MARGIN * 10;
		m_vertex = m_position;
		m_pointA = m_position + Point_<_Tp>(0, -polygonMargin);
		m_pointB = m_position + Point_<_Tp>(polygonMargin, -polygonMargin);
	}

	m_vertex.setPosition(m_position);
}