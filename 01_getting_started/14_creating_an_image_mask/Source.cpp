/*
 * Masking is very important step in many image processing and computer vision algorithms.
 * It allows us to segment out the area of interest and apply our algorithms to a specific
 * region of the image. We can perform all operations on this segmented part and then put
 * it back on the original image.
 */
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	// Read image
	cv::Mat boy{ cv::imread("../data/images/boy.jpg") };

	// Create an empty mask
	cv::Mat mask1{ cv::Mat::zeros(boy.size(), boy.type()) };
	cv::imshow("Mask1", mask1);
	cv::waitKey(0);
	cv::destroyWindow("Mask1");

	// set pixels in row range (height) [50, 200] and column range (width) [170, 320] to 255 (white color)
	mask1(cv::Range(50, 200), cv::Range(170, 320)).setTo(255);
	cv::imshow("Mask1", mask1);
	cv::waitKey(0);
	cv::destroyWindow("Mask1");

	// Create a mask using pixel intensity or color
	/*
	 * There is another way to create masks - we have to use some logic.One example is using a simple color
	 * information. For example let's find out the pixels which are approximately red in color. The trick is:
	 *	- The red channel should have high intensity (keep the range of pixel values from 150 to 255)
	 *	- The other 2 channels should have low intensity (keep the range of pixel values in Blue and
	 *	  Green channels between 0 to 100.
	 *
	 * Function syntax (cv::inRange)
	 * void cv::inRange(InputArray src, InputArray lowerb, InputArray upperb, OutputArray dst)
	 * where:
	 *	src - input array
	 *	lowerb - inclusive lower boundary array or scalar
	 *	upperb - inclusive upper boundary array or scalar
	 *	dst - output array of the same size as scr and CV_*U type
	 */

	cv::Mat mask2;
	cv::inRange(boy, cv::Scalar(0, 0, 150), cv::Scalar(100, 100, 255), mask2);

	cv::imshow("Boy", boy);
	cv::imshow("Mask2", mask2);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}