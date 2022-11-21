/*
 * For resizing an image we can use cv::resize function.
 * Syntax:
 *	void cv::resize(InputArray src, OutputArray dst, Size dsize, double fx=0, double fy=0, int interpolation=cv::INTER_LINEAR)
 *		- src - input image
 *		- dst - output resized image
 *		- dsize - output image size (Either dsize or both fx and fy must be non-zero.)
 *		- fx - scale factor along the horizontal axis (Either dsize or both fx and fy must be non-zero.)
 *		- fy - scale factor along the vertical axis (Either dsize or both fx and fy must be non-zero.)
 *		- interpolation - interpolation method (see documentation: https://docs.opencv.org/4.1.0/da/d54/group__imgproc__transform.html#ga5bb5a1fea74ea38e1a5445ca803ff121)
 *
 * There are two methods to resize an image:
 *	1. Specify width and height of output image explicitly:
 *		Use the dsize argument to specify the output size in the form (width, height)
 *	2. Specify the scaling factors for resizing (for both width and height)
 *		Use fx and fy arguments to specify the scaling factor for horizontal and vertical axis respectively (It should be float).
 *		The output size is calculated as cv::Size(cv::round(fx*src.cols), cv::round(fy*src.rows))
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <string>

int main()
{
	// Read image
	std::string image_path{ "../data/images/boy.jpg" };

	// Load image from disk
	cv::Mat boy{ cv::imread(image_path, cv::IMREAD_COLOR) };

	// Method 1 - specify width and height
	// In this method we have to manually keep track of the aspect ratio of the output image - this method won't preserve
	// the aspect ratio between input and output image
	int resize_down_width{ 300 }; // set new width to 300
	int resize_down_height{ 200 }; // set new height to 200
	cv::Mat resized_down; // cv::Mat to store resized image
	cv::Mat resized_down_lanczos; // cv::Mat to store resized image (cv::INTER_LANCZOS4 method)
	cv::resize(boy, resized_down, cv::Size(resize_down_width, resize_down_height), cv::INTER_LINEAR); // Most popular interpolation method
	cv::resize(boy, resized_down_lanczos, cv::Size(resize_down_width, resize_down_height), cv::INTER_LANCZOS4); // Another interpolation method
	cv::imshow("Original", boy);
	cv::imshow("Resized Linear", resized_down);
	cv::imshow("Resized Lanczos4", resized_down_lanczos);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Mess up with the aspect ratio
	int resize_up_width = 600;
	int resized_up_height = 900;
	cv::Mat resized_up;
	cv::resize(boy, resized_up, cv::Size(resize_up_width, resized_up_height), cv::INTER_LINEAR);
	cv::imshow("Original", boy);
	cv::imshow("Resized Linear", resized_up);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Method 2 - specify scaling factor
	// This method is useful when we want to preserve the aspect ratio of the image. For example, say we have an image of size (400x600)
	// and we want the width to be 360. Then, instead of specifying the height, we can find the scaling factor (i.e., 360/600 = 0.6).

	// Scaling down the image
	double scale_down = 0.6;

	// Scaling up the image
	double scale_up = 1.5;

	cv::Mat scaled_down, scaled_up;
	cv::resize(boy, scaled_down, cv::Size(), scale_down, scale_down, cv::INTER_LINEAR);
	cv::resize(boy, scaled_up, cv::Size(), scale_up, scale_up, cv::INTER_LINEAR);

	cv::imshow("Scaled down", scaled_down);
	cv::imshow("Scaled up", scaled_up);
	cv::waitKey(0);
	cv::destroyAllWindows();

	cv::imwrite("scaled_down.png", scale_down);
	cv::imwrite("scaled_up.png", scaled_up);




	return 0;

}