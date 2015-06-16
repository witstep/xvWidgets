#include "xv/ImageTranslator.hpp"
#include "xv/Image.hpp"

using namespace xv;

ImageTranslator::ImageTranslator()
{
}

ImageTranslator::~ImageTranslator()
{
}

gui_image_t ImageTranslator::toNativeGui(const cv::Mat& mat)
{
	return gui_image_t(mat.cols, mat.rows, (unsigned char*)mat.data, true);
}