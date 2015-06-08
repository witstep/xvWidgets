#pragma once

#include <list>
#include <opencv2/core.hpp>
#include "Widget.hpp"
#include "Point.hpp"

namespace xv{

	/** @brief Widget that represents a contour defined by a set of points */
	class Contour : public Widget, public std::vector<Point>
	{
	public:
		/// Represents a Contour_ for which points were not yet set.
		static Contour UNDEFINED; 

#pragma region constructors

		/// Default constructor. Create a Contour in undefined state.
		Contour();

		/// Creates a contour from a list of points.
		//Contour(const std::list<Point>& v) : std::list<Point>(v){};
#pragma endregion constructors

		/// Checks if 2 contours are equal
		bool operator == (const Contour &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			return isEqual(*this, b);
		};

		/// Checks if 2 contours are different
		bool operator != (const Contour &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			//compare as a gui_point_t_
			return !isEqual(*this, b);
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
		static bool isEqual(const Contour&, const Contour&);

		Point m_centerPoint;

		void setDefaultPoints();
		gui_point_t getMiddlePoint();
		void shiftPosition(gui_point_t);
	};

}