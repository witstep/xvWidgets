#include "xv/Widget.hpp"
#include "Image.hpp"

#include <algorithm>

using namespace xv;

template <typename _Tp>
const cv::Scalar Widget<_Tp>::FOREGROUND_COLOR(255, 0, 0);

template <typename _Tp>
const cv::Scalar Widget<_Tp>::HIGHLIGHT_COLOR(0, 255, 255);

template <typename _Tp>
const cv::Scalar Widget<_Tp>::AFFIRMATIVE_COLOR(0, 255, 0);

template <typename _Tp>
const cv::Scalar Widget<_Tp>::NEGATIVE_COLOR(0, 0, 255);

template <typename _Tp>
Widget<_Tp>::~Widget()
{
	
}