#pragma once

#include "Image.hpp"
#include "Point.hpp"
#include "Contour.hpp"
#include <opencv2/core.hpp>

namespace xvWidgets{

	class pointPolygonTest
	{
	public:
		pointPolygonTest(cv::InputArray contour);
		pointPolygonTest(cv::InputArray contour, cv::Point2f pt = Point2f::UNDEFINED);
		pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist);
		double run(bool gui=false);
	private:
		cv::InputArray m_contour;
		cv::Point2f m_pt;
		bool m_measureDist;
	};

}