/*
 * Opening and Closing:
 * We can combine erosion and dilation to perform operations on binary image. For example we can remove
 * black or white spots from an image. Because erosion removes white regions and dilation adds white regions.
 * Thus, if we want to remove small white spots, we should perform erosion first and then dilation so that
 * white spots are removed, whereas the bigger withe blobs are unchanged. Similarly we remove black spots
 * using dilation followed by erosion.
 * These operations have own names: Opening and Closing.
 * Opening refers erosion followed by dilation - this is used for clearing white blobs.
 * Closing refers dilation followed by erosion - this is used for clearing black holes.
 *
 * Opening and Closing in OpenCV
 * OpenCV library contains function cv::MorphologyEx to perform those operations, but we have to specify
 * the argument for opening and closing operations (cv::MORPH_OPEN, cv:MORPH_CLOSE).
 * Syntax:
 * Opening:
 *  void cv::morphologyEx(Mat initialImage, Mat imageMorphOpened, MORPH_OPEN, Mat structuringElement)
 * Closing:
 *	void cv::morphologyEx(Mat initialImage, Mat imageMorphClosed, MORPH_CLOSE, Mat structuringElement)
 *
 * Generally:
 *	void cv::morphologyEx(InputArray src, OutputArray dst, int op, InputArray kernel, Point anchor=(-1, -1), int iterations=-1, 
 *						  int borderType = BORDER_CONSTANT, const Scaler& borderValue=morphologyDefaultBorderValue)
 * Parameters:
 *	- src - source image, the number of channels can be arbitrary. The depth should be one of: CV_8U, CV_16U,
 *		    CV_16S, CV_32F or CV_64F.
 *	- dst - destination image of the same size and type as source image.
 *	- op - type of a morphological operation.
 *	- kernel - structuring element. It can be created using getStructuringElement.
 *	- anchor - anchor position with the kernel. Negative value mean that the anchor is at the kernel center.
 *	- iterations - number of times opening/closing are applied.
 *	- borderType - pixel extrapolation method.
 *	- borderValue - border value in case of a constant border. The default value has a special meaning. 
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load an image from disk for opening operation
	// In this case we want to remove the small white blobs while keeping the bigger blobs intact
	cv::Mat image{ cv::imread("../data/images/opening.png", cv::IMREAD_GRAYSCALE) };

	// Show the image
	cv::imshow("Image", image);
	cv::waitKey(0);
	cv::destroyWindow("Image");

	// 1. Opening using combination of erosion and dilation

	// Specify kernel size
	int kernelSize{ 10 };

	// Create kernel
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1), cv::Point(kernelSize, kernelSize));

	// Show the element
	cv::namedWindow("Element", cv::WINDOW_NORMAL);
	cv::resizeWindow("Element", 250, 250);
	cv::imshow("Element", element * 255);
	cv::waitKey(0);
	cv::destroyWindow("Element");

	// Perform erosion
	cv::Mat imEroded;
	cv::erode(image, imEroded, element, cv::Point(-1, -1), 1);

	// Perform dilation
	cv::Mat imOpen;
	cv::dilate(imEroded, imOpen, element, cv::Point(-1, -1), 1);

	cv::imshow("Original Image", image);
	cv::imshow("After Erosion Operation", imEroded);
	cv::imshow("After Dilation Operation", imOpen);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// 2. Opening using MorphologyEx function

	// Get structuring element/kernel which will be used for opening operation
	int openingSize{ 3 };

	// Selecting an elliptical kernel
	element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * openingSize + 1, 2 * openingSize + 1), cv::Point(openingSize, openingSize));

	// Applying erosion followed by dilation by using morphologyEx function and specifying cv::MORPH_OPEN parameter
	cv::Mat imageMorphOpened;
	cv::morphologyEx(image, imageMorphOpened, cv::MORPH_OPEN, element, cv::Point(-1, -1), 3);

	cv::imshow("Original Image", image);
	cv::imshow("After Opening Operation", imageMorphOpened);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Load image from disk for closing operation
	// This time we want to remove the small black spots while keeping the bigger black wholes
	image = cv::imread("../data/images/closing.png", cv::IMREAD_GRAYSCALE);

	// 1. Closing using dilation and erosion

	// Specify kernel size
	kernelSize = 10;

	// Create kernel
	element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1), cv::Point(kernelSize, kernelSize));

	// Perform dilation
	cv::Mat imDilated;
	cv::dilate(image, imDilated, element);

	// Perform erosion
	cv::Mat imClose;
	cv::erode(imDilated, imClose, element);

	cv::imshow("Original Image", image);
	cv::imshow("After Dilation Operation", imDilated);
	cv::imshow("After Erosion Operation", imClose);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// 2. Opening using MorphologyEx

	// Crete a structuring element
	int closingSize{ 10 };

	// Selecting a elliptical kernel and storing in Mat element
	element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * closingSize + 1, 2 * closingSize + 1), cv::Point(closingSize, closingSize));

	// Perform Closing
	cv::Mat imageMorphClosed;
	cv::morphologyEx(image, imageMorphClosed, cv::MORPH_CLOSE, element);

	cv::imshow("Original Image", image);
	cv::imshow("After Closing Operation", imageMorphClosed);
	cv::waitKey(0);
	cv::destroyAllWindows();



	return 0;
}