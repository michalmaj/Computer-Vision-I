/*
 * Convolution:
 * The most basic type of image filtering is called Convolution. It is the basis of all linear filters. If we want to blur an image, we
 * use convolution. If we want to sharpen an image, we use another convolution kernel. If we want to detect edges, convolution in the
 * first step. The same convolution is used in state-of-the-art neural networks.
 *
 * What is convolution?
 * It is an operation that requires two inputs:
 *	1. An input image (for color images, convolution is performed on R, G, B channels separately.
 *	2. A convolution kernel. This is a small matrix of numbers. For example a 3x3 convolution kernel look like this:
 *				{
 *					{0, 1, 3},
 *					{0.1, -1. 1.5},
 *					{4, 3, 1.7},
 *				}
 *
 * The output is a filtered image. Depending on the implementation of convolution operation, the size of filtered image is
 * either equal to or slightly smaller than the original image.
 *
 * Convolution in OpenCV:
 * OpenCV library provides  a function to perform convolution: filter2D()
 * Syntax:
 *	void cv::filter2D(InputArray src, OutputArray dst, int ddepth, InputArray kernel, Point anchor=Point(-1,-1),
 *					  double delta=0, int borderType=BORDER_DEFAULT)
 * Parameters:
 *	- src - input image;
 *	- dst - output image of the same size and the same number of channels as src;
 *	- ddepth - desired depth of the destination image, if it is set to -1, bit-depth of the output image is the same as the input image;
 *	- kernel - convolution kernel, a single-channel floating point matrix;
 *	- anchor - anchor of the kernel that indicates the relative position of a filtered point within the kernel; the anchor should
 *			   lie within the kernel; default value (-1, -1) means that the anchor is at the kernel center;
 *	- delta - optional value added to the filtered pixels before storing them in dst;
 *	- borderType - pixel extrapolation method.
 *
 * BoundaryConditions:
 * We have a four options  to defined what will happen at the boundary:
 *	1. Ignore the boundary pixels - the output image will be slightly smaller than the input image.
 *	2. Zero padding - we can pad the input image with zeros at the boundary pixels th make it larger and the perform convolution.
 *	3. Replicate border - replicate the boundary pixels of the input image and then perform convolution on larger image.
 *	4. Reflect border - reflect the border about the boundary. Reflecting ensures a smooth intensity transition of pixels at the boundary.
 *
 * By default OpenCV uses border type: BORDER_REFLECT_101 which is the same as option 4 - this is preferred option.
 *
 * Attention on kernel creation: 
 * When we create a kernel we want all the elements to add up to 1. This ensures that the output image has the same brightness level as the
 * input image.
 */

#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	// Load an image from disk
	cv::Mat img{ cv::imread("../data/images/sample.jpg") };

	// Check to see if we loaded the image properly
	if(img.empty())
	{
		std::cout << "Could not read image" << std::endl;
		return -1;
	}

	// Define the kernel, let's create a kernel size of 5x5. We will also choose a kernel where all elements are the same. This kernel is
	// often called a Box Kernel.

	// Set kernel size to 5
	int kernelSize{ 5 };

	// Create a 5x5 kernel with all elements equal to 1
	cv::Mat kernel{ cv::Mat::ones(kernelSize, kernelSize, CV_32F) };

	// Normalize kernel so sum of all elements equals 1
	kernel = kernel / static_cast<float>(kernelSize * kernelSize);

	// Print kernel
	std::cout << kernel;
	std::cout << std::endl;

	// Calculate sum of elements in kernel
	auto result = std::accumulate(kernel.begin<float>(), kernel.end<float>(), 0.0f);
	std::cout << "Sum of kernel elements: " << result << std::endl;

	// Perform convolution

	// Output image
	cv::Mat filtered;

	// Apply filter
	cv::filter2D(img, filtered, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

	// Show images
	cv::imshow("Original image", img);
	cv::imshow("Convolution Result", filtered);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Homework:

	// 1. Try without normalizing
	kernel = cv::Mat::ones(kernelSize, kernelSize, CV_8U);

	// Apply filter
	cv::filter2D(img, filtered, -1, kernel);

	// Show images
	cv::imshow("Original image", img);
	cv::imshow("Convolution Result", filtered); // Image will be very bright
	cv::waitKey(0);
	cv::destroyAllWindows();

	// 2. Change the values in the kernel and see if you can make sense out of the output

	// Create own (this time 3x3) kernel
	cv::Matx33d kernel1 (-1, 0, 1
		- 2, 0, 2
		- 1, 0, 1);

	// Apply filter
	cv::filter2D(img, filtered, -1, kernel1);

	// Show images
	cv::imshow("Original image", img);
	cv::imshow("Convolution Result", filtered); // We will get edges
	cv::waitKey(0);
	cv::destroyAllWindows();




	return 0;
}