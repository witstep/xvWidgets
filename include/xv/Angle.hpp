#pragma once

#include <list>
#include <opencv2/core.hpp>
#include "Widget.hpp"
#include "Point.hpp"

namespace xv{

	class Image;

	/** @brief Widget that represents an angle between 2 points
	*/
	class Angle : public Widget
	{
	public:
		/// Represents an Angle_ for which user input was not collected
		static Angle UNDEFINED;

#pragma region constructors
		/// Default constructor. Creates an undefined angle
		Angle();

		/// Creates an angle with vertex at a specified point
		Angle(cv::Point);
#pragma endregion constructors

		/// Moves the widget
		virtual void setPosition(cv::Point position);

		/// Conversion operator that returns the angle as a double expressed in radians
		operator double() const{ 
			return atan2(m_vertex.y - m_pointA.y, m_vertex.x - m_pointA.x);
		};

		/// Checks if 2 contours are equal
		bool operator == (const Angle &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			return false;
		};

		/// Checks if 2 contours are different
		bool operator != (const Angle &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			return true;
		};
	private:
		void paint(const cv::Mat&);
		void onMouseMove(const cv::Point&);
		void onMouseDown(const cv::Point&);
		void onMouseUp(const cv::Point&);
		void setMouseOver(bool mouseOver);
		void defineContours();

		Point m_vertex, m_pointA, m_pointB;
		int m_radius = 0;
	};

}
