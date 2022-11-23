/*
 * When we converting from one data type to another we have to scale it to correct format.
 * For example if we won't use some scaling factor (i.e., CV_32F), we will ended up with cv::Mat with
 * type of CV_32F and values in range [0, 255], where it should be in the range [0, 1]. Hence we need to
 * divide the matrix by 255.
 * Syntax:
 * scr.convertTo(dst, dataType, scaleFactor);
 * where:
 *	- src - source image;
 *	- dst - destination image;
 *	- dataType - CV_32F (in this case);
 *	- scaleFactor - 1 / 255.0 (in this case)
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	// read the image from disk
	cv::Mat boy{ cv::imread("../data/images/boy.jpg") };

	// Check image data type
	std::cout << "Image Data Type: " << boy.type() << std::endl; // 16 means data type of the image
																 // is 8 bit unsigned char and it has 3 channel
	std::cout << "CV_8U:\n" << boy(cv::Range(0, 4), cv::Range(0, 4));
	std::cout << std::endl;

	// Display the image
	cv::imshow("Boy", boy);
	cv::waitKey(0);
	cv::destroyWindow("Boy");

	// Convert to CV_16U
	// It will convert from unit8 to uint16
	cv::Mat boy_16U = boy.clone();
	unsigned char scaling_factor{ 255 };
	boy_16U.convertTo(boy_16U, CV_16U, scaling_factor);

	std::cout << "CV_16U:\n" << boy_16U(cv::Range(0, 4), cv::Range(0, 4));
	std::cout << std::endl;

	// Display the image
	cv::imshow("Boy_16U", boy_16U);
	cv::waitKey(0);
	cv::destroyWindow("Boy_16U");

	// Convert to CV_32F
	cv::Mat boy_32F = boy.clone();
	double scaling_factor_double{ 1 / 255.0 };
	boy_32F.convertTo(boy_32F, CV_32F, scaling_factor_double);

	std::cout << "CV_32F:\n" << boy_32F(cv::Range(0, 4), cv::Range(0, 4));
	std::cout << std::endl;

	// Display the image
	cv::imshow("Boy_32F", boy_32F);
	cv::waitKey(0);
	cv::destroyWindow("Boy_32F");

	return 0;
}