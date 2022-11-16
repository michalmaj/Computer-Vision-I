/*
 * Grayscale image is a 2D array.
 *
 * Indexing is in [row, column] version. So (1, 0) means 2nd row and 1st column.
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

int main()
{
	// path to the image
	std::string image_path{ "../data/images/number_zero.jpg" };

	// read image in grayscale
	cv::Mat img = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	// Value of pixel at index (0, 0)
	std::cout << "Value of pixel at index (0, 0): " << static_cast<int>(img.at<uchar>(0, 0)) << std::endl;

	std::cout << img;

	std::cout << "\n=====================================\n" << std::endl;

	// Modifying pixel at 2nd row and 1st column
	img.at<uchar>(1, 0) = 200;
	std::cout << img;


	return 0;
}