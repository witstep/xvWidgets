#include <wx/wxprec.h>
#include "../include/pointPolygonTest.hpp"
#include "opencv2/imgproc.hpp"
#include <sstream>

using namespace cvWidgets;

pointPolygonTest::pointPolygonTest(cv::InputArray contour)
	:m_contour(contour)
{

}

pointPolygonTest::pointPolygonTest(cv::InputArray contour, cv::Point2f pt)
	: m_contour(contour), m_pt(pt)
{

}

pointPolygonTest::pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist)
	: m_contour(contour), m_pt(pt), m_measureDist(measureDist)
{

}

double pointPolygonTest::run(bool gui)
{
	double retVal = cv::pointPolygonTest(m_contour, m_pt, m_measureDist);
	if (gui){
		std::stringstream ss;
		ss << retVal;
		wxMessageBox(ss.str(), "pointPolygonTest");
	}
	return retVal;
}