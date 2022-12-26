/*
 * Image Sharpening:
 * In sharpening we want to enhance the edges and bring out more of the underlying texture. Although, sharpening can be
 * performed using a single convolution filter:
 *	1. Step 1: blur the image to smooth out texture. The blurred image contains low frequency information of the original image.
 *			   Let I be the original image and Ib be the blurred image.
 *	2. Step 2: Obtain the high frequency information of the original image by subtracting the blurred image from original image.
 *	3. Step 3: Put back the high frequency information back in the image and control the amount using a parameter. The final
 *			   sharpened image is therefore:
 */
//tex:
//$$I_s=I+\alpha(I-I_b)$$
// In practice, sharpening is implemented using a simple sharpening kernel that approximates the above behavior. All we need to
// do is convolve the image with the following kernel:
//$$\begin{bmatrix}0 & -1 & 0\\-1 & 5 & -1\\0 & -1 & 0\end{bmatrix}$$
// The above kernel is obtained using α=1 and approximating (I-Ib) using Laplacian kernel:
//$$\begin{bmatrix}0 & 0 & 0\\0 & 1 & 0\\0 & 0 & 0\end{bmatrix}+\begin{bmatrix}0 & -1 & 0\\-1 & 4 & -1\\0 & -1 & 0\end{bmatrix}$$


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main()
{
	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/sample.jpg") };

	// Define the kernel
	cv::Mat sharpen{ {3, 3}, {0, -1, 0, -1, 5, -1, 0, -1, 0} };
	std::cout << sharpen;

	// Apply convolution
	// In this case filter2D is used to perform the convolution
	// The third parameter in this function (depth) is set to -1 which means the bit-depth of the output image is the same as the input image.
	// So if the input image is of type CV_8C3, the output image will also be the same type
	cv::Mat sharpenOutput;
	cv::filter2D(image, sharpenOutput, -1, sharpen);

	// Show images
	cv::imshow("Original image", image);
	cv::imshow("Sharpening result", sharpenOutput);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}