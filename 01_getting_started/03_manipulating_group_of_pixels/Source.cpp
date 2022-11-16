/*
 * Group of pixels (roi -region of interest).
 * Get roi from image in Range(start_row, end_row) Range(start_column, end_column). 
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

int main()
{
	// Path to the image
	std::string image_path{ "../data/images/number_zero.jpg" };

	// Read the image in grayscale
	cv::Mat img = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	// Get roi of the image (row 0 and 1, col 0, 1, 2, and 3)
	cv::Mat roi = img(cv::Range(0, 2), cv::Range(0, 4));

	std::cout << "Image:\n" << img << std::endl << std::endl;
	std::cout << "Roi:\n" << roi << std::endl << std::endl;

	// Modifying all values in given range:
	img(cv::Range(0, 2), cv::Range(0, 4)).setTo(111);

	std::cout << "Modified image:\n" << img << std::endl;

	return 0;
}