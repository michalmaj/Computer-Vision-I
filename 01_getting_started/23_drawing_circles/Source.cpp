/*
 * Draw a Circle
 * For drawing a circle in OpenCV library we can use cv::circle function.
 * Syntax:
 *	void cv::circle(InputOutputArray img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=LINE_8, int shift=0)
 * Arguments:
 *	- img - image where the circle is drawn.
 *	- center - center of the circle
 *	- radius -  radius of the circle
 *	- color - circle color
 *	- thickness - thickness of the circle outline (if positive). If a negative value is supplied for this argument. It will result in a filled circle.
 *	- line type - type of the circle boundary. This is exact same as lineType argument in cv::line 
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load the image from disk
	cv::Mat image{ cv::imread("../data/images/boy.jpg", cv::IMREAD_COLOR) };

	// Make a copy
	cv::Mat imageCircle = image.clone();
	cv::Mat imageFilledCircle = image.clone();

	// Draw a circle
	cv::circle(imageCircle, cv::Point(250, 125), 100, cv::Scalar(255, 0, 0), 5, cv::LINE_AA);

	// Draw filled circle
	cv::circle(imageFilledCircle, cv::Point(250, 125), 100, cv::Scalar(255, 0, 0), -1, cv::LINE_AA);

	cv::imshow("Image with Circle", imageCircle);
	cv::imshow("Image with Filled Circle", imageFilledCircle);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}