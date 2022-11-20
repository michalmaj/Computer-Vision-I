#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	// Read an image
	std::string image_path{ "../data/images/boy.jpg" }; // Path to the image
	cv::Mat boy{ cv::imread(image_path, cv::IMREAD_UNCHANGED) }; // Load the image from disk
	cv::imshow("Boy", boy);
	cv::waitKey(0);
	cv::destroyWindow("Boy");

	// Crop the image between row [50, 200] and between column [170, 320]
	cv::Mat crop{ boy(cv::Range(50, 200), cv::Range(170, 340)) };
	cv::imshow("Cropped", crop);
	cv::waitKey(0);
	cv::destroyWindow("Cropped");


	return 0;
}


