# xvWidgets
An experimental cross-platform C++ GUI toolkit for computer vision applications with (planned) Qt and wxWidgets bindings

## Introduction
The goal of the project is to consolidate the code base and lessons learned from developing desktop applications, with both computer vision and GUI requirements, into a set of reusable components.
The initials xv stand for an arbitrary GUI toolkit (x) and vision (v).

## Status
The library is in an early development phase, major API and architecture changes can happen at any time. It should become minimally usable and stable from the moment sample applications appear in the repository.
The initial OpenCV centric architecture is being dropped in favor of an approach that doesn't depend on any particular CV library. OpenCV is still supported and most sample code will be based on it.

## Source Code
The source code is hosted in [GitHub](https://github.com/witstep/xvWidgets).

## Documentation
The Doxygen generated API reference is hosted in [xvwidgets.witstep.com](http://xvwidgets.witstep.com/).

## Usage
The main *raison d'etre* of the library was the need for a simple and intuitive way of accepting user input for primitive 2D shapes, as exemplified bellow.

* User input of an area contour using a Polygon widget and an OpenCV matrix

![Polygon Input](http://xvwidgets.witstep.com/Polygon_clip.gif "Polygon clip")  
Clip from Tears of Steel | (CC) Blender Foundation | [mango.blender.org](http://mango.blender.org)

~~~~~~~~~~~~~~~{.cpp}
cv::Mat cvMatrix;
xv::ImagePanel imagePanel; 
xv::Polygon polygon;

//...it is assumed an image was loaded into the cvMatrix

imagePanel << cvMatrix; //display the image on screen
imagePanel >> polygon; //lets the user define the polygon within the image

//The above operation is asynchronous, typically the state of the widget is checked, bellow, on a different thread
if(polygon != xv::Polygon::UNDEFINED) //the user clicked the ok button on the widget
	std::cout << "The polygon is defined by " << polygon.size() << " points." << std::endl;

~~~~~~~~~~~~~~~

* Displaying an Angle
~~~~~~~~~~~~~~~{.cpp}
cv::Mat cvMatrix;
xv::ImagePanel imagePanel; 
xv::Angle angle(
	xv::Point(100,100), //vertex
	xv::Point(150,100)  //first ray
);

//...it is assumed an image was loaded into the cvMatrix

// show the OpenCV image and the representation of the angle on the GUI panel
imagePanel << cvMatrix << angle;

~~~~~~~~~~~~~~~

## Modules

* [ Image Representation ](http://xvwidgets.witstep.com/group___image_representation.html)
	* Image
	* ImageTranslator
* [ Widgets ](http://xvwidgets.witstep.com/group___widgets.html)
	* Point
	* Line
	* Ellipse
	* Polygon
	* Angle
	* Label
* [ Rendering ](http://xvwidgets.witstep.com/group___Rendering.html)
	* ImagePanel
	* VideoPlayer
* [Media I/O](http://xvwidgets.witstep.com/group___MediaIO.html)
	* VideoCapture
	* VideoWriter	
