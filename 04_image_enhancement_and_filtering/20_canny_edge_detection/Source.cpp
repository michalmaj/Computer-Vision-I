/*
 * Canny Edge Detection:
 * Canny edge detector is the most widely-used edge detector. For many applications that require edge detection, Canny
 * edge detection is sufficient. It has the following three steps:
 *	1. Gradient calculation: Edges are pixels where intensity changes abruptly. The magnitude of the gradient is very
 *	   high at the edge pixels. Therefore, gradient calculation is the first step in Canny edge detection.
 *	2. Non-maxima suppression: In the real world, the edges in an image are not sharp. The magnitude of gradient is
 *	   very high not only at the real edge location, but also is a small neighborhood around it. We want to represent
 *	   an edge by a single pixel (pixel thick contour). Simply thresholding the gradient leads to a thicker contour that
 *	   contains more pixels. Fortunately, this problem can be eliminated by selecting the pixel with maximum gradient
 *	   magnitude is a small neighborhood (say 3x3) of every pixel in the gradient image. The name non-maxima suppression
 *	   comes from the fact that we eliminate (i.e. set to zero) all gradients except the maximum one in small 3x3
 *	   neighborhoods over the entire image.
 *	3. Hysteresis thresholding: After non-maxima suppression, we could threshold the gradient image to obtain a new binary
 *	   image which is black in all places except for pixels where the gradient is very high. This kind of thresholding
 *	   would naively exclude a lot of edges because, in real world images, edges tend to fade in and out along their length.
 *	   For example, an edge may be strong in the middle but fade out at the two ends. To fix this problem, Canny edge detection
 *	   uses two thresholds. First, a higher threshold is used to select pixels with very high gradients. We say these pixels
 *	   have a strong edge. Second, a lower threshold is used to obtain new pixels that are potential edge pixels. We say these
 *	   pixels have a weak edge. A weak edge pixel can be re-classified as a strong edge if one of its neighbors is a strong
 *	   edge. The weak edges that are not reclassified as strong are dropped from the final edge map.
 *
 * Canny Edge Detection in OpenCV
 * OpenCV provides function named Canny():
 * Synatax:
 *	void cv::Canny(InputArray dx, InputArray dy, OutputArray edges, double threshold1, double threshold2, bool L2gradient=false)
 * Parameters:
 *	- dx - 16-bit x derivative of input image (CV_16SC1 or CV_16SC3).
 *	- dy - 16-bit y derivative of input image (same type as dx).
 *	- edges - output edge map; single channels 8-bit image, which has the same size as image.
 *	- threshold1 - first threshold for the hysteresis procedure.
 *	- threshold2 - second threshold for the hysteresis procedure.
 *	- L2gradient - a flag, indicating whether a more accurate L2 norm should be used to calculate the image gradient magnitude.
 *				   the default (false) calculates L1 norm.
 */
//tex: L1 norm
//$$L1\;norm=\lvert\frac{dl}{dx}\rvert+\lvert\frac{dl}{dy}\rvert$$
// L2 norm
//$$L2\;norm=\sqrt{\left(\frac{dl}{dx}\right)^2+\left(\frac{dl}{dy}\right)^2}$$

/*
 * Deep dive into Canny Parameters
 *	1. lowThreshold: keeping all other parameters constant, when we lower the lowerThreshold, broken edges tend to get connected
 *	   If we increase it, continuous edges may break.
 *	2. highThreshold: Keeping all other parameters constant, when we increase highThreshold, fewer edges are detected. On the other
 *	   hand, decreasing highThreshold leads to more edges.
 *	3. apertureSize: Increasing the aperture size leads to many more edges. This is simple because larger Sobel kernels return
 *	   larger gradient values. Low and high thresholds should be changed when aperture size is changed.
 *	4. blurAmount: When the blur amount is increased, noise in the image is reduced, and spurious edges are removed. As a result,
 *	   fewer edges are detected.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

/**
 * \brief Structure to control four parameters
 *			- lowThreshold
 *			- high threshold
 *			- apertureSize (3, 5, 7)
 *			- blur amount
 */
struct CannyParams
{
	// Variables for source and edge images
	cv::Mat src, edges;

	// Variables for low and high thresholds
	int lowThreshold{ 50 };
	int highThreshold{ 100 };

	// Max track-bar value
	const int maxThreshold{ 1000 };

	// Sobel aperture sizes for Canny edge detector
	const std::vector<int> apertureSizes{ 3, 5, 7 };
	int maxapertureIndex{ 2 };
	int apertureIndex{ 0 };

	// Gaussian blur size
	int blurAmount{ 0 };
	int maxBlurAmount{ 20 };
};

/**
 * \brief This function is called whenever any trackbar value is changed. The image is firs blurred. The amount of blur
 *		  depends on blurAmount. A Sobel apertureSize(3, 5, 7) is chosen based on the trackbar value. Finally, the Canny
 *		  function is called and results displayed
 * \param userdata Object of CannyParam structure
 */
void applyCanny(int, void* userdata)
{
	auto cp = static_cast<CannyParams*>(userdata);

	// variable to store blurred image
	cv::Mat blurredSrc;

	// Blur the image before edge detection
	if (cp->blurAmount > 0)
		cv::GaussianBlur(cp->src, blurredSrc, cv::Size(2 * cp->blurAmount + 1, 2 * cp->blurAmount + 1), 0);
	else
		blurredSrc = cp->src.clone();

	// Canny requires aperture size to be odd
	int apertureSize = cp->apertureSizes[cp->apertureIndex];

	// Apply Canny to get the edges
	cv::Canny(blurredSrc, cp->edges, cp->lowThreshold, cp->highThreshold, apertureSize);

	// Display image
	cv::imshow("Edges", cp->edges);
}

int main()
{
	CannyParams cp;
	

	// Load an image from disk in grayscale
	cp.src = cv::imread("../data/images/sample.jpg", cv::IMREAD_GRAYSCALE);

	// Display images
	cv::imshow("Edges", cp.src);

	// Create a window to display output
	cv::namedWindow("Edges");


	// Trackbar to control the low threshold
	cv::createTrackbar("Low Threshold", "Edges", &cp.lowThreshold, cp.maxThreshold, applyCanny, &cp);

	// Trackbar to control the high threshold
	cv::createTrackbar("High Threshold", "Edges", &cp.highThreshold, cp.maxThreshold, applyCanny, &cp);

	// Trackbar to control the aperture size
	cv::createTrackbar("Aperture Size", "Edges", &cp.apertureIndex, cp.maxapertureIndex, applyCanny, &cp);

	// Trackbar to control blur
	cv::createTrackbar("Blur", "Edges", &cp.blurAmount, cp.maxBlurAmount, applyCanny, &cp);

	cv::waitKey(0);

	cv::destroyAllWindows();

	return 0;
}