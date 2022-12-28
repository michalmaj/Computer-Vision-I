#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Implement Variance of absolute values of Laplacian - Method 1
// Input: image
// Output: Floating point number denoting the measure of sharpness of image
double var_abs_laplacian(const cv::Mat& image)
{
	// Set the kernel size
	int kernelSize{ 3 };

	// Convert image to grayscale
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// Blur the image
	cv::Mat blurred;
	cv::GaussianBlur(gray, blurred, cv::Size(kernelSize, kernelSize), 0);

	// Calculate Laplacian
	cv::Mat laplacian;
	cv::Laplacian(blurred, laplacian, CV_32F, kernelSize, 1, 0);

	// Get absolute value from laplacian image
	cv::Mat laplacianABS;
	laplacianABS = cv::abs(laplacian);

	// Calculate mean and standard deviation
	cv::Mat mean;
	cv::Mat std;
	cv::meanStdDev(laplacianABS, mean, std);

	return std.at<double>(0, 0);
}

// Implement Sum Modified Laplacian - Method 2
// Input: image
// Output: Floating point number denoting the measure of sharpness of image
double sum_modified_laplacian(const cv::Mat& image)
{
	// Set the kernel size
	int kernelSize{ 3 };

	// Convert image to grayscale
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// Blur the image
	cv::Mat blurred;
	cv::GaussianBlur(gray, blurred, cv::Size(kernelSize, kernelSize), 0);

	// Set two kernels for convolution
	cv::Mat kernel1{ {3, 3}, {0, 0, 0, -1, 2, -1, 0, 0, 0} };
	cv::Mat kernel2{ {3, 3}, {0, -1, 0, 0, 2, 0, 0, -1, 0} };

	// Perform first convolution
	cv::Mat firstConv;
	cv::filter2D(blurred, firstConv, CV_32F, kernel1);

	// Perform second convolution
	cv::Mat secondConv;
	cv::filter2D(blurred, secondConv, CV_32F, kernel2);

	// Sum absolute value of both convolution results	
	auto result = cv::sum(cv::abs(firstConv) + cv::abs(secondConv));

	return result[0];
}

int main()
{
	// Read input video from disk
	std::string filename{ "../data/videos/focus-test.mp4" };

	// Create a VideoCapture object
	cv::VideoCapture cap{ filename };

	// Check to see if video if properly read
	if(!cap.isOpened())
	{
		std::cout << "Can't read a video\n";
		return -1;
	}

	// Read first frame from the video
	cv::Mat frame;
	cap >> frame;

	// Display total number of frames in the video
	std::cout << "Total number of frames: " << static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT)) << std::endl;

	double maxV1{ 0 };
	double maxV2{ 0 };

	// Frame with maximum measure of focus
	cv::Mat bestFrame1; // method 1
	cv::Mat bestFrame2; // method 2

	// Get frame ID with maximum measure
	int bestFrameId1{ 0 }; // method 1
	int bestFrameId2{ 0 }; // method 2

	// Get measures of focus from both methods
	double val1 = var_abs_laplacian(frame);
	double val2 = sum_modified_laplacian(frame);

	// Specify the ROI for flower in the frame
	int topCorner{ 25 }; // y1
	int leftCorner{ 430 }; // x1
	int bottomCorner{ 625 }; // y2
	int rightCorner{ 1025 }; // x2
	

	cv::Mat flower;

	flower = frame(cv::Range(topCorner, bottomCorner), cv::Range(leftCorner, rightCorner));

	//// Testing purpose - check the ROI
	//cv::imshow("Flower", flower);
	//cv::waitKey(0);
	//cv::destroyAllWindows();

	// Iterate over all the frames in the video
	while(true)
	{
		// Crop the flower region out of the frame
		flower = frame(cv::Range(topCorner, bottomCorner), cv::Range(leftCorner, rightCorner));

		// Get measure of focus from both methods
		val1 = var_abs_laplacian(flower);
		val2 = sum_modified_laplacian(flower);

		// Method 1:
		// If current measure of focus is greater than the current maximum 
		if(val1 > maxV1)
		{
			// Update the value of maxV1
			maxV1 = val1;

			// Get frame ID of the new best frame
			bestFrameId1 = static_cast<int>(cap.get(cv::CAP_PROP_POS_FRAMES));

			// Set the new best frame
			bestFrame1 = frame.clone();

			std::cout << "Frame ID of the best frame [Method 1]: " << bestFrameId1 << std::endl;
		}

		// Method 2:
		// If current measure of focus is greater than the current maximum
		if(val2 > maxV2)
		{
			// Update the value of maxVal2
			maxV2 = val2;

			//Get frame ID of the new best frame
			bestFrameId2 = static_cast<int>(cap.get(cv::CAP_PROP_POS_FRAMES));

			// Set the new best frame
			bestFrame2 = frame.clone();

			std::cout << "Frame ID of the best frame [Method 2]: " << bestFrameId2 << std::endl;
		}

		cap >> frame;
		if(frame.empty())
			break;
	}

	std::cout << "\n====================================================\n\n";

	// Print the frame ID of the best frame
	std::cout << "Frame ID of the best frame [Method 1]: " << bestFrameId1 << std::endl;
	std::cout << "Frame ID of the best frame [Method 2]: " << bestFrameId2 << std::endl;

	cap.release();

	cv::Mat out;
	cv::hconcat(bestFrame1, bestFrame2, out);

	cv::imshow("Best frames of method 1 & 2", out);
	cv::waitKey(0);
	cv::destroyWindow("Best frames of method 1 & 2");

	return 0;
}