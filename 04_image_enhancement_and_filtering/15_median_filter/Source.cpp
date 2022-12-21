/*
 * Median filter:
 * Median blur filtering is a non-linear filtering technique that in most commonly used to remove salt-and-pepper noise from images.
 * As the name suggests, salt-and-pepper noise shows up as randomly occurring white and black pixels that are sharply different
 * from the surrounding. In color images, salt-and-pepper noise may appear as small random color spot.
 *
 * OpenCV provides implementation of the median blur, where a square kernel is used. THe dimension of the square must be an odd integer
 * greater than 1. For an image, the median blurring filter replaces the value of central pixel with the median of all the pixels within
 * the kernel. For example consider a 3x3 patch in a grayscale image:
 */
//tex:
//$$\begin{bmatrix}31 & 32 & 29\\31 & 255 & 35\\35 & 30 & 33\end{bmatrix}$$

// Notice, the value at the center pixel is way higher than the neighbors. It is likely that the center pixel is corrupted with
// salt-nad-pepper noise.
// If we use a box filter to remove this noise, the central pixel will be average of all pixels in the 3x3 neighborhood. The center
// pixel value after box filtering will be:
//tex:
//$$\frac{30+32+29+31+255+35+35+30+33}{9}=57$$
//$$\begin{bmatrix}31 & 32 & 29\\31 & 57 & 35\\35 & 30 & 33\end{bmatrix}$$

// Which is better than before, but still high compared to the neighboring pixels.
// Let's try a median filter. The median filter takes in only the pixels values in the neighborhood of the central pixel, sorts
// the list of values and then replaces the value of the central pixel with the median value of the sorted list. In this case
// sorted list looks:
//tex:
//[29, 30, 30, 32, 32, 33, 35, 35, 255]

// Thus, the median value is 32, and we replace the central pixel with 32:
//tex:
//$$\begin{bmatrix}31 & 32 & 29\\31 & 32 & 35\\35 & 30 & 33\end{bmatrix}$$

/*
 * Median blur in OpenCV:
 * Median blur is implemented in the library as a medianBlur() function.
 * Syntax:
 *	void cv::medianBlur(InputArray src, OutputArray dst, int ksize)
 * Parameters:
 *  - src - input 1-, 3-, 4-channel image; when kernel size is 3 or 5, the image depth should be:
 *			CV_8U, CV_16U, CV_32F, for larger sizes, it can only be CV_8U.
 *	- dst - destination image of the same size and type as src;
 *	- ksize - aperture linear size, it must be odd and greater than 1 integer, for example: 3, 5, 7, ...
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/salt-and-pepper.png") };

	// Define the kernel size
	int kernelSize{ 5 };

	// Perform median blurring
	cv::Mat medianBlurred;
	cv::medianBlur(image, medianBlurred, kernelSize);

	// Show results
	cv::imshow("Original Image", image);
	cv::imshow("Median Blur Result: Kernel Size = 5", medianBlurred);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}