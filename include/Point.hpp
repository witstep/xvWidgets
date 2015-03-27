#pragma once

#include <opencv2/core.hpp>

namespace xvWidgets{
	
	template <class T>
	class Point_{
	public:
		static T UNDEFINED;

	};

	typedef Point_<cv::Point2i> Point2i;
	typedef Point_<cv::Point2f> Point2f;
	typedef Point_<cv::Point2d> Point2d;
	typedef Point2i Point;
}