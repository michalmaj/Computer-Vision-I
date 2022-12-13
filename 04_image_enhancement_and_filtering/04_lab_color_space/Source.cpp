/*
 * The Lab color space:
 * The Lab color space consists of
 *	- L - lightness;
 *	- A - a color component ranging from Green to Magenta;
 *	- B - a color component ranging from Blue to Yellow.
 * L channel encodes brightness, while A and B channels encode color
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load an image from disk
	cv::Mat bgr{ cv::imread("../data/images/capsicum.jpg") };

	// Prepare a cv::Mat for lab image
	cv::Mat labImage;

	// Convert colors from BGR to Lab
	cv::cvtColor(bgr, labImage, cv::COLOR_BGR2Lab);

	// Split Lab channels
	cv::Mat labChannels[3];
	cv::split(labImage, labChannels);

	// Show images
	cv::imshow("Lab", labImage);
	cv::imshow("L Channel", labChannels[0]);
	cv::imshow("A Channel", labChannels[1]);
	cv::imshow("B Channel", labChannels[2]);
	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}