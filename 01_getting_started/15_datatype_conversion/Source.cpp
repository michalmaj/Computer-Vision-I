/*
 * For reading an image using cv::imread function our data type is unsigned integer (8 bits)
 * For most arithmetic operations like multiplication or division, we need to convert the array
 * type to flating point which each pixels having 16, 32 or 64 bits of data.
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Read the image from disk
	cv::Mat boy{ cv::imread("../data/images/boy.jpg") };
	std::cout << "Image contains value in range [0, 255] before scaling: " << boy(cv::Range(0, 1), cv::Range(0, 1)) << std::endl;

	// Converting image to float32 with 3 channels
	double scaling_factor{ 1 / 255.0 };
	double shift{ 0 };

	// converting from unsigned char to float(32bit)
	/*
	 * The scale of scaling factor reduce  the range of [0, 255] to [0, 1], hence all the floating point numbers now range between 0-1.
	 * When we convert back to unsigned char we use the reciprocal of the scaling factor and the range is converted from 0-1 to 0-255.
	 */
	boy.convertTo(boy, CV_32FC3, scaling_factor, shift);
	std::cout << "Image contains value in range [0, 1] after scaling: " << boy(cv::Range(0, 1), cv::Range(0, 1)) << std::endl;

	cv::imshow("Boy after scaling", boy);
	cv::waitKey(0);
	cv::destroyWindow("Boy after scaling");

	// Converting from float to unsigned char
	boy.convertTo(boy, CV_8UC3, 1.0 / scaling_factor, -shift / scaling_factor);
	std::cout << "Image contains value in range [0, 1] scaling back: " << boy(cv::Range(0, 1), cv::Range(0, 1)) << std::endl;

	cv::imshow("Boy scaling back", boy);
	cv::waitKey(0);
	cv::destroyWindow("Boy scaling back");

	return 0;
}