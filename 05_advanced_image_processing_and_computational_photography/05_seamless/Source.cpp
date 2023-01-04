/*
 * Seamless Cloning:
 * To do the seamless cloning we have to perform gradient domain - Poisson blending:
 *	1. Find the x and y gradients of the source and destination images.
 *	2. Copy the gradients from source images to the destination image.
 *	3. Integration in the gradients domain with Dirichlet boundary conditions.
 *
 * For that OpenCV provides a function named cv::seamlessClone()
 * Syntax:
 *	void cv::seamlessClone(InputArray src, InputArray dst, InputArray mask, Point p, OutputArray blend, int flag)
 * Parameters:
 *	- src - source image that will be cloned into the destination image.
 *	- dst - destination image into which the source image will be cloned.
 *	- mask - a rough mask around the object you want to clone. This should be the size of the source image.
 *	- center - location of the center of the source image in the destination image.
 *	- blend - result image.
 *	- flags - cloning method that could be cv::NORMAL_CLONE, cv::MIXED_CLONE, cv::MONOCHROME_TRANSFER.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Read a source image
	cv::Mat src{ cv::imread("../data/images/airplane.jpg") };

	// Read destination image
	cv::Mat dst{ cv::imread("../data/images/sky.jpg") };

	// Create a rough mask around the airplane
	cv::Mat src_mask{ cv::Mat::zeros(src.rows, src.cols, src.depth()) };

	// Define the mas as a closed polygon
	cv::Point poly[1][7];
	poly[0][0] = cv::Point(4, 80);
	poly[0][1] = cv::Point(30, 54);
	poly[0][2] = cv::Point(151, 63);
	poly[0][3] = cv::Point(254, 37);
	poly[0][4] = cv::Point(298, 90);
	poly[0][5] = cv::Point(272, 134);
	poly[0][6] = cv::Point(43, 122);

	const cv::Point* polygons[1] = {poly[0]};
	int num_points[] = { 7 };

	// Create mask by filling the polygon
	cv::fillPoly(src_mask, polygons, num_points, 1, cv::Scalar(255, 255, 255));

	// Show mask
	cv::imshow("Mask", src_mask);
	cv::waitKey(0);
	cv::destroyWindow("Mask");

	// Set the location of the center of the src in the dst
	cv::Point center(800, 100);

	// Seamless clone src into dst and put the results in output
	cv::Mat output;
	cv::seamlessClone(src, dst, src_mask, center, output, cv::NORMAL_CLONE);

	// Show image
	cv::imshow("Seamless", output);
	cv::waitKey(0);
	cv::destroyWindow("Seamless");

	return 0;
}