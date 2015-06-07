#pragma once //header only mode
#include "xv/Image.hpp"
#include "opencv2/core.hpp"

using namespace xv;

Image::Image()
{

}

Image::~Image()
{

}


void Image::render(const cv::Mat&)
{
}

void Image::paint(const cv::Mat&)
{
}

bool Image::contains(const cv::Point&)
{
	return true;
}

void Image::onMouseDown(const cv::Point&)
{
}

void Image::onMouseMove(const cv::Point&)
{
}

void Image::onMouseUp(const cv::Point&)
{
}