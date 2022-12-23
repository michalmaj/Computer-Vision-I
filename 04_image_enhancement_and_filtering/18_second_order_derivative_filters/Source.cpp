/*
 * Second Order  Derivative Filters:
 * Sobel operator calculates the first derivative in the x- and y-direction. When the intensity changes abruptly, the Sobel filter
 * response fires up, so the response of the Sobel filter can be used as an edge detector.
 * However, first derivative operators (like the Sobel operator) are not the only way to detect edges. Second derivative operators
 * have a zero crossing at the location of the edges. The Laplacian is a filter that is based on the second derivative.
 */
//tex:Laplacian Filter is given by:
//$$L(x,y)=\frac{\partial^2I}{\partial^2x}+\frac{\partial^2I}{\partial^2y}$$
// This expression can be reduce to the simple convolution operation, with kernel given by:
//$$\begin{bmatrix}0 & 1 & 0\\1 & -4 & 1\\0 & 1 & 0\end{bmatrix}$$
/*
 * The Laplacian filter is very sensitive to noise and therefore it is important to smooth the image before applying it.
 * As a rule of thumb, summing and averaging operations are less affected by noise, and differencing operations are
 * greatly affected by noise, for example:
 * Consider two numbers. One is supposed to be 10 and the other is supposed to be 11, but because of a noise, the second
 * number is recorded as 12. Now, the true difference between the numbers is 11 - 10 = 1. But because of noise, the difference
 * is 12 - 10 = 2. So we have made a mistake of 100% while calculating the difference!
 * On the other hand, the true sum of the two numbers is 11 + 10 = 21, but because of noise we estimate it to be 12 + 10 = 22.
 * The error is (22 - 21) / 21 = 4.76%.
 * So, while calculating derivatives we have to worry about noise, but while calculating integrals we don't have to worry about
 * noise.
 *
 * Laplacian in OpenCV
 * OpenCV provides a function Laplacian()
 * Syntax:
 *	void cv::Laplacian(InputArray src, OutputArray dst, int ddepth, int ksize=1, double scale1=,
 *					   double delta=0, int borderType=BORDER_DEFAULT)
 * Parameters:
 *	- src - source image.
 *	- dst - destination image of the same size and the same number of channels.
 *	- ddepth - desired depth of the destination image (CV_32F recommended).
 *	- ksize - aperture size used to compute the second-derivative filters, the size must be positive and odd.
 *	 - scale - optional scale factor for the computed Laplacian values. By default, no scaling is applied.
 *	 - delta - optional delta value that is added to the result prior to storing them is dst.
 *	 - borderType - pixel extrapolation method.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	// Load an image from disk in grayscale
	cv::Mat image{ cv::imread("../data/images/sample.jpg", cv::IMREAD_GRAYSCALE) };

	// Before using a second derivative via Laplacian blur the image using Gaussian blur.
	cv::Mat blurred;
	cv::GaussianBlur(image, blurred, cv::Size(3, 3), 0, 0);

	// Apply Laplacian on blurred image
	cv::Mat LOG;
	int kernelSize{ 3 };
	cv::Laplacian(blurred, LOG, CV_32F, kernelSize, 1, 0);

	// Normalize the image
	cv::normalize(LOG, LOG, 0, 1, cv::NORM_MINMAX);

	// Show images
	cv::imshow("Original Image", image);
	cv::imshow("After Blurred", blurred);
	cv::imshow("After Laplacian", LOG);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}