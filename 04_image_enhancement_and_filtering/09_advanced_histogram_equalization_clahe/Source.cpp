/*
 * Contrast Limited Adaptive Histogram Equalization (CLAHE)
 * Histogram equalization uses the pixels of the entire image to improve contrast. While this may gives a good looking output
 * in many cases, sometimes we want to enhance the contrast locally so the image looks more natural.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

int main()
{
	// Load an image from disk
	cv::Mat img{ cv::imread("../data/images/night-sky.jpg") };

	// Convert to HSV color space
	cv::Mat imhsv;
	cv::cvtColor(img, imhsv, cv::COLOR_BGR2HSV);

	// Prepare clone for CLAHE
	cv::Mat imhsvClahe{ imhsv.clone() };

	// Prepare two std::vector to store split images (separate three channels)
	std::vector<cv::Mat> imhsvChannels(3);
	std::vector<cv::Mat> imhsvClaheChannels(3);

	// Split both hsv images
	cv::split(imhsv, imhsvChannels);
	cv::split(imhsvClahe, imhsvClaheChannels);

	// Perform normal histogram equalization on Value (V) channel
	cv::equalizeHist(imhsvChannels[2], imhsvChannels[2]);

	// Create CLAHE class object
	/*cv::Ptr<cv::CLAHE>*/auto clahe = cv::createCLAHE(2.0, cv::Size(8, 8)); // Use explicit type or auto keyword

	// Perform CLAHE on Value (V) channel
	clahe->apply(imhsvClaheChannels.at(2), imhsvClaheChannels.at(2));

	// Merge back images
	cv::merge(imhsvChannels, imhsv);
	cv::merge(imhsvClaheChannels, imhsvClahe);

	// Convert both images to BGR format
	cv::Mat imEq, imEqCLAHE;
	cv::cvtColor(imhsv, imEq, cv::COLOR_HSV2BGR);
	cv::cvtColor(imhsvClahe, imEqCLAHE, cv::COLOR_HSV2BGR);

	// Show images
	cv::imshow("Original Image", img);
	cv::imshow("Histogram Equalization", imEq);
	cv::imshow("CLAHE", imEqCLAHE);
	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}