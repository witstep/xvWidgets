#pragma once

#include "Widget.hpp"
#include <opencv2/core.hpp>

namespace xv{
	
	/** @brief Widget that represents a point. */

	class Point : public Widget, public cv::Point
	{
	public:

		static const std::string CLASS_NAME; /*!< Class name to be displayed in the GUI.  */

#pragma region constructors
		/// Default constructor.
		Point();

		/// Construct point at x,y coordinates.
		Point(int _x, int _y);

		/// Construct point at same position as pt.
		Point(const cv::Point & pt);

		/// Construct point and ise width and height as x,y coordinates.
		Point(const cv::Size_<int>& sz);

		/// Construct point by finding x,y point in the vector.
		Point(const cv::Vec<int, 2>& v);
#pragma endregion constructors

		static Point UNDEFINED; /*!< Represents a Point_ for which coordinates were not yet set.  */

#pragma region operators
		/// Conversion operator to cv::Point
		operator cv::Point() const{ return m_position; };

		/// Checks if 2 points are equal.
		bool operator == (const Point &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<cv::Point>(*this) == b;
		};

		/// Checks if 2 points are different.
		bool operator != (const Point &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<cv::Point>(*this) != b;
		};
#pragma endregion operators

		/// Move point widget
		virtual void setPosition(cv::Point position);

		/// Overlay the point widget on an image
		void paint(const cv::Mat&);

		/// The user moved the mouse pointer over the widget
		void onMouseMove(const cv::Point&);

		/// The user pressed the left mouse button over the widget
		void onMouseDown(const cv::Point&);

		/// The user released the left mouse button over the widget
		void onMouseUp(const cv::Point&);
	};

}