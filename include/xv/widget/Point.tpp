#include <wx/wxprec.h>
#include "Point.hpp"
#include <climits>

using namespace xvWidgets;

cv::Point2f Point2f::UNDEFINED = cv::Point2f(
	std::numeric_limits<float>::lowest(),
	std::numeric_limits<float>::lowest() 
);