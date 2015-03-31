#pragma once

#include <vector>
#include <opencv2/core.hpp>

namespace xvWidgets{

	class Image;

	template <class T>
	class Contour : public std::vector<T>
	{
	public:
		Contour<T>(const Image&);
		Contour<T>();

		Contour<T>(const std::vector<T>& v) : std::vector<T>(v){};

	private:
		void showGUI();
	};

}

