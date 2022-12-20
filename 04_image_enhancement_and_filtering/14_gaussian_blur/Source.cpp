/*
 * Gaussian blur:
 * The box kernel weights the contribution of all pixels in the neighborhood equally. A Gaussian Blur
 *  kernel, on the other hand, weights the contribution of a neighboring pixels based on the distance
 *	of the pixel from the center pixel.
 *
 *	The bell curve or the Gaussian is given by the following equation:
 */
//tex:
//$$G(x)=\frac{1}{\sqrt{2\pi\sigma^2}}exp^\frac{-x^2}{2\sigma^2}$$

// The shape of the curve is controlled a single parameter called σ that controls the peakiness of the bell curve.
//tex:
//$$G(x)=\frac{1}{\sqrt{2\pi\sigma^2}}exp^\frac{x^2+y^2}{2\sigma^2}$$

// The Gaussian kernel is based on the same equation, except it is in 2D, so instead of a curve we can think of an
// upside down bowl. A bigger σ creates kernel that blurs more.
// A 5x5 Gaussian kernel with σ=1 is given by:
//tex:
//$$\frac{1}{337}\begin{bmatrix}1 & 4 & 7& 4 & 1\\ 4 & 20 & 33 & 20 & 4\\
//7 & 33 & 55 & 33 & 7\\4 & 20& 33 & 20 & 4\\1 & 4 & 7 & 4 & 1\end{bmatrix}$$

/*
 * Unlike the box kernel, the Gaussian kernel is not uniform. The middle pixel gets the maximum weight while the
 * farther away are given less weight.
 *
 * An image blurred using the Gaussian kernel looks less blurry compared to a box kernel of the same size. Small
 * amount of Gaussian blurring is frequently used to remove noise from an image. It is also applied to the image
 * prior to a noise sensitive image filtering operations. For example, the Sobel kernel used for calculating the\
 * derivative of an image is a combination of a Gaussian kernel and a finite difference kernel.
 *
 * OpenCV provides the GaussianBlur() function to perform blurring:
 * Syntax:
 *	void cv::GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX, double sigmaY=0, int borderType=BORDER_DEFAULT)
 * Parameters:
 *  - src - input image; the image can have any number of channels, which are processed independently, but the depth should be:
 *			CV_8U, CV_16U, CV_16S, CV_32F or CV_64F.
 *	- dst - output image of the same size and type as src.
 *	- ksize - Gaussian kernel size, ksize width and ksize height can be different but the both must be positive and odd. Or, the can
 *			  be zero's and then they are computed from sigma.
 *	- sigmaX - Gaussian kernel standard deviation in x-direction.
 *	- sigmaY - Gaussian kernel standard deviation in y-direction. If sigmaY is zero, it is set to be equal to sigmaX, if both sigmas
 *			   are zero, they are computed from ksize (width and height respectively). To be fully control the result regardless of
 *			   possible future modifications of all this semantics, it is recommended to specify all of ksize, and sigmas.
 *	- borderType - pixel extrapolation method.
 */

// Note:
// 1. In most cases, sigmaX and sigmaY are the same.
// 2. It is much easier to control the Gaussian blurring using just one parameter. Usually, we simply provide the kernel size, and
//	  let OpenCV automatically calculate the optimum σ for based on the following equation:
//tex:
// $$\sigma=0.3*((size-1)*0.5-1)+0.8$$

// 3. If we set sigmaY equal to zero, it is set to sigmaX. If both sigmaX and sigmaY are zero, the above formula is used to calculate σ.

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/gaussian-noise.png") };

	// Apply Gaussian filter with size 5 and size 25 with provided sigmaX and sigmaY
	cv::Mat dst1, dst2;

	cv::GaussianBlur(image, dst1, cv::Size(5, 5), 0, 0);
	cv::GaussianBlur(image, dst2, cv::Size(25, 25), 50, 50);

	// Show images
	cv::imshow("Original Image", image);
	cv::imshow("Box blur Result1: kernel size = 5", dst1);
	cv::imshow("Box blur Result2: kernel size = 25", dst2);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}