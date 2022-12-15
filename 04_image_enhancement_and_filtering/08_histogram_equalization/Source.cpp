/*
 * Histogram Equalization:
 * Histogram equalization is a non-linear method for enhancing contrast in an image. OpenCV provides equalizeHist() function.
 * Syntax:
 *	void cv::equalizeHist(InputArray src, OutputArray dst)
 * Parameters:
 *	- src - source 8-bit single channel image
 *	- dst - destination image of the same size and type as src.
 *
 * Histogram Equalization for Color Images:
 * The right way to perform histogram equalization on color images is to transform the image to HSV color space, where
 * colors/hue/tint is separated from the intensity.
 * There are three steps involved:
 *	1. Transform image to HSV color space.
 *	2. Perform histogram equalization only on the Value (V) channel.
 *	3. Transform the image back to BGR color space. 
 */

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <matplot/matplot.h>

/**
 * \brief Function to show Histogram
 * \param imgGray Single channel 8-bit image
 * \param numBins Number of bins
 */
void showHistogram(const cv::Mat& imgGray, int numBins=0)
{
	// Create a std::vector to store image as uchar
	std::vector<uchar> hist{ imgGray.begin<uchar>(), imgGray.end<uchar>() };

	// Create histogram of given image
	auto h{ matplot::hist(hist) };

	// If numBins provided use this value
	if (numBins > 0)
		h->num_bins(numBins);

	// Show image
	matplot::show();
}

int main()
{
	// 1. Histogram Equalization for grayscale images
	// Load an image from disk in grayscale
	cv::Mat img{ cv::imread("../data/images/dark-flowers.jpg", cv::IMREAD_GRAYSCALE) };

	// Equalize histogram
	cv::Mat imEq;

	cv::equalizeHist(img, imEq);

	// Show original image and image after equalization
	cv::imshow("Original Image", img);
	cv::imshow("Histogram Equalized", imEq);
	cv::waitKey(0);
	cv::destroyAllWindows();

	//// Histogram for original image
	//showHistogram(img, 9);

	//// Histogram for equalized histogram image
	//showHistogram(imEq);

	// Show histograms side by side
	std::vector<uchar> imgHist{ img.begin<uchar>(), img.end<uchar>() };
	std::vector<uchar> imgHistEq{ imEq.begin<uchar>(), imEq.end<uchar>() };
	matplot::subplot(1, 2, 0);
	matplot::hist(imgHist, 9);
	matplot::subplot(1, 2, 1);
	matplot::hist(imgHistEq, 10);
	matplot::show();

	// 2. Histogram equalization for color images

	// 2a. Wrong way

	// Load image from disk.
	img = cv::imread("../data/images/dark-flowers.jpg");
	imEq = img.clone();

	// Create std::vector of cv::Mat type to separate each channel of the image
	std::vector<cv::Mat> imgChannels(3);
	std::vector<cv::Mat> imEqChannels(3);

	// Split both images into separate channels
	cv::split(img, imgChannels);
	cv::split(imEq, imEqChannels);

	// Perform histogram equalization on each channel separately
	for (size_t i{ 0 }; i < imgChannels.size(); ++i)
		cv::equalizeHist(imgChannels.at(i), imEqChannels.at(i));

	// Merge images back
	cv::merge(imgChannels, img);
	cv::merge(imEqChannels, imEq);

	// Show images
	cv::imshow("Original Image", img);
	cv::imshow("Histogram Equalized", imEq);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// 2b. Right way

	// Load an image from disk
	img = img = cv::imread("../data/images/dark-flowers.jpg");

	// Convert image from BGR to HSV color space
	cv::Mat hsv;

	cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

	// Split HSV image into three channels
	std::vector<cv::Mat> hsvChannels(3);
	cv::split(hsv, hsvChannels);

	// Perform histogram equalization only on Value (V) channel
	cv::equalizeHist(hsvChannels.at(2), hsvChannels.at(2));

	// Merge back HSV image
	cv::merge(hsvChannels, hsv);

	// Convert back HSV image to BGR format
	cv::cvtColor(hsv, imEq, cv::COLOR_HSV2BGR);

	// Show images
	cv::imshow("Original Image", img);
	cv::imshow("Histogram Equalized", imEq);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}