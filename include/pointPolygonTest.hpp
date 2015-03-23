#pragma once

#include "Mat.hpp"
#include "Point.hpp"
#include "Contour.hpp"
#include <opencv2/core.hpp>

namespace cvWidgets{

	class pointPolygonTest
	{
	public:
		pointPolygonTest(cv::InputArray contour);
		pointPolygonTest(cv::InputArray contour, cv::Point2f pt = Point2f::NULL_POINT);
		pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist);
		double run(bool gui=false);
	private:
		cv::InputArray m_contour;
		cv::Point2f m_pt;
		bool m_measureDist;
	};

}

