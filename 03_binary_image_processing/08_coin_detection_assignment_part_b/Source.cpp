#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <format>

// Helper function to resize window in cv::imshow() function
void show(const std::string& name, const cv::Mat& m)
{
	auto [w, h] = m.size();
	w /= 3;
	h /= 3;
	cv::namedWindow(name, cv::WINDOW_NORMAL);
	cv::resizeWindow(name, cv::Size(w, h));
	cv::imshow(name, m);
}

// Function for step 4.4: Perform Connected Component Analysis
cv::Mat displayConnectedComponents(const cv::Mat& im)
{
	// Make the copy of the image
	cv::Mat imLabels{ im.clone() };

	// Normalize image, so the minimum value will be 0 and maximum will be 255
	cv::normalize(imLabels, imLabels, 0, 255, cv::NORM_MINMAX, CV_8U);

	// Apply a color map
	cv::Mat imColorMap;
	cv::applyColorMap(imLabels, imColorMap, cv::COLORMAP_JET);

	return imColorMap;
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

	// Step 4.1: Create SimpleBlobDetector

	// Setup SimpleBlobDetector parameters.
	cv::SimpleBlobDetector::Params params;
	params.blobColor = 0;

	params.minDistBetweenBlobs = 2;

	// Filter by Area
	params.filterByArea = false;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.8;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.8;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.8;

	// Set up detector with params
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

	// Step 4.2: Detect Coins

	// Create a std::vector to store keypoints
	std::vector<cv::KeyPoint> keypoints;

	detector->detect(imageMorphOpen, keypoints);

	std::cout << "Number of coins detected = " << keypoints.size() << std::endl;

	// Step 4.3: Display the detected coins on original image
	for(size_t i{0}; i < keypoints.size(); ++i)
	{
		cv::KeyPoint k{ keypoints.at(i) };
		cv::Point keyPt{ k.pt };
		auto [x, y] = k.pt;

		// Mark the center of the coins in blue
		cv::circle(image, cv::Point(static_cast<int>(x), static_cast<int>(y)), 5, cv::Scalar(255, 0, 0), -1);

		// Calculate diameter and radius
		float diameter{ k.size };
		int radius{ static_cast<int>(diameter / 2.0) };

		// Draw a circle around coin in red
		cv::circle(image, cv::Point(static_cast<int>(x), static_cast<int>(y)), radius, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
	}

	// Show image with annotation
	show("Image with annotation", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with annotation");

	// Step 4.4: Perform Connected Components

	cv::Mat imLabels;

	// Find number of connected components (Invert pixels, white to black and black to white, we can use bitwise_not or ~ operator).
	int nComponents{ cv::connectedComponents(~imageMorphOpen, imLabels, 8, CV_32S, cv::CCL_SAUF) };

	std::cout << "Number of connected components detected = " << nComponents << std::endl;

	//// For testing purpose - show connected components one by one
	//for(int i{0}; i < nComponents; ++i)
	//{
	//	show(std::format("{} component", i), imLabels==i); // 0 is a background
	//	cv::waitKey(0);
	//	cv::destroyAllWindows();
	//}

	cv::Mat colorMap{ displayConnectedComponents(imLabels) };

	// Display connected components
	show("Connected Components", colorMap);
	cv::waitKey(0);
	cv::destroyWindow("Connected Components");

	// Step 4.5: Detect coins using Contour Detector

	// Set up two vectors
	std::vector<std::vector<cv::Point>> contours; // vector of vectors to store cv::Points
	std::vector <cv::Vec4i> hierarchy; // vectors to store contours hierarchy

	// Find all contours and compress points
	cv::findContours(imageMorphOpen, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);


	// Print number of contours
	std::cout << "Number of contours found = " << contours.size() << std::endl;

	// Draw all contours in green
	imageCopy.copyTo(image);

	cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 5, cv::LINE_AA);

	// Show image with contours
	show("Image with contours", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contours");

	// Find only outer contours
	cv::findContours(imageMorphOpen, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Print number of contours
	std::cout << "Number of contours found = " << contours.size() << std::endl;

	// Draw and show external contour
	imageCopy.copyTo(image);
	cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 10, cv::LINE_AA);
	show("Image with contour", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contour");

	// Calculate area and perimeter of all contours
	cv::findContours(imageMorphOpen, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	for(size_t i{0}; i < contours.size(); ++i)
	{
		double area{ cv::contourArea(contours.at(i)) };
		double perimeter{ cv::arcLength(contours.at(i), true) };
		std::cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << std::endl;
	}

	// Sort contours from smallest to largest and print contour with highest area
	std::sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& p1, const std::vector<cv::Point>& p2) {return (cv::contourArea(p1) < cv::contourArea(p2)); });

	// Print maximum area of contour
	double maxContourArea{ cv::contourArea(contours.end()[-1]) };

	std::cout << "Maximum area of contour = " << maxContourArea << std::endl;

	// Remove this contour from our contours collection
	contours.pop_back();

	// Plot rest of contours
	imageCopy.copyTo(image);
	cv::drawContours(image, contours, -1, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
	show("Image with contour", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contour");

	// Remove both inner contours (smallest area)
	// Print again area and perimeters
	for (size_t i{ 0 }; i < contours.size(); ++i)
	{
		double area{ cv::contourArea(contours.at(i)) };
		double perimeter{ cv::arcLength(contours.at(i), true) };
		std::cout << "Contour #" << i + 1 << " has area = " << area << " and perimeter = " << perimeter << std::endl;
	}

	contours.erase(contours.begin()); // Remove 1st smallest
	contours.erase(contours.begin()); // Remove 2nd smallest

	std::cout << "Number of coins detected = " << contours.size() << std::endl;

	// Draw revisited contours
	imageCopy.copyTo(image);

	cv::drawContours(image, contours, -1, cv::Scalar(255, 0, 0), 5, cv::LINE_AA);
	show("Image with contour", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contour");

	// Fit circles on coins and show numbers from 1(smallest) to 10(highest)

	// Create object of moments
	cv::Moments M;

	// Prepare variables for (x, y)-coordiantes
	int x, y;

	for(size_t i{0}; i < contours.size(); ++i)
	{
		M = cv::moments(contours.at(i));
		x = static_cast<int>(M.m10 / M.m00);
		y = static_cast<int>(M.m01 / M.m00);

		// Draw circle
		cv::circle(image, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), -1);

		cv::putText(image, std::to_string(i + 1), cv::Point(x + 40, y - 10), cv::FONT_HERSHEY_COMPLEX, 2, cv::Scalar(0, 0, 0), 3);
	}
	show("Image with contour", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contour");

	return 0;
}
