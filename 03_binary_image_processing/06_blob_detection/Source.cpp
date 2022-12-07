/*
 * Blob:
 * A blob is a group of connected pixels in an image that share some common property (eg. grayscale value).
 *
 * Simple blob detector:
 * OpenCV provides a functionality to detect blobs and filter them based on different characteristics.
 *
 * How does Blob detector work:
 * SimpleBlobDetector is controlled by parameters passed to this algorithm:
 *	1. Threshold - convert the source image to several binary images by threshold the source image with thresholds
 *				   starting at minThreshold. These thresholds are incremented by thresholdStep until maxThreshold.
 *	2. Grouping - in each binary image, connected white pixels are grouped together.
 *	3. Merging - the centers of the binary blobs in the binary images are computed and blobs located closer than
 *				 minDistBetweenBlobs are merged.
 *	4. Center and Radius Calculation: the centers and radii of the new merged blobs are computed and returned.
 *
 * The parameters for SimpleBlobDetector can be set to filter the type of blobs we want:
 *  1. By color - first we need to set filterByColor=1, then set blobColor=0 to select darker blobs or
 *				  blobColor=255 for lighter blobs.
 *	2. By size - we can filter the blobs based on size by setting the parameters filterByArea=1, and appropriate
 *				 values for minArea and maxArea. For example minArea=100 will filter out all the blobs that have
 *				 less than 100 pixels.
 *	3. By shape - shape have three different parameters:
 *				  * Circularity - this measures how close to a circle the blob is. For example regular hexagon has
 *								  higher circularity than say a square. To filter by Circularity set filterByCircularity=1
 *								  Then set appropriate value for minCircularity and maxCircularity:
 *								  Circularity = (4*pi*area)/(perimeter)^2
 *								  This means that a circle has a circularity of 1, circularity of a square is 0.785.
 *				  * Convexity - is defined as:
 *								Area of the blob / Area of convex hull
 *								Convex Hull of shape is the tightest convex shape that completely enclose the shape. To
 *								filter by convexity we have to set filterByConvexity=1, followed by setting:
 *								0 <= minConvexity <= 1 and maxConvexity <= 1
 *				  * Inertia ratio - measures how elongated the shape is. For example a circle is 1, for an ellipse is
 *									between 0 and 1, for a line is 0. To filter by inertia ratio we have to set
 *									filterByInertia=1, and set 0 <= minInertiaRation <= 0 and maxInertiaRatio <= 1. 
 */


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

 // Set simple function to modify SimpleBlobDetector params
cv::SimpleBlobDetector::Params parameters()
{
	// Create SimpleBlobDetection object for parameters
	cv::SimpleBlobDetector::Params params;

	// Change threshold
	params.minThreshold = 10;
	params.maxThreshold = 200;

	// Filter by area
	params.filterByArea = true;
	params.minArea = 150; // Everything below this value will be discarded

	// Filter by circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.1f; // For value that will by close to 1, the blob must be more circular

	// Filter by convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.87f;

	// Filter by inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01f;


	return params;
}

int main()
{

	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/blob_detection.jpg", cv::IMREAD_GRAYSCALE) };
	cv::Mat imageClone = image.clone();

	// Set up detector with default parameters
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create();
	std::vector<cv::KeyPoint> keypoints;
	detector->detect(image, keypoints);

	// Mark blobs using image annotation concepts
	int x, y;
	int radius;
	double diameter;
	cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
	for(size_t i{0}; i < keypoints.size(); ++i)
	{
		cv::KeyPoint k = keypoints.at(i);
		cv::Point keyPt{ k.pt };
		x = static_cast<int>(keyPt.x);
		y = static_cast<int>(keyPt.y);

		// Mark center in black
		cv::circle(image, cv::Point(x, y), 5, cv::Scalar(0, 0, 0), -1);

		// Get radius of blob
		diameter = k.size;
		radius = static_cast<int>(diameter / 2.0);

		// Mark blob in green
		cv::circle(image, cv::Point(x, y), radius, cv::Scalar(0, 255, 0), 2);
	}

	cv::imshow("Image", image);
	cv::waitKey(0);
	cv::destroyWindow("Image");


	// SimpleBlobDetector custom parameters
	imageClone.copyTo(image);
	detector = cv::SimpleBlobDetector::create(parameters());
	detector->detect(image, keypoints);

	cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);

	for(size_t i{0}; i < keypoints.size(); ++i)
	{
		cv::KeyPoint k = keypoints.at(i);
		cv::Point keyPt{ k.pt };
		auto [x1, y1] = keyPt;

		// Mark center in black
		cv::circle(image, cv::Point(x1, y1), 5, cv::Scalar(0, 0, 0), -1);

		// Calculate radius
		diameter = k.size;
		radius = static_cast<int>(diameter / 2.0);

		// Mark blob in green
		cv::circle(image, cv::Point(x1, y1), radius, cv::Scalar(0, 255, 0), 2);
	}

	cv::imshow("Image", image);
	cv::waitKey(0);
	cv::destroyWindow("Image");


	return 0;
}