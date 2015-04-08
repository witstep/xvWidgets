#pragma once

#include <opencv2/core.hpp>

namespace xv {

	template <typename _Tp>
	class Image;

	template <typename _Tp>
	class Widget{
	public:
		static const cv::Scalar FOREGROUND_COLOR;
		static const cv::Scalar HIGHLIGHT_COLOR;
		static const cv::Scalar AFFIRMATIVE_COLOR;
		static const cv::Scalar NEGATIVE_COLOR;

		virtual ~Widget() = 0;
		virtual void paint(const cv::Mat&) = 0;
		virtual void onMouseDown(const cv::Point&) = 0;
		virtual void onMouseMove(const cv::Point&) = 0;
		virtual void onMouseUp(const cv::Point&) = 0;

		friend void operator >> (Image<_Tp> &image, Widget<_Tp> &widget){
			if (std::find(image.m_widgets.begin(), image.m_widgets.end(), &widget) == image.m_widgets.end()){
				image.m_widgets.push_back(&widget);
				widget.m_image = &image;
			}
		};
	protected:
		Image<_Tp> *m_image = NULL;
		static const _Tp CROSSHAIR_OFFSET;
	};
}

#include "Widget.tpp"