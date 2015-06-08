#include "xv/Contour.hpp"
#include "xv/Point.hpp"
#include <numeric>
#include <opencv2/imgproc.hpp>

using namespace xv;

Contour Contour::UNDEFINED = Contour();

Contour::Contour(){
	m_undefined = true;
}

void Contour::paint(const cv::Mat& image)
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
			Widget::HIGHLIGHT_COLOR,
			Widget::LINE_THICKNESS
			);
	}

	//closing line between last and first point
	cv::line(
		image,
		(*this)[0],
		(*this)[this->size()-1],
		Widget::HIGHLIGHT_COLOR,
		Widget::LINE_THICKNESS
		);

	m_centerPoint.paint(image);
}

void Contour::defineContours()
{
	//widget contour not the xv::Contour itself
	m_contour.clear();
	for (auto p : *this)
		m_contour.push_back(p);
}

void Contour::paintAddPointButton(const Point& point, const cv::Mat& image)
{
	Point addButtonPosition = point + gui_point_t(BUTTON_RADIUS, -BUTTON_RADIUS);

	cv::line(
		image,
		cv::Point(addButtonPosition.x + (int)BUTTON_RADIUS / 2, addButtonPosition.y),
		cv::Point(addButtonPosition.x - (int)BUTTON_RADIUS / 2, addButtonPosition.y),
		Widget::AFFIRMATIVE_COLOR
		);

	cv::line(
		image,
		cv::Point(addButtonPosition.x, addButtonPosition.y - (int)BUTTON_RADIUS / 2),
		cv::Point(addButtonPosition.x , addButtonPosition.y + (int)BUTTON_RADIUS / 2),
		Widget::AFFIRMATIVE_COLOR
		);

	cv::circle(
		image,
		addButtonPosition,
		BUTTON_RADIUS,
		Widget::AFFIRMATIVE_COLOR
		);

	Point delButtonPosition = point + Point(BUTTON_RADIUS, BUTTON_RADIUS);

	cv::line(
		image,
		cv::Point(delButtonPosition.x + (int)BUTTON_RADIUS / 2, delButtonPosition.y),
		cv::Point(delButtonPosition.x - (int)BUTTON_RADIUS / 2, delButtonPosition.y),
		Widget::AFFIRMATIVE_COLOR
		);

	cv::circle(
		image,
		delButtonPosition,
		BUTTON_RADIUS,
		Widget::NEGATIVE_COLOR
		);
}

void Contour::onMouseDown(const gui_point_t& point)
{
	Widget::onMouseDown(point);

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

void Contour::onMouseUp(const gui_point_t& point)
{
	Widget::onMouseUp(point);

	m_centerPoint.onMouseUp(point);
	for (auto &p : *this)
		p.onMouseUp(point);//no need to check for bounds?

	for (std::vector<Point>::iterator i = begin(); i != end();i++){
		if (i->isMouseOverButton(point, gui_point_t(BUTTON_RADIUS, -BUTTON_RADIUS))){
			insert(i + 1, *i + gui_point_t(BUTTON_RADIUS * 2, -BUTTON_RADIUS * 2));
			break;
		}

		if (
			i->isMouseOverButton(point, gui_point_t(BUTTON_RADIUS, BUTTON_RADIUS)) &&
			size() > 3
			){
			erase(i);
			break;
		}
	}

	defineContours();

	gui_point_t middle(getMiddlePoint());
	Widget::setPosition(middle);
	m_centerPoint.setPosition(middle);
}

void Contour::onMouseMove(const gui_point_t& point)
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

void Contour::setMouseOver(bool mouseOver)
{
	Widget::setMouseOver(mouseOver);
	for (auto &p : *this){
		p.setMouseOver(mouseOver);
	}
}

void Contour::setPosition(gui_point_t position)
{
	shiftPosition(m_position - position);
	Widget::setPosition(position);

	if (this->size() == 0)
		setDefaultPoints();

	m_centerPoint.setPosition(m_position);
}

void Contour::setDefaultPoints()
{
	int polygonMargin = MARGIN * 6;
	push_back(m_position + Point(-polygonMargin, -polygonMargin));
	push_back(m_position + Point(-polygonMargin, polygonMargin));
	push_back(m_position + Point(polygonMargin, polygonMargin));
	push_back(m_position + Point(polygonMargin, -polygonMargin));

	gui_point_t middle(getMiddlePoint());
	Widget::setPosition(middle);
	m_centerPoint.setPosition(middle);
	defineContours();
}

gui_point_t Contour::getMiddlePoint()
{
	gui_point_t middle(0, 0);
	for (auto &p : *this)
		middle += gui_point_t(p.x / (int)this->size(), p.y / (int)this->size());
	return middle;
}

void Contour::shiftPosition(gui_point_t shift)
{
	for (auto &p : *this)
		p.setPosition(p.position() - shift);
}

bool Contour::isEqual(const Contour &a, const Contour &b)
{
	for (auto p : a) /// check if all points in contour a also exist in b
		if (std::find(b.begin(), b.end(), p) == b.end())
			return false;
	return true;
}