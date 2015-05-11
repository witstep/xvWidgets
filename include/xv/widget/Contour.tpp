#include "Contour.hpp"
#include "Point.hpp"
#include "../Image.hpp"
#include <numeric>

using namespace xv;

template <typename _Tp>
Contour_<_Tp> Contour_<_Tp>::UNDEFINED = Contour_<_Tp>();

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
			paintAddPointButton(p, image);//cloning buttons
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
	//widget contour not the xv::Contour itself
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

	cv::Point_<T> middle(getMiddlePoint());
	Widget::setPosition(middle);
	m_centerPoint.setPosition(middle);
}

template <class T>
void Contour_<T>::onMouseMove(const cv::Point& point)
{
	m_centerPoint.onMouseMove(point);

	//when the central anchor point is dragged, move the whole widget and return
	if (m_centerPoint.isDragging()){

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
	shiftPosition(m_position - position);
	Widget::setPosition(position);

	if (this->size() == 0)
		setDefaultPoints();

	m_centerPoint.setPosition(m_position);
}

template <typename _Tp>
void Contour_<_Tp>::setDefaultPoints()
{
	int polygonMargin = MARGIN * 10;
	push_back(m_position + Point_<_Tp>(-polygonMargin, -polygonMargin));
	push_back(m_position + Point_<_Tp>(-polygonMargin, polygonMargin));
	push_back(m_position + Point_<_Tp>(polygonMargin, -polygonMargin));

	cv::Point_<_Tp> middle(getMiddlePoint());
	Widget::setPosition(middle);
	m_centerPoint.setPosition(middle);
	defineContours();
}

template <typename _Tp>
cv::Point_<_Tp> Contour_<_Tp>::getMiddlePoint()
{
	cv::Point_<_Tp> middle(0, 0);
	for (auto &p : *this)
		middle += cv::Point_<_Tp>(p.x / this->size(), p.y / this->size());
	return middle;
}

template <typename _Tp>
void Contour_<_Tp>::shiftPosition(cv::Point_<_Tp> shift)
{
	for (auto &p : *this)
		p.setPosition(p.position() - shift);
}

template <typename _Tp>
bool Contour_<_Tp>::isEqual(const Contour_<_Tp> &a, const Contour_<_Tp> &b)
{
	for (auto p : a) /// check if all points in contour a also exist in b
		if (std::find(b.begin(), b.end(), p) == b.end())
			return false;
	return true;
}