/*
 * Thresholding:
 * Many computer vision tasks require to process a binary image.
 * A binary image consists of pixels that are either black (pixel has a value = 0) or white (pixel has value = 255).
 * It is easier to come up with algorithms which work on binary images.
 *
 * Threshold function in OpenCV:
 *	cv::threshold()
 * Syntax:
 *	double cv::threshold(InputArray src, OutputArray dst, double thresh, double maxval, int type)
 *
 * Parameters:
 *	 - src - input array of image (it can be multiple channel, 8-bit or 32-bit floating point).
 *	 - dst - output array contains binary image.
 *	 - thresh - the threshold value.
 *	 - maxval - the maximum value.
 *	 - type - the threshold type (eg. cv::THRESH_BINARY, cv::THRESH_BINARY_INV, etc)
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <chrono>
#include <iostream>


// Custom function for threshold a given image
/**
 * \brief Own implementation of function to threshold given image. 
 * \param src Input image.
 * \param dst Output image (after binarization).
 * \param thresh Value of thresh.
 * \param maxValue The maximum value to use to thesholding the image.
 */
void thresholdingUsingForLoop(const cv::Mat& src, cv::Mat& dst, int thresh, int maxValue)
{
	auto [width, height] = src.size();

	// Loop over rows
	for(int i{0}; i < height; ++i)
	{
		// Loop over columns
		for(int j{0}; j < width; ++j)
		{
			if (src.at<uchar>(i, j) > thresh)
				dst.at<uchar>(i, j) = maxValue;
			else
				dst.at<uchar>(i, j) = 0;
		}
	}
}


int main()
{
	// Load an image from disk
	cv::Mat src{ cv::imread("../data/images/threshold.png", cv::IMREAD_GRAYSCALE) };

	// Show the original image
	cv::imshow("Original", src);
	cv::waitKey(0);
	cv::destroyWindow("Original");

	// Set values for threshold and maximum value
	int thresh{ 100 };
	int maxValue{ 255 };

	// Usage of own implementation
	cv::Mat dst = src.clone();

	// Check the execution time of the function
	auto start = std::chrono::high_resolution_clock::now();
	thresholdingUsingForLoop(src, dst, thresh, maxValue);
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Time taken by custom function: " << duration << " in microseconds" << std::endl;

	// Show both images
	cv::imshow("Original Image", src);
	cv::imshow("Thresholded Image", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Check execution time for built-in function
	start = std::chrono::high_resolution_clock::now();
	cv::threshold(src, dst, thresh, maxValue, cv::THRESH_BINARY);
	stop = std::chrono::high_resolution_clock::now();

	duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "Time taken by built-in function: " << duration << " in microseconds" << std::endl;

	// Show both images
	cv::imshow("Original Image", src);
	cv::imshow("Thresholded Image", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Different threshold types
	thresh = 100;
	maxValue = 150;

	// 1. Threshold binary:
	//tex:
	// $$dst(x,y)=\begin{cases} maxval, & \mbox{if $src(x,y)>thresh$}.\\ 0, & \mbox{otherwise}.\end{cases}$$
	cv::Mat dst_bin;
	cv::threshold(src, dst_bin, thresh, maxValue, cv::THRESH_BINARY);

	// 2. Inverse Binary Threshold
	//tex:
	// $$dst(x,y)=\begin{cases} 0, & \mbox{if $src(x,y)>thresh$}.\\ maxval, & \mbox{otherwise}.\end{cases}$$
	cv::Mat dst_bin_inv;
	cv::threshold(src, dst_bin_inv, thresh, maxValue, cv::THRESH_BINARY_INV);

	// 3. Truncate threshold
	//tex:
	// $$dst(x,y)=\begin{cases} threshold, & \mbox{if $src(x,y)>thresh$}.\\ src(x, y), & \mbox{otherwise}.\end{cases}$$
	cv::Mat dst_trunc;
	cv::threshold(src, dst_trunc, thresh, maxValue, cv::THRESH_TRUNC);

	// 4. Threshold to zero
	//tex:
	// $$dst(x,y)=\begin{cases} src(x, y), & \mbox{if $src(x,y)>thresh$}.\\ 0, & \mbox{otherwise}.\end{cases}$$
	cv::Mat dst_to_zero;
	cv::threshold(src, dst_to_zero, thresh, maxValue, cv::THRESH_TOZERO);

	//tex:
	// $$dst(x,y)=\begin{cases} 0, & \mbox{if $src(x,y)>thresh$}.\\ src(x, y), & \mbox{otherwise}.\end{cases}$$
	cv::Mat dst_to_zero_inv;
	cv::threshold(src, dst_to_zero_inv, thresh, maxValue, cv::THRESH_TOZERO_INV);

	cv::imshow("Binary Threshold", dst_bin);
	cv::imshow("Binary Threshold Inverse", dst_bin_inv);
	cv::imshow("Truncate Threshold", dst_trunc);
	cv::imshow("Threshold to Zero", dst_to_zero);
	cv::imshow("Threshold to Zero Inverse", dst_to_zero_inv);
	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}