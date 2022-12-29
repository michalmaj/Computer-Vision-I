/*
 * Hough Transform
 * Hough transform is a feature extraction method for detecting simple shapes such as circles, lines, etc, in an image.
 * A simple shape is one that can be represented by only a few parameters. For example a line can be represented by two
 * parameters (slope and intercept) and a circle has three parameters - the coordinate of the center (x, y) and the
 * radius (r). The main advantage of using Hough transform is that it is insensitive to occlusion.
 *
 * In OpenCV we have two function for that transform: HoughLines() and HoughLinesP (probabilistic Hough Transform).
 * Syntax:
 *	void cv::HoughLinesP(InputArray image, OutputArray lines, double rho, double theta, int threshold,
 *						 double minLineLength=0, double maxLineGap=0)
 *
 *	void cv::HoughLines(InputArray image, OutputArray lines, double rho, double theta, int threshold,
 *						double srn=0, double stn=0, double min_theta=0, double max_theta=CV_PI)
 * Parameters:
 *	- image - 8-bit, single-channel binary source image.
 *	- lines - output vector of lines. Each line is represented by a 4-element vector (x1, y1, x2, y2), where (x1, y2)
 *			  and (x2, y2) are the ending points of each detected line segment.
 *	- rho - distance resolution of the accumulator in pixels.
 *	- theta - angle resolution of the accumulator in radians.
 *	- threshold - accumulator threshold parameter. Only those lines are returned that get enough votes (> threshold).
 *	- srn - for the multi-scale Hough transform, it is a divisor for the distance resolution rho. The coarse accumulator
 *			distance resolution is rho and the accurate accumulator resolution is rho/srn. If both srn=0 and stn=0, the
 *			classical Hough transform is used. Otherwise both these parameters should be positive.
 *	- stn - for the multi-scale Hough transform, it is a divisor for the distance resolution theta.
 *	- min_theta - for standard and multi-scale Hough transform, minimum angle to check for lines. Must fall between
 *				  0 and max_theta.
 *	- max_theta - for standard and multi-scale Hough transform, maximum angle to check for lines. Must fall between
 *				  min_theta and CV_PI.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

int main()
{
	// Loa an image from disk
	cv::Mat img{ cv::imread("../data/images/lanes.jpg", cv::IMREAD_COLOR) };

	// Convert image to gray-scale
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	// Find edges using Canny detector
	cv::Mat edges;
	cv::Canny(gray, edges, 50, 200);

	// Detect points that form a line
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 100, 10, 250);

	// Draw lines on the Original Image
	for(size_t i{0}; i < lines.size(); ++i)
	{
		cv::Vec4i line = lines.at(i);
		cv::line(img, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
	}

	// Show the result
	cv::imshow("Lines", img);
	cv::waitKey(0);
	cv::destroyWindow("Lines");


	return 0;
}