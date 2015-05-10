#pragma once

#include <list>
#include <opencv2/core.hpp>
#include "Widget.hpp"

namespace xv{

	template <typename _Tp>
	class Image_;

	template <class _Tp>
	class Angle_ : public Widget<_Tp>
	{
	public:
#pragma region constructors
		Angle_<_Tp>();
		Angle_<_Tp>(cv::Point_<_Tp>);
#pragma endregion constructors

		virtual void setPosition(cv::Point_<_Tp> position);

		operator double() { 
			return atan2(m_vertex.y - m_pointA.y, m_vertex.x - m_pointA.x);
		};
	private:
		void paint(const cv::Mat&);
		void onMouseMove(const cv::Point&);
		void onMouseDown(const cv::Point&);
		void onMouseUp(const cv::Point&);
		void setMouseOver(bool mouseOver);
		void defineContours();

		Point_<_Tp> m_vertex, m_pointA, m_pointB;
		std::list< Point_<_Tp>* >  m_points;
	};

	typedef Angle_<int> Angle;

}

#include "Angle.tpp"
