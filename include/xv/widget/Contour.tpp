#include "Contour.hpp"
#include "Point.hpp"
#include "../Image.hpp"
#include <numeric>

using namespace xv;

template <class _Tp>
Contour_<_Tp>::Contour_(){
	m_undefined = true;
}

template <class _Tp>
void Contour_<_Tp>::paint(const cv::Mat& image)
{
	//paint each point
	for (auto &p : *this){
		if (p.isMouseOver())
			paintAddPointButton(p, m_image->m_renderMat);//cloning buttons
		p.paint(image);
	}

	//draw lines between each point and the next
	for (size_t i = 0; i < this->size()-1; i++){
		cv::line(
			image,
			(*this)[i],
			(*this)[i+1],
			Widget::HIGHLIGHT_COLOR
			);
	}

	//closing line between last and first point
	cv::line(
		image,
		(*this)[0],
		(*this)[this->size()-1],
		Widget::HIGHLIGHT_COLOR
		);

	m_centerPoint.paint(image);
}

template <class _Tp>
void Contour_<_Tp>::defineContours()
{
	m_contour.clear();
	for (auto p : *this)
		m_contour.push_back(p);
}

template <typename _Tp>
void Contour_<_Tp>::paintAddPointButton(const Point_<_Tp>& point, const cv::Mat& image)
{
	cv::Point_<_Tp> addButtonPosition = point + cv::Point_<_Tp>(BUTTON_RADIUS, -BUTTON_RADIUS);

	cv::line(
		image,
		cv::Point_<_Tp>(addButtonPosition.x + (int)BUTTON_RADIUS / 2, addButtonPosition.y),
		cv::Point_<_Tp>(addButtonPosition.x - (int)BUTTON_RADIUS / 2, addButtonPosition.y),
		Widget::AFFIRMATIVE_COLOR
		);

	cv::line(
		image,
		cv::Point_<_Tp>(addButtonPosition.x, addButtonPosition.y - (int)BUTTON_RADIUS / 2),
		cv::Point_<_Tp>(addButtonPosition.x , addButtonPosition.y + (int)BUTTON_RADIUS / 2),
		Widget::AFFIRMATIVE_COLOR
		);

	cv::circle(
		image,
		addButtonPosition,
		BUTTON_RADIUS,
		Widget::AFFIRMATIVE_COLOR
		);

	cv::Point_<_Tp> delButtonPosition = point + cv::Point_<_Tp>(BUTTON_RADIUS, BUTTON_RADIUS);

	cv::line(
		image,
		cv::Point_<_Tp>(delButtonPosition.x + (int)BUTTON_RADIUS / 2, delButtonPosition.y),
		cv::Point_<_Tp>(delButtonPosition.x - (int)BUTTON_RADIUS / 2, delButtonPosition.y),
		Widget::AFFIRMATIVE_COLOR
		);

	cv::circle(
		image,
		delButtonPosition,
		BUTTON_RADIUS,
		Widget::NEGATIVE_COLOR
		);
}

template <class T>
void Contour_<T>::onMouseDown(const cv::Point& point)
{
	if (m_centerPoint.contains(point)){
		m_centerPoint.onMouseDown(point);
		return;
	}

	for (auto &p : *this){
		if (p.contains(point)){
			p.onMouseDown(point);
			return;//pass click to the topmost point
		}
	}
}

template <class T>
void Contour_<T>::onMouseUp(const cv::Point& point)
{
	Widget::onMouseUp(point);

	m_centerPoint.onMouseUp(point);
	for (auto &p : *this)
		p.onMouseUp(point);//no need to check for bounds?

	cv::Point_<T> middle(0, 0);

	for (auto &p : *this)
		middle += cv::Point_<T>(p.x / this->size(), p.y / this->size());
	
	for (std::vector<Point_<T>>::iterator i = begin(); i != end();i++){
		if (i->isMouseOverButton(point, cv::Point_<T>(BUTTON_RADIUS, -BUTTON_RADIUS))){
			insert(i+1, *i + cv::Point_<T>(BUTTON_RADIUS * 2, -BUTTON_RADIUS * 2));
			break;
		}

		if (
			i->isMouseOverButton(point, cv::Point_<T>(BUTTON_RADIUS, BUTTON_RADIUS)) &&
			size() > 3
			){
			erase(i);
			break;
		}
	}

	defineContours();

	setPosition(middle);
}

template <class T>
void Contour_<T>::onMouseMove(const cv::Point& point)
{
	m_centerPoint.onMouseMove(point);

	//when the central anchor point is dragged, move the whole widget and return
	if (m_centerPoint.isDragging()){
		for (auto &p : *this){
			p.setPosition(p.position()+(m_centerPoint-m_position));
		}
		setPosition(m_centerPoint);
		return;
	}

	for (auto &p : *this){
		p.onMouseMove(point);

		if (p.contains(point)){
			p.setMouseOver(true);
		}else{
			p.setMouseOver(false);
		}
		if (p.isDragging())
			p.onMouseMove(point);
	}


}

template <class _Tp>
void Contour_<_Tp>::setMouseOver(bool mouseOver)
{
	Widget::setMouseOver(mouseOver);
	for (auto &p : *this){
		p.setMouseOver(mouseOver);
	}
}

template <typename _Tp>
void Contour_<_Tp>::setPosition(cv::Point_<_Tp> position)
{

	Widget::setPosition(position);
	if (this->size() == 0){
		int polygonMargin = MARGIN * 10;
		push_back(m_position + Point_<_Tp>(-polygonMargin, -polygonMargin));
		push_back(m_position + Point_<_Tp>(-polygonMargin, polygonMargin));
		push_back(m_position + Point_<_Tp>(polygonMargin, -polygonMargin));
		defineContours();
	}

	m_centerPoint.setPosition(m_position);

}