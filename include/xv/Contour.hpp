#pragma once

#include <list>
#include <opencv2/core.hpp>
#include "Widget.hpp"
#include "Point.hpp"

namespace xv{

	template <typename _Tp>
	class Image_;

	/** @brief Widget that represents a contour defined by a set of points */
	template <typename _Tp>
	class Contour_ : public Widget<_Tp>, public std::vector<Point_<_Tp>>
	{
	public:
		/// Represents a Contour_ for which points were not yet set.
		static Contour_<_Tp> UNDEFINED; 

#pragma region constructors

		/// Default constructor. Create a Contour in undefined state.
		Contour_<_Tp>();

		/// Creates a contour from a list of points.
		Contour_<_Tp>(const std::list<_Tp>& v) : std::list<_Tp>(v){};
#pragma endregion constructors

		/// Checks if 2 contours are equal
		bool operator == (const Contour_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			return isEqual(*this, b);
		};

		/// Checks if 2 contours are different
		bool operator != (const Contour_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return !isEqual(*this, b);
		};

		/// Moves the widget
		virtual void setPosition(cv::Point_<_Tp> position);
	private:
		void paint(const cv::Mat&);
		void paintAddPointButton(const Point_<_Tp>&, const cv::Mat& image);
		void onMouseMove(const cv::Point&);
		void onMouseDown(const cv::Point&);
		void onMouseUp(const cv::Point&);
		void setMouseOver(bool mouseOver);
		void defineContours();
		static bool isEqual(const Contour_<_Tp>&, const Contour_<_Tp>&);

		Point_<_Tp> m_centerPoint;

		void setDefaultPoints();
		cv::Point_<_Tp> getMiddlePoint();
		void shiftPosition(cv::Point_<_Tp>);
	};

	typedef Contour_<int> Contour;

}

#include "Contour.tpp"
