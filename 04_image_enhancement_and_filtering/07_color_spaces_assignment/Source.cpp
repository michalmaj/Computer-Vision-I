/*
 * Implement from scratch two functions:
 *	1. convertBGRtoGray - function for converting BGR images to Grayscale image.
 *	2. convertBGRtoHSV - function for converting BGR image to HSV image;
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>
#include <vector>

void convertBGRtoGray(const cv::Mat& img, cv::Mat& output)
{
	// Create a copy the original image
	cv::Mat imClone{ img.clone() };

	// Get width and height of the original image
	auto [width, height] = img.size();

	// Convert clone image to float32 [0, 1]
	imClone.convertTo(imClone, CV_32F);
	cv::normalize(imClone, imClone, 0, 1, cv::NORM_MINMAX);

	// Set cv::Mat of the same height and width but as float32 [0, 1] with one dimension
	cv::Mat m { height, width, CV_32FC1, 0.0 };

	// Loop over rows
	for(int h{0}; h < height; ++h)
	{
		// Loop over columns
		for(int w{0}; w < width; ++w)
		{
			// Get value of Blue, Red and Green channels to separate variables
			float b{ imClone.at<cv::Vec3f>(h, w)[0] };
			float g{ imClone.at<cv::Vec3f>(h, w)[1] };
			float r{ imClone.at<cv::Vec3f>(h, w)[2] };

			// Perform a linear conversion from BGR to Grayscale (standard formula: 0.299r + 0.587g + 0.114b)
			// W can also use different formula: 0.2126 * r + 0.7152 * g + 0.0722 * b
			m.at<float>(h, w) = 0.299 * r + 0.587 * g + 0.114 * b;
		}
	}

	// Convert image to uchar
	cv::normalize(m, m, 0, 255, cv::NORM_MINMAX, CV_8UC1);

	output = m;
	
}

void convertBGRtoHSV(const cv::Mat& img, cv::Mat& output)
{
	// Clone the original image
	cv::Mat imClone{ img.clone() };

	// Get width and height of the original image
	auto [width, height] = img.size();

	// Convert clone image to float32 [0, 1]
	imClone.convertTo(imClone, CV_32F);
	cv::normalize(imClone, imClone, 0, 1, cv::NORM_MINMAX);

	// Create temporary cv::Mat to perform conversion to HSV color space
	cv::Mat hsv{ height, width, CV_32FC3, 0.0 };
	//hsv.at < cv::Vec3f>(0, 0)[0] = 0.1;
	//std::cout << hsv.at < cv::Vec3f>(0, 0)[0] << std::endl;
	// Perform algorithm to change BGR color space to HSV color space
	for(int h{0}; h < height; ++h)
	{
		for(int w{0}; w < width; ++w)
		{
			// Get Blue, Green, and Red values at given position
			double b{ imClone.at<cv::Vec3f>(h, w)[0] };
			double g{ imClone.at<cv::Vec3f>(h, w)[1] };
			double r{ imClone.at<cv::Vec3f>(h, w)[2] };

			// Set started Hue value as -1 and Saturation value as -1
			double hue{ -1 };
			double saturation{ -1 };

			// Calculate maximum of b, g, r
			double colorMax{ std::max(r, std::max(g, b)) };

			// Calculate minimum of b, g, r
			double colorMin{ std::min(r, std::min(g, b)) };

			// Calculate difference between maximum value and minimum value
			double diff{ colorMax - colorMin };

			// Compute Hue channel
			// If colorMax and colorMin are equal then Hue=0
			if (colorMax == colorMin)
				hue = 0;
			else if (colorMax == r)
				hue = std::fmod(30 * ((g - b) / diff) + 180, 180);
			else if (colorMax == g)
				hue = std::fmod(30 * ((b - r) / diff) + 60, 180);
			else if (colorMax == b)
				hue = std::fmod(30 * ((r - g) / diff) + 120, 180);

			// Compute Saturation channel
			if (colorMax == 0)
				saturation = 0;
			else
				saturation = (diff / colorMax) * 100;

			// Compute Value channel
			double value = colorMax * 100;

			// Put Hue, Saturation, and Value into corresponding channels
			hsv.at < cv::Vec3f>(h, w)[0] = hue; // Hue channel
			hsv.at < cv::Vec3f>(h, w)[1] = saturation; // Saturation channel
			hsv.at < cv::Vec3f>(h, w)[2] = value; // Value channel
 		}
	}

	// Normalize HSV from float32 to uchar
	// First we need to remember that Hue channel is represented in range [0, 179]
	// Crete separate channels to normalize Hue, Saturation, and Value channels separately
	std::vector<cv::Mat> hsvChannels(3);

	// Split channels
	cv::split(hsv, hsvChannels);

	// Normalize Hue in range [0, 179]
	cv::normalize(hsvChannels[0], hsvChannels[0], 0, 179, cv::NORM_MINMAX, CV_8U);

	// Normalize Saturation and Value channels in range [0, 255]
	cv::normalize(hsvChannels[1], hsvChannels[1], 0, 255, cv::NORM_MINMAX, CV_8U);
	cv::normalize(hsvChannels[2], hsvChannels[2], 0, 255, cv::NORM_MINMAX, CV_8U);

	// Merge HSV channels
	cv::merge(hsvChannels, hsv);

	output = hsv;

}


int main()
{
	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/girl.jpg") };

	// Output image in grayscale
	cv::Mat gray;
	cv::Mat grayOpenCV;

	convertBGRtoGray(image, gray);
	cv::cvtColor(image, grayOpenCV, cv::COLOR_BGR2GRAY);

	cv::imshow("Grayscale", gray);
	cv::imshow("Grayscale OpenCV", grayOpenCV);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Output image in HSV color space
	cv::Mat hsv;
	cv::Mat hsvOpenCV;
	
	convertBGRtoHSV(image, hsv);
	cv::cvtColor(image, hsvOpenCV, cv::COLOR_BGR2HSV);

	cv::imshow("HSV", hsv);
	cv::imshow("HSV OpenCV", hsvOpenCV);
	cv::waitKey(0);
	cv::destroyAllWindows();
	

	return 0;
}