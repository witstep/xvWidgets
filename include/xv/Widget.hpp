#pragma once

#include <opencv2/core.hpp>
#include <vector>

namespace xv {

	template <typename _Tp>
	_Tp distance(cv::Point_<_Tp>, cv::Point_<_Tp>);

	template <typename _Tp>
	class Image_;

	template <typename _Tp>
	class Widget{
		friend Image_<_Tp>;
	public:
		static const _Tp MARGIN; /*\< test*/
		static const int BUTTON_RADIUS;
		static const cv::Point OK_POSITION;
		static const cv::Point CANCEL_POSITION;
		static const cv::Scalar FOREGROUND_COLOR;
		static const cv::Scalar HIGHLIGHT_COLOR;
		static const cv::Scalar AFFIRMATIVE_COLOR;
		static const cv::Scalar NEGATIVE_COLOR;

		//widgets for user input
		friend void operator >> (Image_<_Tp> &image, Widget<_Tp> &widget){
			if (std::find(image.m_widgets.begin(), image.m_widgets.end(), &widget) == image.m_widgets.end()){
				if (widget.m_undefined)//center widget with undefined value in the image
					widget.setPosition(cv::Point_<_Tp>(image.m_cvMat.size()/2));
				image.m_widgets.push_back(&widget);
				widget.m_image = &image;
			}else{
				assert(("Widget already added to the Image",false));
			}
		};

		//simple rendering of widget without user input
		void operator >> (Image_<_Tp> &image){
			this->paint(image);
		};

		virtual void render(const cv::Mat&);
		virtual void paint(const cv::Mat&) = 0;
		virtual bool contains(const cv::Point_<_Tp>&);
		virtual void paintButtons(const cv::Mat&);
		virtual void onMouseDown(const cv::Point&) = 0;
		virtual void onMouseMove(const cv::Point&) = 0;
		virtual void onMouseUp(const cv::Point&) = 0;

		void startDragging(){ m_dragging = true; };
		void stopDragging(){ m_dragging = false; };
		bool isDragging(){ return m_dragging; };
		virtual void setMouseOver(bool mouseOver){ m_mouseOver = mouseOver; };
		virtual bool isMouseOver(){ return m_mouseOver; };
		virtual void setPosition(cv::Point_<_Tp> position);
		virtual cv::Point_<_Tp> position();
		bool isMouseOverButton(cv::Point_<_Tp> mousePosition, cv::Point_<_Tp> buttonPosition);
	protected:
		virtual ~Widget() = 0;

		Image_<_Tp> *m_image = NULL;
		std::vector<cv::Point_<_Tp>> m_contour;
		bool m_dragging = false;  /*!< The user is dragging the widget */
		bool m_mouseOver = false; /*!< The mouse pointer is over the widget */
		bool m_undefined = false; /*!< The member variable that stores a primitive datatype doesn't hold a defined value */
		cv::Point_<_Tp> m_position;

	private:

	};
}

#include "Widget.tpp"