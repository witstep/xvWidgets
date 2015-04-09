#pragma once

#include "../Widget.hpp"
#include <opencv2/core.hpp>

namespace xv{
namespace widget{
	
	template <class _Tp>
	class Point_ : public Widget<_Tp>
	{
	public:
		static const wxString CLASS_NAME;
		Point_();
		Point_(_Tp _x, _Tp _y) { m_position = cv::Point_<_Tp>(_x, _y); };
		Point_(const cv::Point_<_Tp> & pt) : m_position(pt) {};
		Point_(const cv::Size_<_Tp>& sz) : m_position(sz) {};
		Point_(const cv::Vec<_Tp, 2>& v) : m_position(v) {};

		static cv::Point_<_Tp> UNDEFINED;

#pragma region operators
		operator _Tp() const{ return m_position; };
#pragma endregion operators

	private:
		//cv::Point_<_Tp> m_position;
		void paint(const cv::Mat&);
		void onMouseMove(const cv::Point&);
		void onMouseDown(const cv::Point&);
		void onMouseUp(const cv::Point&);
		void initWidget();
		bool m_dragging=false;

	};

	typedef Point_<int> Point2i;
	typedef Point2i Point;
	typedef Point_<float> Point2f;
	typedef Point_<double> Point2d;
}
}

#include "Point.tpp"