/*
 * Box Blur:
 * A very common use case for convolution is blurring or smoothing or low pass filtering. It reduces the noise in the image.
 * The noise in an image can be due to external factors like poor lightning or environment or internal factors like a few pixels
 * in the sensor may have gone bad. We can use image blurring/smoothing techniques to get rid of different types of noise.
 * OpenCV library provides a blur() function for blurring images.
 * Syntax:
 *	void cv::blur(InputArray src, OutputArray dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT)
 * Parameters:
 *	- src - input image; it can have any number of channel, which are processed independently, but the depth should be:
 *			CV_8U, CV_16U, CV_16S, CV_32F or CV_64F.
 *	- dst - output image of the same size and type as src.
 *	- ksize - blurring kernel size.
 *	- anchor - anchor point; default value Point(-1,-1) means that the anchor is at kernel center.
 *	-borderType - border mode used to extrapolate pixels outside of the image.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/gaussian-noise.png") };

	// Apply box filtering with two kernels of sizes 3 and 7 with the blur() function.
	cv::Mat dst1, dst2;

	// Box filter - kernel size 3
	cv::blur(image, dst1, cv::Size(3, 3));

	// Box filter - kernel size 7
	cv::blur(image, dst2, cv::Size(7, 7));

	// Show images
	// Noise can be reduced by increasing the kernel size, but at the same time,
	// the images becomes more and more blurry.
	cv::imshow("Original Image", image);
	cv::imshow("Box blur Result1: kernel size = 3", dst1);
	cv::imshow("Box blur Result2: kernel size = 7", dst2);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}