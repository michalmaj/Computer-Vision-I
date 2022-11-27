/*
 * Draw an ellipse
 * For drawing an ellipse in OpenCV library we can use cv::ellipse function.
 * Syntax:
 *	void cv::ellipse(InputOutputArray img, Point center, Size axes, double angle, double startAngle, double endAngle,
 *					 cont Scalar& color, int thickness=1, int lineType=LINE_8, int shift=0)
 * Arguments
 *	- img - image on which the ellipse is to be drawn
 *	- center - center of the ellipse
 *	- axes - half of the size of the ellipse main axes
 *	- angle - ellipse rotation angle in degrees
 *	- startAngle - starting angle of the elliptic arc in degrees
 *	- endAngle - ending angle of the elliptic arc in degrees
 *	- color - ellipse line color
 *	- thickness - thickness of the ellipse outline (if positive). If a negative value is supplied for this argument. It will result in a filled ellipse.
 *	- line type - type of the ellipse boundary. This is exact same as lineType argument in cv::line and cv::circle
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load the image from disk
	cv::Mat image{ cv::imread("../data/images/boy.jpg", cv::IMREAD_COLOR) };

	// Make a copy of the image
	cv::Mat imageEllipse = image.clone();

	// Draw an ellipse, note: Ellipse centers and Axis lengths must be integers

	// Draw an ellipse at an angle of 0 degrees with blue color - it will be a complete ellipse (the starting angle is 0 and ending is 360)
	cv::ellipse(imageEllipse, cv::Point(250, 125), cv::Point(100, 50), 0, 0, 360, cv::Scalar(255, 0, 0), 3, cv::LINE_AA);

	// Draw an ellipse at an angle of 90 degrees with red color - it will be also a complete ellipse
	cv::ellipse(imageEllipse, cv::Point(250, 125), cv::Point(100, 50), 90, 0, 360, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);

	// Show the image with ellipse
	cv::imshow("Image with ellipse", imageEllipse);
	cv::waitKey(0);
	cv::destroyWindow("Image with ellipse");

	// Draw more variations of the ellipse
	imageEllipse = image.clone();

	// Incomplete/Open ellipse
	cv::ellipse(imageEllipse, cv::Point(250, 125), cv::Point(100, 50), 0, 180, 360, cv::Scalar(255, 0, 0), 3, cv::LINE_AA);

	// Filed ellipse
	cv::ellipse(imageEllipse, cv::Point(250, 125), cv::Point(100, 50), 0, 0, 180, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);

	// Show the image with ellipse
	cv::imshow("Image with ellipse", imageEllipse);
	cv::waitKey(0);
	cv::destroyWindow("Image with ellipse");


	return 0;
}