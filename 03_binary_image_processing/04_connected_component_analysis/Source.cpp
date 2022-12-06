/*
 * Connected Component Analysis:
 * Primary goal of Connected Component Analysis is labeling blobs in a binary image. It can also be used to count number of blobs.
 * In OpenCV we have w cv::connectedComponents() function:
 * Syntax:
 *	int cv::connectedComponents(InputArray image, OutputArray labels, int connectivity, int ltype, int ccltype)
 * Parameters:
 *	- image - the 8-bit single-channel image to be labeled
 *	- labels - destination labeled image
 *	- connectivity - 8 or 4 for 8-way or 4-way connectivity respectively
 *	- ltype - output image label type. CV_32S and CV_16U are supported
 *	- ccltype - connected components algorithm type (https://docs.opencv.org/3.4/d3/dc0/group__imgproc__shape.html#ga5ed7784614678adccb699c70fb841075)
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main()
{
	// Load image from disk
	cv::Mat img{ cv::imread("../data/images/truth.png", cv::IMREAD_GRAYSCALE) };

	// Show the image
	cv::imshow("Image", img);
	cv::waitKey(0);
	cv::destroyWindow("Image");

	// Use threshold to get binary image
	cv::Mat imThresh;
	cv::threshold(img, imThresh, 127, 255, cv::THRESH_BINARY);

	// Find connected components in binary image
	cv::Mat imLabels;
	int nComponents = cv::connectedComponents(imThresh, imLabels);

	// Clone the image with labels
	cv::Mat imLabelsCopy = imLabels.clone();

	// Find the minimum and maximum pixel values in imLabels
	cv::Point minLoc, maxLoc;
	double minVal, maxVal;

	// Find min and max pixel values and their locations in the given image
	cv::minMaxLoc(imLabels, &minVal, &maxVal, &minLoc, &maxLoc);

	// Normalize the image so the min value is 0 and max value is 255
	imLabels = 255 * (imLabels - minVal) / (maxVal - minVal);

	// Convert image to 8-bits
	imLabels.convertTo(imLabels, CV_8U);

	// Another option to normalize an image is use a cv::normalize() function
	cv::Mat imLabelsNormalize;
	cv::normalize(imLabelsCopy, imLabelsNormalize, 0, 255, cv::NORM_MINMAX, CV_8U);

	// Show result of both operations
	cv::imshow("Normalize Image using cv::normalize", imLabelsNormalize);
	cv::imshow("Standard normalization", imLabels);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Display each label separately
	// Copy back the original imLabels
	imLabelsCopy.copyTo(imLabels);
	// Print number of components
	std::cout << "Number of components: " << nComponents << std::endl; // 6 (from 0 to 5, where 0 is a background)

	// Show images
	cv::imshow("Background", imLabels == 0);
	cv::imshow("Letter T_f", imLabels == 1);
	cv::imshow("Letter R", imLabels == 2);
	cv::imshow("Letter U", imLabels == 3);
	cv::imshow("Letter T_s", imLabels == 4);
	cv::imshow("Letter H", imLabels == 5);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Color Coding Components
	/*
	 * We can also apply a colormap to grayscale image. First we have to normalize the image to range [0, 255], the we have to apply a colormap.
	 * For that we have a cv::applyColorMap() function
	 * Syntax:
	 *	void cv::applyColorMap	(InputArray src, OutputArray dst, int colormap)
	 * Parameters:
	 *	- src - he source image, grayscale or colored of type CV_8UC1 or CV_8UC3.
	 *	- dst - The result is the colormapped source image.
	 *	- colormap - the colormap to apply (https://docs.opencv.org/4.x/d3/d50/group__imgproc__colormap.html#ga9a805d8262bcbe273f16be9ea2055a65)
	 */

	// Apply a color map
	cv::Mat imColorMap;
	cv::applyColorMap(imLabelsNormalize, imColorMap, cv::COLORMAP_JET);

	// Display colormapped labels
	cv::imshow("Connected Components in Color", imColorMap);
	cv::waitKey(0);
	cv::destroyWindow("Connected Components in Color");

	return 0;
}