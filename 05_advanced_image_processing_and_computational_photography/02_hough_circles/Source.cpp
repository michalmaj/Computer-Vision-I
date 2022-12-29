/*
 * Hough Transform Detect Circles
 * In the case of line Hough transform, we required two parameters, (θ,ρ) but to detect circles, we require three parameters:
 *	- (x, y) coordinates of the center of the circle.
 *	- radius.
 * So the circle detector will require a 3D accumulator - one for each parameter.
 * We have to perform the following steps to detect circles in an image:
 *	1. Find the edges in a given image with the help of edge detectors (Canny).
 *	2. For detecting circles in an image, we set a threshold for the maximum and minimum value of the radius.
 *	3. Evidence is collected in a 3D accumulator array for the presence of the circles with different centers and radii.
 *
 * In OpenCV we have a function HoughCircles() to detect circles in an image:
 * Syntax:
 *	void cv::HoughCircles(InputArray image, OutputArray circles, int method, double dp, double minDist,
 *						  double param1=100, double param2=100, int mixRadius=0, int maxRadius=0)
 * Parameters:
 *	- image - 8-bit, single channel binary source image.
 *	- circles - output vector of found circles. Each vector is encoded as 3 or 4 element floating-point vector (x, y, radius)
 *	     		or (x, y, radius, votes).
 *	- method - detection method. Currently, the only implemented method is HOUGH_GRADIENT.
 *	- dp - inverse ratio of the accumulator resolution to the image resolution. For example, if dp=1, the accumulator has the
 *		   same resolution as the input image. If dp=2, the accumulator has half as big width and height.
 *	- minDist - minimum distance between the centers of the detected circles. If it too large some circles may be missed.
 *	- param1 - First method-specific parameter. In case of HOUGH_GRADIENT, it is the higher threshold of the two passed to the
 *			   Canny edge detector (the lower one is twice smaller).
 *	- param2 - Second method-specific parameter. In case of HOUGH_GRADIENT, it is the accumulator threshold for the circle
 *			   centers at the detection stage. The smaller it is, the more false circles may be detected. Circles, corresponding
 *			   to the larger accumulator values, will be returned first.\
 *	- minRadius - minimum circle radius.
 *	- maxRadius - maximum circle radius. If <=0, uses the maximum image dimension. If < 0, return centers without finding the radius. 
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

int main()
{
	// Load an image from disk
	cv::Mat img{ cv::imread("../data/images/circles.jpg", cv::IMREAD_COLOR) };

	// Convert to gray-scale
	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	// Blur the image to reduce noise
	cv::Mat imgBlur;
	cv::medianBlur(gray, imgBlur, 5);

	// Apply Hough Transform on the image
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(imgBlur, circles, cv::HOUGH_GRADIENT, 1, 50, 450, 10, 30, 40);

	// Draw circles and (x, y)-point
	for(size_t i{0}; i < circles.size(); ++i)
	{
		cv::Point center{ cvRound(circles[i][0]), cvRound(circles[i][1]) };
		int radius{ cvRound(circles[i][2]) };

		// Draw the outer circle
		cv::circle(img, center, radius, cv::Scalar(0, 255, 0), 2);

		// Draw the center of the circle
		cv::circle(img, center, 2, cv::Scalar(0, 0, 255), -1);
	}

	// Show the image
	cv::imshow("Circles", img);
	cv::waitKey(0);
	cv::destroyWindow("Circles");


	return 0;
}