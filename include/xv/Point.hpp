#pragma once

#include "Widget.hpp"
#include <opencv2/core.hpp>

namespace xv
{

	/*! \brief Widget that represents a point.
	 *  \ingroup Widgets
	 */

	class Point : public Widget, public gui_point_t
	{
	public:

		static const std::string CLASS_NAME; /*!< Class name to be displayed in the GUI.  */

#pragma region constructors
		/// Default constructor.
		Point();

		/// Construct point at x,y coordinates.
		Point(int _x, int _y);

		/// Construct point at same position as pt.
		Point(const gui_point_t & pt);
#pragma endregion constructors

#if defined(wxUSE_GUI)
		/// Sets the x coordinate of this point.
		void setX(int a) { wxPoint::x = a; };

		/// Sets the y coordinate of this point.
		void setY(int a) { wxPoint::y = a; };
#endif

		static Point UNDEFINED; /*!< Represents a Point_ for which coordinates were not yet set.  */

#pragma region operators
		/// Conversion operator to cv::Point
		operator cv::Point() const{ return cv::Point(m_position.x(),m_position.y()); };
		operator gui_point_t() const{ return gui_point_t(m_position.x(), m_position.y()); };

		/// Checks if 2 points are equal.
		bool operator == (const Point &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<gui_point_t>(*this) == b;
		};

		/// Checks if 2 points are different.
		bool operator != (const Point &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<gui_point_t>(*this) != b;
		};
#pragma endregion operators

		/// Move point widget
		virtual void setPosition(gui_point_t position);

		/// Overlay the point widget on an image
		void paint(const cv::Mat&);

		/// The user moved the mouse pointer over the widget
		void onMouseMove(const gui_point_t&);

		/// The user pressed the left mouse button over the widget
		void onMouseDown(const gui_point_t&);

		/// The user released the left mouse button over the widget
		void onMouseUp(const gui_point_t&);
	};

}