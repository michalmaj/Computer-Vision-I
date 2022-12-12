/*
 * The RGB color space:
 * The RGB color space is an additive color space, in which Red, Green and blue light rays are added in various proportions to
 * produce different colors. It is the most commonly used color space for image processing.
 * In RGB color space, all three channels contain information about he color as well as brightness. It is better for
 * some applications if we can separate the color component (Chrominance) from the lightness or brightness (Luminance).
 *
 * In OpenCV library the image is typically loaded in BGR format. It si the same as RGB, but stored in reversed order.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load an image from disk
	cv::Mat bgr{ cv::imread("../data/images/capsicum.jpg") };

	// Show the image
	cv::imshow("BGR", bgr);
	cv::waitKey(0);
	cv::destroyWindow("BGR");

	// Split image into separate channels
	cv::Mat bgrChannels[3];
	cv::split(bgr, bgrChannels);

	// Show separate channels
	cv::imshow("Blue Channel", bgrChannels[0]);
	cv::imshow("Green Channel", bgrChannels[1]);
	cv::imshow("Red Channel", bgrChannels[2]);
	cv::waitKey(0);
	cv::destroyAllWindows();
}