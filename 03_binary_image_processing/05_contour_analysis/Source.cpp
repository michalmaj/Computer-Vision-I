/*
 * Contours:
 * Contours are simply the boundaries of an object or part of object in an image. They are useful in shape analysis and
 * object detection/recognition.
 *
 * How to find contours using OpenCV:
 * The library contains a cv::findContours() function
 * Syntax:
 *	void cv::findContours(InputArray image, OutputArrayOfArrays contours, OutputArray hierarchy, int mode, int method, Point offset=Point())
 * Parameters
 *	- image - input image (8-bit single channel). Non-zero pixels are treated as 1's. Zero pixels remains 0's, so the image is treated as binary.
 *			  We can use compare, inRange, threshold, adaptiveThreshold, Canny, and others th create a binary image out of a grayscale or color.
 *	- contours - detect contours. Each contour is stored as a vector of points.
 *	- hierarchy - optional output vector containing information about the image topology.
 *	- mode - contour retrieval mode (RETR_EXTERNAL, RETR_LIST, RETR_CCOMP, RETR_TREE):
 *		* RETR_EXTERNAL - gives outer contours;
 *		* RETR_LIST - gives all the contours but doesn't calculate the hierarchy;
 *		* RETR_CCOMP - gives contours and organizes them into outer and inner contours (but only one nested level);
 *		* RETR_TREE - gives all contours and the full hierarchy of the contours (creates a tree of parent-child dependence).
 *	- method - contour approximation method (CHAIN_APPROX_NONE, CHAIN_APPROX_SIMPLE, CHAIN_APPROX_TC89_L1, CHAIN_APPROX_TC89_KCOS):
 *		* CHAIN_APPROX_NONE - stores all points that belong to the contour;
 *		* CHAIN_APPROX_SIMPLE - compresses horizontal, vertical, and diagonal segments and leaves only their end points;
 *		* CHAIN_APPROX_TC89 - applies one of the flavors of the Teh-Chin chain approximation algorithm
 *	- offset - optional offset by which every contour is shifted. This is useful if the contours are extracted from the image ROI and then
 *			   they should be analyzed in the whole image context.
 *
 * Next we can draw finding contours. For that OpenCV provides a cv::drawContour() function
 * Syntax:
 *	void cv::drawContours(InputOutputArray image, InputArrayOfArrays contours, int contourIdx, const Scalar& color, int thickness=1,
 *						  int lineType=LINE_8, InputArray hierarchy=noArray(), int maxLevel=INT_MAX, Point offset=Point())
 * Parameters:
 *  - image - destination image, on which we will draw contours;
 *	- contours - all the input contours, each contour is stored as a point vector;
 *	- contourIdx - parameter indicating a contour to draw. If it is negative, all contours are drawn;
 *	- color - color of the contour;
 *	- thickness - thickness of lines the contours are drawn with. If it is negative, the contour interiors are drawn;
 *	- lineType - line connectivity;
 *	- hierarchy - optional information about hierarchy, it is only need if we want to draw only some of the contours;
 *	- maxLevel - maximal level for drawn contours. If it is 0, only the specified contour is drawn. If it is 1, the function draws
 *				 the contour(s) and all the nested contours. If it is 2, the function draws the contours, all the nested contours,
 *				 all the nested-to-nested contours, and so on. This parameter is only taken into account when there is hierarchy available.
 *	- offset - optional contour shift parameter. Shift all the drawn contours by the specific offset=(dx, dy).
 *
 * If we have contours we can calculate some properties:
 *	1. Center of Mass or Centroid
 *		The centroid of a shape is the arithmetic mean of all the points in a shape. In case of image processing, each shape is made of pixels,
 *		and the centroid is the weighted average of all pixels constituting the shape.
 *		We can find the center of the blob using moments in OpenCV. Image moment is a particular weighted average of image pixel intensities,
 *		which the help of which we can find some specific properties of an image. The centroid formula:
 *		Cx = M10 / M00, Cy = M01/M00
 *		Cx is the x coordinate, Cy is the y coordinate of the centroid and M denotes Moment.
 *	2. Area and Perimeter
 *		It is possible to calculate area using a cv::contourArea() function, we have to pass one argument: contour.
 *		Also we can calculate perimeter using cv::arcLength(), we have to pass two arguments: contour and boolean (true if it is a closed contour).
 *	3. Bounding boxes
 *		We can create two types of bounding boxes around a contour:
 *			- a vertical rectangle
 *			- a rotated rectangle - this is a bounding box with the minimum area.
 *	4. Fit a circle and ellipse
 *		Fitting a bounding box (vertical or rotated) is the preferred choice in most cases when we are only dealing with circles or ellipses, fitting
 *		a circle or ellipse is a much better choice.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <format>

// Helper function for printing hierarchy
template <typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T>& v)
{
	for (const auto& e : v)
		out << e << " ";
	return  out;
}

// Function to draw different color for contour hierarchy 
cv::Scalar colorContour(int value)
{
	switch (value)
	{
	case -1: return { 0, 255, 0 };
	case 0: return { 255, 0, 0 };
	case 1: return { 0, 0, 255 };
	case 2: return { 100, 255, 100 };
	case 3: return { 255, 100, 100 };
	case 4: return { 100, 100, 255 };
	default: return { 255, 0, 255 };
	}
}

// Function to sort contours - our custom comparator, from smallest to biggest area
bool compareContourAreas(const std::vector<cv::Point>& contour1, const std::vector<cv::Point>& contour2) {
	double i = std::fabs(contourArea(cv::Mat(contour1)));
	double j = std::fabs(contourArea(cv::Mat(contour2)));
	return (i < j);
}


int main()
{
	// Load image from disk
	cv::Mat image{ cv::imread("../data/images/Contour.png") };
	// Make a copy the image
	cv::Mat imageCopy = image.clone();

	// Convert image to grayscale
	cv::Mat imageGray;
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

	// Show images
	cv::imshow("Original Image", image);
	cv::imshow("Grayscale Image", imageGray);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Find contours in grayscale image
	std::vector<std::vector<cv::Point>> contours; // vector of vectors for contours
	std::vector<cv::Vec4i> hierarchy; // vector for hierarchy

	// Find all contours with compress segments.
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	// Print number of contours
	std::cout << "Number of contours find using cv::RETR_LIST: " << contours.size() << std::endl;

	// Draw contours
	cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 6, cv::LINE_AA);

	// Show the image
	cv::imshow("Image with contours", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contours");

	// Find only external contours in the image
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	std::cout << "Number of contours find using cv::RETR_EXTERNAL: " << contours.size() << std::endl;

	// Prepare clean image
	imageCopy.copyTo(image);

	// Draw contours
	cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 6, cv::LINE_AA);

	// Show the image
	cv::imshow("Image with contours", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contours");

	// Draw only the 3rd contour
	// Note that we do not know the numbering of contour in term of the shape
	image = imageCopy.clone();
	cv::drawContours(image, contours, 2, cv::Scalar(0, 0, 255), 6, cv::LINE_AA);

	// Show the image
	cv::imshow("Image with contour", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with contour");

	// Find contours and all hierarchy
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	std::cout << "Hierarchy: " << hierarchy << std::endl;

	image = imageCopy.clone();

	for(int i{0}; i < contours.size(); ++i)
	{
		cv::Scalar color = colorContour(hierarchy[i][3]);
		cv::drawContours(image, contours, i, color, 6, cv::LINE_AA);
	}
	// Show the image
	cv::imshow("Image with different hierarchy", image);
	cv::waitKey(0);
	cv::destroyWindow("Image with different hierarchy");

	// Contour properties:

	// 1. Center of Mass or Centroid
	// Find all contours in the image
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	imageCopy.copyTo(image);

	// Draw all the contours
	cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);

	// Create object of Moments
	cv::Moments M;

	// Prepare variables for (x, y)-coordinates
	int x, y;
	for(size_t i{0}; i < contours.size(); ++i)
	{
		// Use the contour moments to find the centroid
		M = cv::moments(contours[i]);
		x = static_cast<int>(M.m10 / M.m00);
		y = static_cast<int>(M.m01 / M.m00);

		// Draw a circle in the center of centroid
		cv::circle(image, cv::Point(x, y), 10, cv::Scalar(255, 0, 0), -1);

		// Add a number for each contour
		cv::putText(image, std::to_string(i + 1), cv::Point(x + 40, y - 10), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 2);
	}

	cv::imshow("Center", image);
	cv::waitKey(0);
	cv::destroyWindow("Center");

	// 2. Area and perimeter
	double area;
	double perimeter;
	for(size_t i{0}; i < contours.size(); ++i)
	{
		area = cv::contourArea(contours[i]);
		perimeter = cv::arcLength(contours[i], true);
		std::cout << "Contour #" << i + 1 << " has area = " << area <<
			" and perimeter = " << perimeter << std::endl;
	}

	// Using perimeter or area we can sort contours using std::sort and custom comparator, in this case we are sorting from smallest to biggest area
	std::sort(contours.begin(), contours.end(), compareContourAreas);
	imageCopy.copyTo(image);
	for(size_t i{0}; i < contours.size(); ++i)
	{
		cv::drawContours(image, contours, i, cv::Scalar{ 0, 0, 255 }, -1, cv::LINE_AA);
		cv::imshow(std::format("{} Image", i + 1), image);
		cv::waitKey(0);
		cv::destroyAllWindows();
		image = imageCopy.clone();
	}

	// 3. Area and Perimeter
	image = imageCopy.clone();
	cv::findContours(imageGray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	// Create object of rectangle
	cv::Rect rect;
	for(size_t i{0}; i < contours.size(); ++i)
	{
		// Vertical rectangle
		rect = cv::boundingRect(contours[i]);
		cv::rectangle(image, rect, cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
	}

	cv::imshow("Contours with rectangles", image);
	cv::waitKey(0);
	cv::destroyWindow("Contours with rectangles");

	image = imageCopy.clone();

	// Create object of rotated rectangle
	cv::RotatedRect rotated_rect;
	cv::Point2f rect_points[4];
	cv::Mat boxPoints2f, boxPointsCov;
	for(size_t i{0}; i < contours.size(); ++i)
	{
		rotated_rect = cv::minAreaRect(contours[i]);
		cv::boxPoints(rotated_rect, boxPoints2f);
		boxPoints2f.assignTo(boxPointsCov, CV_32S);
		cv::polylines(image, boxPointsCov, true, cv::Scalar(0, 255, 255), 2);
	}

	cv::imshow("Rotated rectangles", image);
	cv::waitKey(0);
	cv::destroyWindow("Rotated rectangles");

	// 4. Fit a circle and ellipse
	imageCopy.copyTo(image);
	cv::Point2f center;
	float radius;
	for(size_t i{0}; i < contours.size(); ++i)
	{
		// Fit a circle
		cv::minEnclosingCircle(contours[i], center, radius);
		cv::circle(image, center, radius, cv::Scalar(125, 125, 125), 2);
	}

	cv::imshow("Fit a circle", image);
	cv::waitKey(0);
	cv::destroyWindow("Fit a circle");

	image = imageCopy.clone();
	cv::RotatedRect rotated_ellipse;
	for(size_t i{0}; i < contours.size(); ++i)
	{
		// Fit an ellipse
		// We can fit an ellipse only when our contour has minimum 5 points
		if(contours[i].size() < 5)
			continue;
		rotated_ellipse = cv::fitEllipse(contours[i]);
		cv::ellipse(image, rotated_ellipse, cv::Scalar(255, 0, 125), 2);
	}

	cv::imshow("Fit an ellipse", image);
	cv::waitKey(0);
	cv::destroyWindow("Fit an ellipse");

	return 0;
}