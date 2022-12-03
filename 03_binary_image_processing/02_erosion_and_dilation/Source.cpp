/*
 * Morphological operations:
 * In OpenCV we can use two basic morphological operations:
 *  - dilatation - is used to merge or expand white regions which may be close to each other
 *	- erosion - is used to separate or shrink white regions.
 *
 * The library contains two function cv::dilate and cv::erode
 * Syntax:
 *	void cv::dilate(InputArray src, OutputArray dst, InputArray kernel, Point anchor=(-1, -1), int iterations=-1, 
 *					int borderType = BORDER_CONSTANT, const Scaler& borderValue=morphologyDefaultBorderValue)
 *
 *	void cv::erode(InputArray src, OutputArray dst, InputArray kernel, Point anchor=(-1, -1), int iterations=-1, 
 *				   int borderType = BORDER_CONSTANT, const Scaler& borderValue=morphologyDefaultBorderValue)
 *
 * Parameters:
 *	- src - input image, the number of channels can be arbitrary, but the depth should be one of CV_8U, CV_16U,
 *			CV_16S, CV_32F or CV_64F;
 *	- dst - output image of the same size and type as src;
 *	- kernel - structuring element used for dilatation/erosion, if element=Mat(), a 3x3 rectangular structuring element is used;
 *	- anchor - position of the anchor within the element; default value (-1, -1) means that the anchor is at the element center;
 *	- iterations - number of times dilation is applied;
 *	- borderType - pixel extrapolation method;
 *	- borderValue - border value in case of constant border.
 *
 * For dilatation an erosion we will use check out two ways of performing morphological operation:
 *	1. Using a single big kernel.
 *	2. Using multiple iterations of smaller kernel.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load image from disk for dilatation
	cv::Mat image{ cv::imread("../data/images/dilation_example.jpg", cv::IMREAD_COLOR) };

	cv::imshow("Image", image);
	cv::waitKey(0);
	cv::destroyWindow("Image");

	// Dilatation operation - using a single big kernel
	int kSize = 7;
	cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));

	// Show the kernel
	cv::namedWindow("Big kernel", cv::WINDOW_NORMAL);
	cv::resizeWindow("Big kernel", 250, 250);
	cv::imshow("Big kernel", kernel1 * 255);
	cv::waitKey(0);
	cv::destroyWindow("Big kernel");

	cv::Mat imageDilated;
	cv::dilate(image, imageDilated, kernel1);

	cv::imshow("Original", image);
	cv::imshow("Dilated Image", imageDilated);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Dilation using smaller kernel with multiple iterations
	kSize = 3;
	cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kSize, kSize));

	// Show the kernel
	cv::namedWindow("Small Kernel", cv::WINDOW_NORMAL);
	cv::resizeWindow("Small Kernel", 250, 250);
	cv::imshow("Small Kernel", kernel2 * 255);
	cv::waitKey(0);
	cv::destroyWindow("Small Kernel");

	cv::Mat imageDilated1, imageDilated2;
	cv::dilate(image, imageDilated1, kernel2, cv::Point(-1, -1), 1);
	cv::dilate(image, imageDilated2, kernel2, cv::Point(-1, -1), 2);

	cv::imshow("Original", image);
	cv::imshow("Dilated Image, Iteration 1", imageDilated1);
	cv::imshow("Dilated Image, Iteration 2", imageDilated2);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Load image from disk for erosion
	image = cv::imread("../data/images/erosion_example.jpg", cv::IMREAD_COLOR);

	cv::imshow("Image", image);
	cv::waitKey(0);
	cv::destroyWindow("Image");

	// Erosion using big kernel
	cv::Mat imageEroded;

	cv::erode(image, imageEroded, kernel1);

	cv::imshow("Original", image);
	cv::imshow("Eroded Image", imageEroded);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Erosion using smaller kernel with multiple iterations
	cv::Mat imageEroded1, imageEroded2;

	cv::erode(image, imageEroded1, kernel2, cv::Point(-1, -1), 1);
	cv::erode(image, imageEroded2, kernel2, cv::Point(-1, -1), 2);

	cv::imshow("Original", image);
	cv::imshow("Eroded Image, Iteration 1", imageEroded1);
	cv::imshow("Eroded Image, Iteration 2", imageEroded2);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}