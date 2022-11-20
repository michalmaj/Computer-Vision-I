#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main()
{
	// Read image:
	std::string image_path{ "../data/images/boy.jpg" };
	cv::Mat boy_img{ cv::imread(image_path, cv::IMREAD_COLOR) };
	cv::imshow("Boy", boy_img);
	cv::waitKey(0);
	cv::destroyWindow("Boy");

	// Create copy of an image
	cv::Mat img_copy{ boy_img.clone() }; // Create a new image by copying the already
										 // present image using the clone() method.
	cv::imshow("Copy", img_copy);
	cv::waitKey(0);
	cv::destroyWindow("Copy");

	// Create an empty matrix with size (height=100, width=200)
	cv::Mat empty_mat{ cv::Mat(100, 200, CV_8UC3, cv::Scalar(0, 0, 0)) }; // Create empty cv::Mat with zeros - black color
	cv::imshow("Black", empty_mat);
	cv::waitKey(0);
	cv::destroyWindow("Black");

	// Fill cv::Mat with (255, 255, 255) - white color
	empty_mat.setTo(cv::Scalar(255, 255, 255));
	cv::imshow("White", empty_mat);
	cv::waitKey(0);
	cv::destroyWindow("White");

	// Create new cv::Mat using properties of above cv::Mat (height=100, width=200) and type CV_8UC3
	cv::Mat empty_copy{ cv::Mat(empty_mat.size(), empty_mat.type(), cv::Scalar(100, 100, 100)) }; // Fill cv::Mat (100, 100, 100) - gray color
	cv::imshow("Gray", empty_copy);
	cv::waitKey(0);
	cv::destroyWindow("Gray");

	return 0;
}