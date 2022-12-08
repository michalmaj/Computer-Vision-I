// Include libraries
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <format>

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

// Helper function to print hierarchy
template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
	for (const auto& e : v)
		out << e << " ";
	return out;
}

int main()
{
	// Step 1: Read Image
	// Image path
	std::string imagePath{ "../data/images/CoinsA.png" };

	// Load image from disk
	cv::Mat image{ cv::imread(imagePath, cv::IMREAD_COLOR) };

	// Make a copy if the original image
	cv::Mat imageCopy = image.clone();

	// Show the image
	cv::imshow("Image", image);
	cv::waitKey(0);
	cv::destroyWindow("Image");

	// Step 2.1: Convert Image to Grayscale
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

	// Show grayscale image
	cv::imshow("Gray", imageGray);
	cv::waitKey(0);
	cv::destroyWindow("Gray");

	// Step 2.2: Split Image into R, G, B Channels
	cv::Mat imageB, imageG, imageR;
	cv::Mat splitted[3];

	cv::split(image, splitted);

	imageB = splitted[0];
	imageG = splitted[1];
	imageR = splitted[2];

	// Show individual channels
	cv::imshow("Blue Channel", imageB);
	cv::imshow("Green Channel", imageG);
	cv::imshow("Red Channel", imageR);
	cv::waitKey(0);
	cv::destroyAllWindows();

	cv::Mat imgB{ imageB.clone() }, imgG{imageG.clone()}, imgR{imageR.clone()}, imgGray{imageGray.clone()};

	// Step 3.1: Perform threshold
	// Set some started values for threshold and maximum value
	int thresh{ 15 }; // Checking in range [5, 230, step=5]
	int maxVal{ 255 }; // Checking 100, 120, 150, 170, 200, 230, 255


	//// For testing purpose - values for threshold from 5 to 230 inclusive with step=5
	//std::vector<int> t(230 / 5);
	//std::generate(t.begin(), t.end(), [n = 0]()mutable {return n += 5; });

	//for(size_t i{0}; i < t.size(); ++i)
	//{
	//	cv::threshold(imageB, imageB, t.at(i), maxVal, cv::THRESH_BINARY_INV); // Should be inv
	//	cv::threshold(imageG, imageG, t.at(i), maxVal, cv::THRESH_BINARY_INV); // Should be inv, best channel to perform threshold - minimum holes in coins
	//	cv::threshold(imageR, imageR, t.at(i), maxVal, cv::THRESH_BINARY_INV); // Because background is red we should discard this channel.
	//	cv::threshold(imageGray, imageGray, t.at(i), maxVal, cv::THRESH_BINARY_INV); // Results aren't enough good 
	//	cv::imwrite(std::format("testing/blue/{}.png", t.at(i)), imageB);
	//	cv::imwrite(std::format("testing/green/{}.png", t.at(i)), imageG);
	//	cv::imwrite(std::format("testing/red/{}.png", t.at(i)), imageR);
	//	cv::imwrite(std::format("testing/gray/{}.png", t.at(i)), imageGray);

	//	imgB.copyTo(imageB);
	//	imgG.copyTo(imageG);
	//	imgR.copyTo(imageR);
	//	imgGray.copyTo(imageGray);
	//}

	imgB.copyTo(imageB);
	imgG.copyTo(imageG);
	imgR.copyTo(imageR);
	imgGray.copyTo(imageGray);

	// Threshold all channels and find best solution
	cv::threshold(imageG, imageG, thresh, maxVal, cv::THRESH_BINARY_INV); // Should be inv, best channel to perform threshold - minimum holes in coins


	// Show after threshold
	cv::imshow("Green Channel Threshold", imageG);
	cv::waitKey(0);
	cv::destroyWindow("Green Channel Threshold");

	// Step 3.2: Perform morphological operations
	// Create kernel
	int kernelSize{ 5 };
	cv::Mat kernel{ cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(kernelSize, kernelSize)) };

	cv::Mat imageDilated;

	// Use dilatation to remove background noises
	cv::dilate(imageG, imageDilated, kernel, cv::Point(-1, -1), 4);

	// Show image after morphological operation
	cv::imshow("Image after dilatation", imageDilated);
	cv::waitKey(0);
	cv::destroyWindow("Image after dilatation");

	// Use erosion to expand coins (make them more circular)
	cv::Mat imageEroded;
	cv::erode(imageDilated, imageEroded, kernel, cv::Point(-1, -1), 3);

	cv::imshow("Image after erosion", imageEroded);
	cv::waitKey(0);
	cv::destroyWindow("Image after erosion");

	// Step 4.1: Create SimpleBlobDetector

	// Set up custom SimpleBlobDetector parameters
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

	// Create detector with params
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

	// Create a std::vector to store keypoints
	std::vector<cv::KeyPoint> keypionts;

	// Step 4.2: Detect Coins
	detector->detect(imageEroded, keypionts);

	std::cout << "Number of coins detected = " << keypionts.size() << std::endl;

	// Step 4.3: Display the detected coins on original image
	for(size_t i{0}; i < keypionts.size(); ++i)
	{
		cv::KeyPoint k{ keypionts.at(i) };
		cv::Point keyPt{ k.pt };
		auto [x, y] = k.pt;
		x = static_cast<int>(x);
		y = static_cast<int>(y);

		// Mark center of the coin in blue
		cv::circle(image, cv::Point(x, y), 3, cv::Scalar(255, 0, 0), -1);

		// Get radius of the blob
		float diameter{ k.size };
		int radius{ static_cast<int>(diameter / 2.0)};

		// Draw circle around coins in green
		cv::circle(image, cv::Point(x, y), radius, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);

	}

	// Show image with annotation
	cv::imshow("Image with annotation", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with annotation");

	// Step 4.4: Perform Connected Components Analysis
	cv::Mat imLabels;

	// Find number of connected components (Invert pixels, white to black and black to white, we can use bitwise_not or ~ operator).
	int nComponents{ cv::connectedComponents(~imageEroded, imLabels, 8, CV_32S, cv::CCL_SAUF) };

	std::cout << "Number of connected components detected = " << nComponents << std::endl;

	cv::Mat colorMap{ displayConnectedComponents(imLabels) };

	// Display connected components
	cv::imshow("Connected Components", colorMap);
	cv::waitKey(0);
	cv::destroyWindow("Connected Components");

	//// Show components one by one (0 is a background) - for testing purpose
	//for(int i{0}; i < nComponents; ++i)
	//{
	//	cv::imshow("Connected Component", imLabels==i);
	//	cv::waitKey(0);
	//	cv::destroyWindow("Connected Component");
	//}

	// Step 4.5: Detect Coins using contour detection

	// Set up two std::vectors
	std::vector<std::vector<cv::Point>> contours; // vector of vectors to store contours
	std::vector<cv::Vec4i> hierarchy; // vector to store hierarchy

	// Find all contours and compress parameters
	cv::findContours(imageEroded, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	// Print number of contours
	std::cout << "Number of contours found = " << contours.size() << std::endl;

	//// Print hierarchy - testing purpose
	//std::cout << "hierarchy = " << hierarchy << std::endl;

	// Restore original image to default
	imageCopy.copyTo(image);
	// Draw all contours
	cv::drawContours(image, contours, -1, cv::Scalar(0, 0, 0), 2, cv::LINE_AA);

	// Show image with contours
	cv::imshow("Image with contours", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contours");

	// Draw only outer contour
	imageCopy.copyTo(image);
	cv::findContours(imageEroded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 10, cv::LINE_AA);

	// Show image with external contour
	cv::imshow("Image with external contour", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with external contour");

	// Sort contours by area in descending order and pop_back last element

	// Find again all contours and show the area
	cv::findContours(imageEroded, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	for(size_t i{0}; i < contours.size(); ++i)
	{
		double area{ cv::contourArea(contours.at(i)) };
		double perimeter{ cv::arcLength(contours.at(i), true) };
		std::cout << "Contour #" << i + 1 << "has perimeter: " << perimeter << ", and area: " << area << std::endl;
	}

	// Remove highest area contour

	// Sort from smallest to biggest
	std::sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& p1, const std::vector<cv::Point>& p2) {return cv::contourArea(p1) < cv::contourArea(p2); });

	// Print maximum area of contour
	double areaBiggestContour{ cv::contourArea(contours.back()) };
	std::cout << "Maximum area of contour = " << areaBiggestContour << std::endl;

	contours.pop_back();

	// Draw rest of contours
	imageCopy.copyTo(image);
	cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);

	// Show image with contours
	cv::imshow("Image with contours", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contours");

	// Fit circles on coin
	for(size_t i{0}; i < contours.size(); ++i)
	{
		cv::Moments M{ cv::moments(contours.at(i)) };
		int x{ static_cast<int>(M.m10 / M.m00) };
		int y{ static_cast<int>(M.m01 / M.m00) };

		// Draw a circle
		cv::circle(image, cv::Point(x, y), 3, cv::Scalar(0, 0, 255), -1);
	}

	// Show image with contours and circle
	cv::imshow("Image with contours and circle", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contours and circle");


	return 0;
}
