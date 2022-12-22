/*
 * First Order Derivative Filters:
 * Prewitt Filter:
 * Knowing that what gradients are, we cna guess what kind of filter will help us calculate the X and Y gradients.
 * For the X Gradient (Y has the same logic) at a pixel, we want to find the difference in intensity to the right and left of the 
 * current pixel. The following convolution filter achieves that: [-1, 0, 1].
 * It is possible to improve the above gradient. First, it calculates the difference between just two pixels. As a rule of thumb,
 * when we do calculations based on a small number of pixels, our estimates are noisy. So the above filter can be enlarged to
 * calculate the gradient over a 3x3 patch instead of over a line. The filters below provide a slightly less noisy version of the
 * gradients in the x- and y-directions (The below filters are called the Prewitt filters).
 */
//tex:
//$$\begin{bmatrix}-1 & 0 & 1\\-1 & 0 & 1\\-1 & 0 & 1\end{bmatrix}\:\:\begin{bmatrix}-1 & -1 & -1\\0 & 0 & 0\\1 & 1 & 1\end{bmatrix}$$

/*
 * Sobel filter
 * To make gradient calculations even more robust and noise-free, the image can be Gaussian-blurred slightly before applying a
 * gradient filter. We have to remember that blurring is also a convolution operation. So applying a Gaussian blur filter before
 * applying the gradient filter would require two convolution operations.
 * But there is a way to use just a single convolution operation to blur and the calculate the gradient, because convolution is associative.
 * Therefore, it is possible to combine two convolution operations into one. This is where the Sobel Filter come in. They perform Gaussian
 * smoothing implicitly. The Sobel filters for X Gradient and Y Gradient calculations are shown below: 
 */
 //tex:
 //$$\begin{bmatrix}-1 & 0 & 1\\-2 & 0 & 2\\-1 & 0 & 1\end{bmatrix}\:\:\begin{bmatrix}-1 & -2 & -1\\0 & 0 & 0\\1 & 2 & 1\end{bmatrix}$$

/*
 * Magnitude and Direction of Gradient
 * We have two different gradients - the gradient in the x-direction Ix and the gradient in the y-direction, Iy. We can think of the gradients
 * as the a-vector with x and y components. The combined strength of the X and Y Gradients is given by the magnitude of gradient:
 */
//tex:
//$$G=\sqrt{I^2_x+I^2_y}$$
// The direction is given by:
//$$\theta=arctan\frac{I_y}{I_x}$$
//The magnitude and direction of gradient is calculated at every pixel. Ix and Iy are the images obtained by running the Sobel filters
// for X and Y Gradients of the image.

/*
 * Sobel Gradients in OpenCV
 * OpenCV provides a Sobel() function for calculating the X and Y Gradients.
 * Syntax:
 *	void cv::Sobel(InputArray src, OutputArray dst, int ddepth, int dx, int dy, int ksize=3,
 *				   double scale=1, double delta=0, int borderType=BORDER_DEFAULT)
 * Parameters:
 *	- src - input image.
 *	- dst - output image of the same size and the same number of channels as src.
 *	- ddepth - output image depth, in the case of 8-bit input images it will return in truncated derivatives (so CV_32F is recommended)
 *	- dx - order of the derivative x.
 *	- dy - order of the derivative y.
 *	- ksize - size of the extended Sobel kernel, it must be 1, 3, 5, or 7.
 *	- scale - optional scale factor for the computed derivative values; by default, no scaling is applied.
 *	- delta - optional delta value that is added to the results prior to storing them is dst.
 *	- borderType - pixel extrapolation method.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	// Load an image from disk in grayscale
	cv::Mat image{ cv::imread("../data/images/truth.png", cv::IMREAD_GRAYSCALE) };

	// The X and Y Gradients are calculate using Sobel function.
	// Depth of the output images is set to CV_32F
	cv::Mat sobelX, sobelY;

	// Apply Sobel filter with only x gradient
	cv::Sobel(image, sobelX, CV_32F, 1, 0);

	// Apply Sobel filter only y gradient
	cv::Sobel(image, sobelY, CV_32F, 0, 1);

	// For display purposes, the image is normalized by scaling and shifting so that all pixel values lie between 0 and 1
	cv::normalize(sobelX, sobelX, 0, 1, cv::NORM_MINMAX);
	cv::normalize(sobelY, sobelY, 0, 1, cv::NORM_MINMAX);

	cv::imshow("Original Image", image);
	cv::imshow("X Gradients", sobelX);
	cv::imshow("Y Gradients", sobelY);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}