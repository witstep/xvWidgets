#pragma once

#include <opencv2/core.hpp>
#include <vector>

namespace xv {

	template <typename _Tp>
	_Tp distance(cv::Point_<_Tp>, cv::Point_<_Tp>);

	template <typename _Tp>
	class Image_;

	/** @brief Base class for all Widgets
	*/

	template <typename _Tp>
	class Widget{
		friend Image_<_Tp>;
	public:

		/// Display widget in input mode (with OK/Cancel buttons)
		virtual void render(const cv::Mat&);

		/// Display widget in output mode (without OK/Cancel buttons)
		virtual void paint(const cv::Mat&) = 0;

		/// Check if point is inside the widget
		virtual bool contains(const cv::Point_<_Tp>&);

		/// Draw the OK/Cancel buttons
		virtual void paintButtons(const cv::Mat&);

		/// The user clicked the left mouse button over the widget
		virtual void onMouseDown(const cv::Point&) = 0;

		/// The user moved the mouse pointer over the widget
		virtual void onMouseMove(const cv::Point&) = 0;

		/// The user released the left mouse button over the widget
		virtual void onMouseUp(const cv::Point&) = 0;

		/// Signal that the user started dragging the widget
		void startDragging(){ m_dragging = true; };

		/// Signal that the user stopped dragging the widget
		void stopDragging(){ m_dragging = false; };

		/// Check if the user is dragging the widget
		bool isDragging(){ return m_dragging; };

		/// Signal that the user placed the mouse pointer over the widget
		virtual void setMouseOver(bool mouseOver){ m_mouseOver = mouseOver; };

		/// Check if the mouse pointer is over the widget
		virtual bool isMouseOver(){ return m_mouseOver; };

		/// Check if Widget is read-only
		virtual bool isReadOnly(){ return m_readonly; };

		/// Move the widget to a point in an image
		virtual void setPosition(cv::Point_<_Tp> position);

		/// Move the widget to the center of the image
		virtual void center();

		/// Get the current position
		virtual cv::Point_<_Tp> position();

		/// Check if the mouse pointer is over a button
		bool isMouseOverButton(cv::Point_<_Tp> mousePosition, cv::Point_<_Tp> buttonPosition);

		/// Make it disappear from an Image
		void hide();

	protected:
		/// The thickness of contour lines
		static const _Tp LINE_THICKNESS;

		/// Generic margin around widget
		static const _Tp MARGIN;

		/// Used to define the size of buttons
		static const int BUTTON_RADIUS;

		/// Offset of the OK button in relation to the widget position
		static const cv::Point OK_POSITION;

		/// Offset of the Cancel button in relation to the widget position
		static const cv::Point CANCEL_POSITION;

		/// Color used to draw main components of widget
		static const cv::Scalar FOREGROUND_COLOR;

		/// Color used to draw a few important components of the widget
		static const cv::Scalar HIGHLIGHT_COLOR;

		/// Color associated with OK/accept action
		static const cv::Scalar AFFIRMATIVE_COLOR;

		/// Color associated with Cancel/reject action
		static const cv::Scalar NEGATIVE_COLOR;

		virtual ~Widget() = 0;

		/// Pointer to the parent image used to display the widget
		Image_<_Tp> *m_image = NULL;

		/// The contours of area occupied by the widget
		std::vector<cv::Point_<_Tp>> m_contour;

		bool m_dragging = false;  /*!< The user is dragging the widget */
		bool m_canceling = false;  /*!< The user left clicked over cancel and didn't release the mouse button */
		bool m_accepting = false;  /*!< The user left clicked over ok and didn't release the mouse button */
		bool m_mouseOver = false; /*!< The mouse pointer is over the widget */
		bool m_undefined = false; /*!< The member variable that stores a primitive datatype doesn't hold a defined value */
		bool m_positioned = false;/*!< Was the Widget positioned in a particular place, independently of being undefined */
		bool m_readonly = false; /*!< The widget can be made visible but the user can't interact with it */

		cv::Point_<_Tp> m_position; /*!< The position of the widget, usually the center point */

	private:

	};
}

#include "Widget.tpp"