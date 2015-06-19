#pragma once

#include <list>
#include <opencv2/core.hpp>
#include "Widget.hpp"
#include "Point.hpp"

namespace xv{

/*! 
 * \brief Widget that represents a polygon defined by a set of points 
 * \ingroup Widgets
 */

class Polygon : public Widget, public std::vector<Point>
{
public:
	/// Represents a Polygon for which points were not yet set.
	static Polygon UNDEFINED;

#pragma region constructors

	/// Default constructor. Create a Polygon in undefined state.
	Polygon();

	/// Creates polygon from a list of points.
	//Polygon(const std::list<Point>& v) : std::list<Point>(v){};
#pragma endregion constructors

	/// Checks if 2 Polygons are equal
	bool operator == (const Polygon &b) {
		if (this->m_undefined && b.m_undefined)
			return true;//if both are undefined they are considered equal
		return isEqual(b);
	};

	/// Checks if 2 Polygons are different
	bool operator != (const Polygon &b) {
		if (this->m_undefined && b.m_undefined)
			return false; //if both are undefined they are considered equal

		if (this->m_undefined == !b.m_undefined)
			return true; //opposite undefined state

		//compare as a gui_point_t_
		return !isEqual(b);
	};

	/// Moves the widget
	virtual void setPosition(gui_point_t position);
private:
	void paint(const cv::Mat&);
	void paintAddPointButton(const Point&, const cv::Mat& image);
	void onMouseMove(const gui_point_t&);
	void onMouseDown(const gui_point_t&);
	void onMouseUp(const gui_point_t&);
	void setMouseOver(bool mouseOver);
	void defineContours();
	bool isEqual(const Polygon&);

	Point m_centerPoint;

	void setDefaultPoints();
	gui_point_t getMiddlePoint();
	void shiftPosition(gui_point_t);

	std::mutex m_mutex;
};

}