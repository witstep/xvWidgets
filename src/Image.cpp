#pragma once //header only mode
#include "xv/Image.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

using namespace xv;

const cv::Scalar Image::PADDING_COLOR(0, 0, 0);

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

bool Image::contains(const gui_point_t&)
{
	return true;
}

void Image::setBestSizeFit(int cols, int rows)
{
	int newCols, newRows;
	m_hBorder = 0; m_vBorder = 0; m_scale = 1;
	if (m_renderMat.cols == cols && m_renderMat.rows == rows)
		return; //no resize required

	float aspectRatio = (float)m_renderMat.cols / m_renderMat.rows;
	float vFactor = (float)rows / m_renderMat.rows;
	float hFactor = (float)cols / m_renderMat.cols;

	if (vFactor > hFactor){
		newCols = cols;
		newRows = cols / aspectRatio;
		m_vBorder = (rows - newRows) / 2;//padding
	}
	else{
		newCols = rows * aspectRatio;
		newRows = rows;
		m_hBorder = (cols - newCols) / 2;//padding
	}

	m_scale = (float)newCols / m_cvMat.cols;

	if (newCols < 1 || newRows < 1)
		return;//too small to draw

	cv::resize(m_renderMat, m_renderMat, cv::Size(newCols, newRows));
	cv::copyMakeBorder(m_renderMat, m_renderMat, m_vBorder, m_vBorder, m_hBorder, m_hBorder, cv::BORDER_ISOLATED, PADDING_COLOR);
}

gui_point_t Image::getPixelInterpolation(gui_point_t point)
{
	int x = point.x / m_scale - m_hBorder / m_scale;
	int y = point.y / m_scale - m_vBorder / m_scale;

	if (x > m_cvMat.cols - 1)
		x = m_cvMat.cols - 1;
	if (x < 0)
		x = 0;

	if (y > m_cvMat.rows - 1)
		y = m_cvMat.rows - 1;
	if (y < 0)
		y = 0;

	return gui_point_t(x, y);
}

void Image::createBitmap(int w, int h)
{
	m_mutex.lock();
	//create a blank bitmap
	if (m_cvMat.empty()){
		if (w < 1 || h < 1)
			return;
		m_renderMat = cv::Mat(h, w, CV_8UC3, PADDING_COLOR);
	}
	else{
		m_renderMat = m_cvMat.clone();

		if (m_children.size() > 0)
			for (auto &w : m_children)
				w->render(m_renderMat);

		cv::cvtColor(m_renderMat, m_renderMat, cv::COLOR_BGR2RGB);
		setBestSizeFit(w,h);
	}
	m_bitmap = ImageTranslator::toNativeGui(m_renderMat);
	m_mutex.unlock();
}