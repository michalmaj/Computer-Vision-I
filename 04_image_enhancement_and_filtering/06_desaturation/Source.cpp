/*
 * Desaturation Filter:
 * Photo editing applications like photoshop or instagram use many different kinds of image enhancement techniques to make the
 * images look special. One such image enhancement technique is desaturation. We desaturate the image be decreasing the
 * values in the Saturation channel in HSV color space. This result in an image which looks faded or washed out, with no colors.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

int main()
{
	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/girl.jpg") };

	// Specify scaling factor - desaturate a given image using the S channel of the HSV color space
	double saturationScale{ 0.01 };

	// Convert image to HSV color space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

	// Split HSV image into three channels
	std::vector<cv::Mat> hsvChannels(3);
	cv::split(hsv, hsvChannels);

	// Convert image to float32
	// First get min, and max values to normalize after changing
	double min, max;
	cv::minMaxIdx(hsvChannels[1], &min, &max, nullptr, nullptr);
	std::cout << "Started minimum Saturation value: " << min << ", maximum value: " << max << std::endl;

	// Convert to float32 only Saturation channel, because we will modify only this channel
	hsvChannels[1].convertTo(hsvChannels[1], CV_32F);

	hsvChannels[1] *= saturationScale;

	// Check how min and max value changes
	double minAfter, maxAfter;
	cv::minMaxIdx(hsvChannels[1], &minAfter, &maxAfter, nullptr, nullptr);
	std::cout << "After changes minimum value: " << minAfter << ", maximum value: " << maxAfter << std::endl;

	// Clipping operation performed to limit pixel values 
	// between 0 and 255
	cv::min(hsvChannels[1], max, hsvChannels[1]);
	cv::max(hsvChannels[1], min, hsvChannels[1]);

	// Convert back from float32 to uchar
	hsvChannels[1].convertTo(hsvChannels[1], CV_8UC1);

	// Merge channels
	cv::merge(hsvChannels, hsv);

	// Prepare saturated image
	cv::Mat imSat;

	// Convert from HSV to BGR
	cv::cvtColor(hsv, imSat, cv::COLOR_HSV2BGR);

	cv::imshow("Original Image", image);
	cv::imshow("Saturated Image", imSat);
	cv::waitKey(0);
	cv::destroyAllWindows();
	


	return 0;
}