#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <format>

void show(const std::string& name, const cv::Mat m)
{
	auto [w, h] = m.size();
	w /= 3;
	h /= 3;
	cv::namedWindow(name, cv::WINDOW_NORMAL);
	cv::resizeWindow(name, cv::Size(w, h));
	cv::imshow(name, m);
}

int main()
{
	// Step 1: Read image
	// Load image from disk
	cv::Mat image{ cv::imread("../data/images/CoinsB.png") };

	// Create a copy of the original image
	cv::Mat imageCopy{ image.clone() };

	// Step 2.1: Convert Image to Grayscale
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);


	// Show original and grayscale images
	show("Color Image", image);
	show("Grayscale Image", imageGray);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Step 2.2: Split image into R, G, B Channels
	cv::Mat imageB, imageG, imageR;
	cv::Mat channels[3];
	cv::split(image, channels);

	imageB = channels[0];
	imageG = channels[1];
	imageR = channels[2];

	show("Blue Channel", imageB);
	show("Green Channel", imageG);
	show("Red Channel", imageR);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Step 3.1: Perform threshold
	cv::Mat imB{ imageB.clone() }, imG{ imageG.clone() }, imR{ imageR.clone() }, imGray{ imageGray.clone() };

	// Set some starting values for threshold
	int thresh{ 135 };
	int maxVal{ 255 };

	//// For testing purpose
	//std::vector<int> t(230/5);
	//std::generate(t.begin(), t.end(), [n = 0]()mutable {return n += 5; });

	//for(size_t i{0}; i < t.size(); ++i)
	//{
	//	cv::threshold(imB, imB, t.at(i), maxVal, cv::THRESH_BINARY);
	//	cv::threshold(imG, imG, t.at(i), maxVal, cv::THRESH_BINARY);
	//	cv::threshold(imR, imR, t.at(i), maxVal, cv::THRESH_BINARY);
	//	cv::threshold(imGray, imGray, t.at(i), maxVal, cv::THRESH_BINARY);

	//	cv::imwrite(std::format("testing/blue/{}.png", t.at(i)), imB);
	//	cv::imwrite(std::format("testing/green/{}.png", t.at(i)), imG);
	//	cv::imwrite(std::format("testing/red/{}.png", t.at(i)), imR);
	//	cv::imwrite(std::format("testing/gray/{}.png", t.at(i)), imGray);

	//	imageB.copyTo(imB);
	//	imageG.copyTo(imG);
	//	imageR.copyTo(imR);
	//	imageGray.copyTo(imGray);
	//}

	cv::Mat imageThresh;
	cv::threshold(imageB, imageThresh, thresh, maxVal, cv::THRESH_BINARY);

	show("Threshold Image", imageThresh);
	cv::waitKey(0);
	cv::destroyWindow("Threshold Image");

	// Step 3.2: Perform morphological operations
	
	// Crete a structuring element
	int closingSize{ 3 };

	// Selecting a elliptical kernel and storing in Mat element
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * closingSize + 1, 2 * closingSize + 1), cv::Point(closingSize, closingSize));

	// Perform Closing
	cv::Mat imageMorphClosed;
	cv::morphologyEx(imageThresh, imageMorphClosed, cv::MORPH_CLOSE, element, cv::Point(-1, -1), 2);

	show("Closed Image", imageMorphClosed);
	cv::waitKey(0);
	cv::destroyWindow("Closed Image");

	cv::Mat imageMorphOpen;
	cv::morphologyEx(imageMorphClosed, imageMorphOpen, cv::MORPH_OPEN, element, cv::Point(-1, -1), 20);

	show("Open Image", imageMorphOpen);
	cv::waitKey(0);
	cv::destroyWindow("Open Image");

	return 0;
}
