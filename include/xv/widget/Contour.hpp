#pragma once

#include <list>
#include <opencv2/core.hpp>
#include "../Widget.hpp"
#include "Point.hpp"

namespace xv{

	template <typename _Tp>
	class Image_;

	template <typename _Tp>
	class Contour_ : public Widget<_Tp>, public std::vector<Point_<_Tp>>
	{
	public:
		static Contour_<_Tp> UNDEFINED; /*!< Represents a Contour_ for which points were not yet set.  */

#pragma region constructors
		Contour_<_Tp>();
		Contour_<_Tp>(const std::list<_Tp>& v) : std::list<_Tp>(v){};
#pragma endregion constructors

		bool operator == (const Contour_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return isEqual(*this, b);
		};

		bool operator != (const Contour_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return !isEqual(*this, b);
		};

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
