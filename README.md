# cvWidgets

A cross-platform C++ GUI toolkit based in OpenCV and wxWidgets

## Features
* Conventions
* User Input
	* Primitives
	* Composites
* Visualization
* Helper Classes


### Conventions
The toolkit follows a set of conventions intended to make usage intuitive and coherent with OpenCV programming.

* Widgets are named after the data structures or algorithms for which the graphical representation is being provided. For example the graphical representation of cv::Mat is provided by cvWidgets::Mat, in the same way, function cv::Canny is represented by the class cvWidgets::Canny.


* The data type and number of parameters passed to cvWidgets' contructors are the same as the OpenCV counterpart data structures and functions.

* cvWidgets representing functions provide the method run() to execute the underlying algorithm and return values.

* Argument omissions result in user input.

For example, consider the function cv::pointPolygonTest with the following prototype

```C++
double cv::pointPolygonTest(InputArray contour, Point2f pt, bool measureDist);
```

Resulting in the following constructor declarations and run method for cvWidgets::pointPolygonTest
```C++
pointPolygonTest(cv::InputArray contour);
pointPolygonTest(cv::InputArray contour, cv::Point2f pt = Point2f::NULL_POINT);
pointPolygonTest(cv::InputArray contour, cv::Point2f pt, bool measureDist);
double pointPolygonTest::run();

```

Constructing cvWidgets::pointPolygonTest with all the arguments set followed by a call to run(), is analogous to calling cv::pointPolygonTest directly.

```C++
double distance;
std::vector<cv::Point2f> contours( { Point2f(40,20), Point2f(25,60), Point2f(65,65) } );
distance = cvWidgets::pointPolygonTest ppt(contours, Point2f(10,20), true).run();

//in this case we might as well call OpenCV directly, the result would be the same.
distance = cv::pointPolygonTest(contours, Point2f(10,20), true);
```

cvWidgets becomes useful only when we ommit arguments.
If any of the constructor parameters is omitted or empty, the user is prompted for it through a GUI.

```C++
std::vector<cv::Point2f> contours( { Point2f(40,20), Point2f(25,60), Point2f(65,65) } );
std::vector<cv::Point2f> emptyContours();

//A pop-up dialog asks if the distance should be measured
cvWidgets::pointPolygonTest ppt1(contours, Point2f(10,20));

//The user is offered a graphical interface to draw a polygon
cvWidgets::pointPolygonTest ppt2(NULL, Point2f(10,20), true);

//The user is asked to draw a polygon and select a point
cvWidgets::pointPolygonTest ppt3(emptyContours, Point2f::NULL_POINT, true);

//The user will need to input all 3 parameters
cvWidgets::pointPolygonTest ppt4();
```