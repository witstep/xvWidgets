#pragma once

#include <list>
#include <opencv2/core.hpp>
#include "../Widget.hpp"

namespace xv{

	template <typename _Tp>
	class Image_;

	template <class _Tp>
	class Contour_ : public Widget<_Tp>, public std::vector<Point_<_Tp>>
	{
	public:
#pragma region constructors
		Contour_<_Tp>();
		Contour_<_Tp>(const std::list<_Tp>& v) : std::list<_Tp>(v){};
#pragma endregion constructors
		virtual void setPosition(cv::Point_<_Tp> position);
	private:
		void paint(const cv::Mat&);
		void paintAddPointButton(const Point_<_Tp>&, const cv::Mat& image);
		void onMouseMove(const cv::Point&);
		void onMouseDown(const cv::Point&);
		void onMouseUp(const cv::Point&);
		void setMouseOver(bool mouseOver);

		Point_<_Tp> m_centerPoint;

	};

	typedef Contour_<int> Contour;

}

#include "Contour.tpp"
