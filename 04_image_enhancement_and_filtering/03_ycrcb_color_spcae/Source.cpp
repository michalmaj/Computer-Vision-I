/*
 * The YCrCb color space:
 * The YCrCb colr space is derived from RGB color space. There are three components:
 *	- Y - luma, derived from the RGB values of the image;
 *	- Cr - R-Y, how far is the red component from luma (also known as Red difference)
 *	- Cb - B-Y, how far is the blue component from luma (also known as Blue Difference)
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main()
{

	// Load an image from disk
	cv::Mat bgr{ cv::imread("../data/images/capsicum.jpg") };

	// Prepare cv::Mat for YCrCb image
	cv::Mat ycbImage;

	// Convert colors from BGR to YCrCb
	cv::cvtColor(bgr, ycbImage, cv::COLOR_BGR2YCrCb);

	// Split channels of YCrCb image
	cv::Mat ycbChannels[3];

	// Split image into separate channels
	cv::split(ycbImage, ycbChannels);

	// Show images
	cv::imshow("YCrCb", ycbImage);

	// Y channle looks similar to a grayscale
	cv::imshow("Y Channel", ycbChannels[0]);

	// Cr channel indicates the amount of Red in the image
	cv::imshow("Cr Channel", ycbChannels[1]);

	// Cb indicates the amount of Blue int the image
	cv::imshow("Cb Channel", ycbChannels[2]);
	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}