#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Path to the image
	std::string image_path{ "../data/images/musk.jpg" };

	// Load image from disk
	cv::Mat img{ cv::imread(image_path, cv::IMREAD_UNCHANGED) };

	// Get width and height of the image using structured bindings (since C++17)
	auto [width, height] = img.size();

	// Get number of channels of the image
	int channels = img.channels();

	std::cout << "Image width: " << width << ", height: " << height
		<< ", number of channels:" << channels;

	// Show the image
	cv::imshow("Musk", img);
	// Wait for key press
	cv::waitKey(0);
	// Clean up - destroy all windows
	cv::destroyAllWindows();

	return 0;
}