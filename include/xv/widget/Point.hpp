#pragma once

#include "../Widget.hpp"
#include <opencv2/core.hpp>

namespace xv{
	
	template <class _Tp>
	class Point_ : public Widget<_Tp>, public cv::Point_<_Tp>
	{
	public:
		static const std::string CLASS_NAME;
#pragma region constructors
		Point_();
		Point_(_Tp _x, _Tp _y);
		Point_(const cv::Point_<_Tp> & pt);
		Point_(const cv::Size_<_Tp>& sz);
		Point_(const cv::Vec<_Tp, 2>& v);
#pragma endregion constructors

		static Point_<_Tp> UNDEFINED;

#pragma region operators
		operator cv::Point_<_Tp>() const{ return m_position; };
		bool operator == (const Point_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return true;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<cv::Point_<_Tp>>(*this) == b;
		};

		bool operator != (const Point_<_Tp> &b) {
			if (this->m_undefined && b.m_undefined)
				return false;//if both are undefined they are considered equal
			//compare as a cv::Point_
			return static_cast<cv::Point_<_Tp>>(*this) != b;
		};
#pragma endregion operators
		void paint(const cv::Mat&);
		void onMouseMove(const cv::Point&);
		void onMouseDown(const cv::Point&);
		void onMouseUp(const cv::Point&);
		virtual cv::Point_<_Tp> position();
	private:
	};

	typedef Point_<int> Point2i;
	typedef Point2i Point;
	typedef Point_<float> Point2f;
	typedef Point_<double> Point2d;
}

#include "Point.tpp"