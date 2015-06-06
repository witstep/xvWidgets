#pragma once

#include "Widget.hpp"
#include <opencv2/core.hpp>

namespace xv{
	
	/** @brief Widget that represents a point. */
	template <class _Tp>
	class Point_ : public Widget<_Tp>, public cv::Point_<_Tp>
	{
	public:

		static const std::string CLASS_NAME; /*!< Class name to be displayed in the GUI.  */

#pragma region constructors
		/// Default constructor.
		Point_();

		/// Construct point at x,y coordinates.
		Point_(_Tp _x, _Tp _y);

		/// Construct point at same position as pt.
		Point_(const cv::Point_<_Tp> & pt);

		/// Construct point and ise width and height as x,y coordinates.
		Point_(const cv::Size_<_Tp>& sz);

		/// Construct point by finding x,y point in the vector.
		Point_(const cv::Vec<_Tp, 2>& v);
#pragma endregion constructors

		static Point_<_Tp> UNDEFINED; /*!< Represents a Point_ for which coordinates were not yet set.  */

#pragma region operators
		/// Conversion operator to cv::Point
		operator cv::Point_<_Tp>() const{ return m_position; };

		/// Checks if 2 points are equal.
		bool operator == (const Point_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<cv::Point_<_Tp>>(*this) == b;
		};

		/// Checks if 2 points are different.
		bool operator != (const Point_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<cv::Point_<_Tp>>(*this) != b;
		};
#pragma endregion operators

		/// Move point widget
		virtual void setPosition(cv::Point_<_Tp> position);

		/// Overlay the point widget on an image
		void paint(const cv::Mat&);

		/// The user moved the mouse pointer over the widget
		void onMouseMove(const cv::Point&);

		/// The user pressed the left mouse button over the widget
		void onMouseDown(const cv::Point&);

		/// The user released the left mouse button over the widget
		void onMouseUp(const cv::Point&);
	};

	typedef Point_<int> Point;
}

#include "Point.hxx"