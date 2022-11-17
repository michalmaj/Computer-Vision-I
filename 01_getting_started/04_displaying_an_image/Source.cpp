#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Path to the image
	std::string image_path{ "../data/images/boy.jpg" };

	// Read an image
	cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

	int width = img.size().width;
	int height = img.size().height;
	int channels = img.channels();
	

	std::cout << "Resolution of the image: " << width << "x" << height << ", number of channels: " << channels << std::endl;

	// We can change resolution of the output window
	cv::namedWindow("Image", cv::WINDOW_NORMAL);
	cv::resizeWindow("Image", 640, 480);

	// Show the image using cv::imshow(std::string window_name, input_array cv::Mat)
	cv::imshow("Image", img);

	// Wait for user to press a key
	cv::waitKey(0);

	// Cleanup and destroy all windows
	cv::destroyAllWindows();

	return 0;
}