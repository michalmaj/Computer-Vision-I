/*
 * Draw a rectangle
 * For drawing a rectangle in OpenCV library we can use cv::rectangle function.
 * Syntax:
 *	void cv::rectangle(InputOutputArray img, Point pt1, Point pt2, const Scalar& color, int thickness=1, lineType=LINE_8, int shift=0)
 * Arguments:
 *	- img - image on which the rectangle is to be drawn.
 *	- pt1 - vertex of the rectangle. Usually we use the top-left vertex here.
 *	- pt2 - vertex of the rectangle opposite to pt1. Usually we use the bottom-right vertex here.
 *	- color - rectangle color.
 *	- thickness - thickness of the rectangle outline (if positive). If a negative value is supplied for this argument. It will result in a filled rectangle.
 *	- line type - type of the rectangle boundary. This is exact same as lineType argument in cv::line and cv::circle etc.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load the image from disk
	cv::Mat image{ cv::imread("../data/images/boy.jpg", cv::IMREAD_COLOR) };

	// Create a copy of the original image
	cv::Mat imageRectangle = image.clone();

	// Draw a rectangle
	/*
	 * For drawing this shape we need two points. These are the opposite vertices of the rectangle. From the sample image, we can approximately find the vertices as:
	 *	- top-left - (170, 50)
	 *	- bottom-right - (300, 200)
	 */

	cv::rectangle(imageRectangle, cv::Point(170, 50), cv::Point(300, 200), cv::Scalar(255, 0, 255), 5, cv::LINE_AA);

	// Show the image
	cv::imshow("Image Rectangle", imageRectangle);
	cv::waitKey(0);
	cv::destroyWindow("Image Rectangle");

	// Draw filled rectangle
	cv::Mat imageFilledRectangle = image.clone();

	cv::rectangle(imageFilledRectangle, cv::Point(170, 50), cv::Point(300, 200), cv::Scalar(255, 0, 255), -1, cv::LINE_AA);

	// Show the image
	cv::imshow("Image Filled Rectangle", imageFilledRectangle);
	cv::waitKey(0);
	cv::destroyWindow("Image Filled Rectangle");

	return 0;
}