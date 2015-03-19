#include <wx/wxprec.h>
#include "../include/Contour.hpp"

using namespace cvWidgets;

template Contour < cv::Point2i >::Contour(const std::vector< cv::Point2i >& v);
template Contour < cv::Point2f >::Contour(const std::vector< cv::Point2f >& v);

template Contour<cv::Point2i>::Contour(const Mat&);
template Contour<cv::Point2f>::Contour(const Mat&);
template <class T>
Contour<T>::Contour(const Mat&){
	if (this->empty())
		showGUI();
}

template Contour<cv::Point2i>::Contour();
template Contour<cv::Point2f>::Contour();
template <class T>
Contour<T>::Contour(){
	if (this->empty())
		showGUI();
}

template <class T>

void Contour<T>::showGUI(){
	wxMessageBox("Contour");
}


