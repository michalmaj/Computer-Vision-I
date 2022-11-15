/*
 * Reading image from disk using OpenCV
 * cv::Mat image = cv::imread(file_name, flag);
 * For flag (it's an optional argument) we can use:
 *	cv::IMREAD_GRAYSCALE or 0
 *	cv::IMREAD_COLOR or 1
 *	cv::IMREAD_UNCHANGED or -1
 *
 * In C++ version of OpenCV size of Mat is stored as [#columns, #rows] (width, height)
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

/**
 * \brief Function for identifying OpenCV matrix (cv::Mat) at runtime.
 * \param type value returned from read image type() method.
 * \return std::string that contains information about cv::Mat type
 */
std::string type2str(int type)
{
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar channels = 1 + (type >> CV_CN_SHIFT);

	switch (depth)
	{
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	// Add number of channels
	r += "C";
	r += (channels + '0');

	return r;
}

int main()
{
	// String for path to the image stored on a hard drive
	std::string image_path{ "../data/images/number_zero.jpg" };
	// Read image as Mat object
	cv::Mat img = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	std::cout << img;
	std::cout << std::endl;

	// We can also get properties of the image
	// For example: data type
	std::cout << "Data type: " << type2str(img.type()) << std::endl;

	// We can also get a shape
	std::cout << "Image dimension" << img.size() << std::endl;

	// W can divide to width and height:
	auto width = img.size().width;
	auto height = img.size().height;

	std::cout << "Image width: " << width << ", image height: " << height << std::endl;


	return 0;
}