#pragma once

#include <vector>
#include <opencv2/core.hpp>

namespace xv{

	template <typename _Tp>
	class Image_;

	template <class _Tp>
	class Contour_ : public Widget<_Tp>, public std::vector<Point_<_Tp>>
	{
	public:
#pragma region constructors
		Contour_<_Tp>();
		Contour_<_Tp>(const std::vector<_Tp>& v) : std::vector<_Tp>(v){};
#pragma endregion constructors

	private:
		void paint(const cv::Mat&);
		void onMouseMove(const cv::Point&);
		void onMouseDown(const cv::Point&);
		void onMouseUp(const cv::Point&);
		virtual cv::Point_<_Tp> position();
	};

	typedef Contour_<int> Contour;

}

#include "Contour.tpp"
