#include "Contour.hpp"
#include "Point.hpp"
#include "../../Image.hpp"

using namespace xv;

template <class _Tp>
Contour_<_Tp>::Contour_(){
	m_undefined = true;

	//a contour must have at least 3 points
	push_back(Point_<_Tp>(100, 100));
	push_back(Point_<_Tp>(100, 150));
	push_back(Point_<_Tp>(350, 350));
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

	//the whole image
	m_bounds = cv::Rect_<_Tp>(
		cv::Point_<_Tp>(0,0),
		cv::Point_<_Tp>(image.cols-1, image.rows-1)
		);
}

template <typename _Tp>
void Contour_<_Tp>::paintAddPointButton(const Point_<_Tp>& pointCancel, const cv::Mat& image)
{
	double radians = 45 * M_PI / 180;
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

template <class _Tp>
cv::Point_<_Tp> Contour_<_Tp>::position()
{
	if (m_undefined && m_image)
		return cv::Point_<_Tp>(100, 100);

	return cv::Point_<_Tp>(0, 0);// this->x, this->y);
}

template <class T>
void Contour_<T>::onMouseDown(const cv::Point& point)
{
	for (auto &p : *this){
		if (p.getBounds().contains(point)){
			p.onMouseDown(point);
			return;//no need to look for other points
		}
	}
}

template <class T>
void Contour_<T>::onMouseUp(const cv::Point& point)
{
	Widget::onMouseUp(point);
	for (auto &p : *this)
		p.onMouseUp(point);//no need to check for bounds?
}

template <class T>
void Contour_<T>::onMouseMove(const cv::Point& point)
{
	for (auto &p : *this){
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