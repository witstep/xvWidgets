#include <wx/wxprec.h>
#include "../include/Point.hpp"
#include <climits>

using namespace cvWidgets;

cv::Point2f Point2f::NULL_POINT = cv::Point2f(
	std::numeric_limits<float>::lowest(),
	std::numeric_limits<float>::lowest() 
);