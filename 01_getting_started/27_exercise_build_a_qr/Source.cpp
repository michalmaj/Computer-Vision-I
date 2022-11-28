/*
 * Aim:
 * Implement a QR Code Detector and Decoder using OpenCV from scratch.
 *
 * Outline:
 *	1. Read image.
 *	2. Detect QR code in the image.
 *	3. Draw bounding box around the detected QR Code.
 *	4. Print the decoded text.
 *	5. Save and display the result image.
 *
 * OpenCV functions to use:
 *	1. Reading and writing image: cv::imread(), cv::imwrite()
 *	2. Write text on image: cv::putText()
 *	3. Draw a line on image: cv::line()
 *	4. QR Detection: cv::QRCodeDetector(), detectAndDecode()
 */

// Include libraries
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>


int main()
{
	// Step 1: Read Image
	// Path to the image file
	std::string imgPath{ "../data/images/IDCard-Satya.png" };

	// Read image and store it in variable img
	// Load the image from disk
	cv::Mat img{ cv::imread(imgPath, cv::IMREAD_COLOR) };

	// Show height and width of the image
	std::cout << img.size().height << " " << img.size().width << std::endl;


	// Step 2: Detect QR Code in the Image
	cv::Mat bbox, rectifiedImage;

	// Create a QRCodeDetector Object
	// Variable name should be qrDecoder
	cv::QRCodeDetector qrDecoder{ cv::QRCodeDetector::QRCodeDetector() };

	// Detect QR Code in the Image
	// Output should be stored in opencvData
	std::string opencvData{ qrDecoder.detectAndDecode(img, bbox, rectifiedImage) };

	// Check if a QR Code has been detected
	if (opencvData.length() > 0)
		std::cout << "QR Code Detected" << std::endl;
	else
		std::cout << "QR Code NOT Detected" << std::endl;


	// Step 3: Draw bounding box around the detected QR Code
	// get number of rows
	int n{ bbox.rows};

	// Create a copy of the original image
	cv::Mat imgCopy1 = img.clone();

	// Creates point from bbox
	cv::Point p1{ static_cast<int>(bbox.at<float>(0,0)) , static_cast<int>(bbox.at<float>(0,1)) };
	cv::Point p2{ static_cast<int>(bbox.at<float>(0,2)) , static_cast<int>(bbox.at<float>(0,3)) };
	cv::Point p3{ static_cast<int>(bbox.at<float>(0,4)) , static_cast<int>(bbox.at<float>(0,5)) };
	cv::Point p4{ static_cast<int>(bbox.at<float>(0,6)) , static_cast<int>(bbox.at<float>(0,7)) };

	cv::line(img, p1, p2, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	cv::line(img, p2, p3, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	cv::line(img, p3, p4, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	cv::line(img, p4, p1, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

	cv::imshow("Img", img);
	cv::waitKey(0);
	cv::destroyWindow("Img");

	// It is also possible to draw a rectangle instead of lines
	// Where p1 is top left point and p2 is bottom right point 
	cv::rectangle(imgCopy1, p1, p3, cv::Scalar(255, 0, 255), 1, cv::LINE_AA);
	cv::imshow("Img", imgCopy1);
	cv::waitKey(0);
	cv::destroyWindow("Img");


	// Step 4: Print the Decoded Text
	// Since we have already detected and decoded the QR Code
	// using qrDecoder.detectAndDecode, we will directly
	// use the decoded text we obtained at that step (opencvData)
	std::cout << "QR Code Detected!" << std::endl;
	std::cout << opencvData << std::endl;


	// Step 5: Save and display the result image
	cv::imwrite("QRCode-Output.png", img);

	cv::imshow("Img", img);
	cv::waitKey(0);
	cv::destroyWindow("Img");



	return 0;
}