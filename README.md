# xvWidgets

An experimental cross-platform C++ GUI toolkit for OpenCV with Qt and wxWidgets bindings

## Status

The library is in an early development phase, major API and architecture can happen at any moment. It should become minimally usable and stable from the moment sample applications appear in the repository.

## Introduction

The goal of the project is to consolidate the code base and lessons learned from developing desktop applications, with both computer vision and GUI requirements, into a set of reusable components.

The usefulness of some components is not apparent and they may seem pointless. They are being built as a result of enforcing a basic set of conventions, and in the hope they can become useful in the future as building blocks of applications not yet imagined.

## Features
* Conventions
* User Input
	* Primitives
	* Composites
* Visualization
* Helper Classes


### Conventions
The toolkit follows a set of conventions intended to make usage intuitive and coherent with OpenCV programming.

* Widgets are named after the data structures or algorithms for which the graphical representation is being provided. For example the graphical representation of cv::Mat is provided by xv::Mat, in the same way, function cv::Canny is represented by the class xv::Canny.

* The data type and number of parameters passed to xvWidgets' contructors are the same as the OpenCV counterpart data structures and functions.

* xvWidgets representing functions provide the method run() to execute the underlying algorithm and return values.

* Argument omissions result in user input.

For example, consider the function cv::pointPolygonTest with the following prototype

```C++
double cv::pointPolygonTest(InputArray contour, Point2f pt, bool measureDist);
```

Resulting in the following constructor declarations and run method for xv::pointPolygonTest
```C++
pointPolygonTest(cv::InputArray contour);
pointPolygonTest(cv::InputArray contour, cv::Point2f pt = Point2f::NULL_POINT);
pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist);
double pointPolygonTest::run();

```

Constructing xv::pointPolygonTest with all the arguments set followed by a call to run(), is analogous to calling cv::pointPolygonTest directly.

```C++
double distance;
std::vector<cv::Point2f> contours( { Point2f(40,20), Point2f(25,60), Point2f(65,65) } );
distance = xv::pointPolygonTest ppt(contours, Point2f(10,20), true).run();

//in this case we might as well call OpenCV directly, the result would be the same.
distance = cv::pointPolygonTest(contours, Point2f(10,20), true);
```

xvWidgets becomes useful only when we ommit arguments.
If any of the constructor parameters is omitted or empty, the user is prompted for it through a GUI.

```C++
std::vector<cv::Point2f> contours( { Point2f(40,20), Point2f(25,60), Point2f(65,65) } );
std::vector<cv::Point2f> emptyContours();

//A pop-up dialog asks if the distance should be measured
xv::pointPolygonTest ppt1(contours, Point2f(10,20));

//The user is offered a graphical interface to draw a polygon
xv::pointPolygonTest ppt2(NULL, Point2f(10,20), true);

//The user is asked to draw a polygon and select a point
xv::pointPolygonTest ppt3(emptyContours, Point2f::NULL_POINT, true);

//The user will need to input all 3 parameters
xv::pointPolygonTest ppt4();
```