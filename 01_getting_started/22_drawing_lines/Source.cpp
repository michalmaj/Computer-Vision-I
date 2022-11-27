/*
 * Draw a Line
 * For drawing a line in OpenCV library we can use cv::line function.
 * Syntax:
 *	void cv::line(InputOutputArray img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=LINE_8, int shift=0)
 * Arguments
 *	- img - image on which we will draw a line
 *	- pt1 - first point of the line segment
 *	- pt2 - second point of the line segment
 *	- color - color of the line which will be drawn
 *	- thickness - integer specifying the line thickness - default value is 1
 *	- lineType - type of the line. Default value is LINE_8 which stands for an 8-connected line. Usually, LINE_AA (anti-aliased line) is used.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load the image from disk
	cv::Mat image{ cv::imread("../data/images/boy.jpg", cv::IMREAD_COLOR) };

	// Draw a line
	/*
	 * The line starts from (200, 80) at ends at (280, 80) - (x, y)-coordinates
	 * The color of the line is GREEN (in BGR format)
	 * Thickness of line is 3px
	 * Line type is LINE_AA
	 */

	// Make a copy
	cv::Mat imageLine = image.clone();

	cv::line(imageLine, cv::Point(200, 80), cv::Point(280, 80), cv::Scalar(0, 255, 0), 3, cv::LINE_AA);

	// Show the image with line
	cv::imshow("Image with Line", imageLine);
	cv::waitKey(0);
	cv::destroyWindow("Image with Line");

	return 0;
}