#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// path to an image
	std::string image_path{ "../data/images/number_zero.jpg" };

	// Load an image from disk
	cv::Mat img{ cv::imread(image_path, cv::IMREAD_COLOR) };

	cv::namedWindow("Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Image", 600, 600);
	cv::imshow("Image", img);
	cv::waitKey(0);
	cv::destroyWindow("Image");

	// Access color pixels
	std::cout << img.at<cv::Vec3b>(0, 0) << std::endl;

	// Modify pixels
	img.at<cv::Vec3b>(0, 0) = cv::Vec3b(0, 255, 255); // Set pixel at location (0, 0) to Yellow
	cv::namedWindow("Yellow", cv::WINDOW_NORMAL);
	cv::resizeWindow("Yellow", 600, 600);
	cv::imshow("Yellow", img);

	img.at<cv::Vec3b>(1, 1) = cv::Vec3b(255, 255, 0); // Set pixel at location (1, 1) to Cyan
	cv::namedWindow("Cyan", cv::WINDOW_NORMAL);
	cv::resizeWindow("Cyan", 600, 600);
	cv::imshow("Cyan", img);

	img.at<cv::Vec3b>(2, 2) = cv::Vec3b(255, 0, 255); // Set pixel at location (2, 2) to Magenta
	cv::namedWindow("Magenta", cv::WINDOW_NORMAL);
	cv::resizeWindow("Magenta", 600, 600);
	cv::imshow("Magenta", img);
	
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Modify Region of interest:
	// Set pixels at index [0, 3) row and index [0, 3) column to blue
	img(cv::Range(0, 3), cv::Range(0, 3)).setTo(cv::Scalar(255, 0, 0));
	// Set pixels at index [3, 6) row and index [0, 3) column to green
	img(cv::Range(3, 6), cv::Range(0, 3)).setTo(cv::Scalar(0, 255, 0));
	// Set pixels at index [6, 9) row and index [0, 3) column to red
	img(cv::Range(6, 9), cv::Range(0, 3)).setTo(cv::Scalar(0, 0, 255));
	cv::namedWindow("Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Image", 600, 600);
	cv::imshow("Image", img);
	cv::waitKey(0);
	cv::destroyWindow("Image");

	return 0;
}