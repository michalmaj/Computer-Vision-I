

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	// read the image from disk
	cv::Mat boy{ cv::imread("../data/images/boy.jpg") };

	/*
	* Many definitions of contrast exist in literature. In simple terms, it is the difference in intensity between the brightest and darkest regions of an image. Higher the difference, higher is the contrast.
	* The maximum contrast of an image is also known as Dynamic Range. In an image with good contrast, objects are easily discernible.One of the easiest ways to improve contrast of an image is Intensity Scaling.
	* In this approach, a scale factor (alpha) is multiplied with intensity values of all the pixels. Given below is the code snippet to do the same. Intensity scaling is represented by the following equation:
	*/
	//tex:
   //$$I_o = \alpha I$$

	// Contrast enhancement
	// Increase contrast by 50%
	double contrast_percentage{ 50 };

	cv::Mat image32F_bad = boy.clone();
	cv::Mat image32f = boy.clone();
	cv::Mat high_contrast_bad;
	cv::Mat hight_contrast;

	image32F_bad.convertTo(image32F_bad, CV_32F); // Convert without mapping to [0, 1]
	image32f.convertTo(image32f, CV_32F, 1 / 255.0); // In this case we are using scale factor (1/255.0)

	// Multiply with scaling factor to increase contrast
	high_contrast_bad = image32F_bad * (1 + contrast_percentage / 100.0);
	hight_contrast = image32f * (1 + contrast_percentage / 100.0);

	cv::imshow("Original 32 bit", image32f);
	cv::imshow("Incorrect 32 bit high contrast", high_contrast_bad);
	cv::imshow("Correct 32 bit high contrast", hight_contrast);
	cv::waitKey(0);
	cv::destroyAllWindows();

	/*
	 * Brightness is a measure of light falling on the scene. In RGB color space, it can be thought of as the arithmetic mean of the R, G and B color values. To make an image brighter, the intensity values should be
	 * increased by some offset (beta) and vice-versa.
	 * If (I) is the input image, and (Io) is the output image, brightness enhanced image is given by the equation:
	 */

	//tex:
	// $$I_o = I + \beta$$

	// Brightness enhancement
	// Increase the brightness of the image by 100
	int brightness_offset{ 100 };

	cv::Mat bright32f;
	cv::Mat brightHighChannels32f[3];

	// Incorrect way
	cv::Mat temp_bright = image32f.clone();
	// Add brightness offset to the image directly
	temp_bright += brightness_offset;

	// Correct way
	// First split channels
	cv::split(image32f, brightHighChannels32f);

	// Second add brightness offset to each channels
	for (size_t i{ 0 }; i < 3; ++i)
		cv::add(brightHighChannels32f[i], brightness_offset / 255.0, brightHighChannels32f[i]);

	// Third merge back
	cv::merge(brightHighChannels32f, 3, bright32f);

	cv::imshow("Original 32 bit", image32f);
	cv::imshow("Incorrect Brightness Enhanced", temp_bright);
	cv::imshow("Correct Brightness Enhanced", bright32f);
	cv::waitKey(0);
	cv::destroyAllWindows();

	/*
	 * Note that the colors appear washed. This is because after performing addition, some pixel intensities has values greater than 1. Meaning the image is not in correct range of [0,1], which means all the values that are
	 * above 1 will be mapped to 255. Making the image appear washed. We can tone the image by normalizing it to the correct range. We need to divide the matrix by the maximum value to normalize the image. It can be obtained
	 * by the function minMaxLoc as shown below.
	 */

	// Performing normalization
	// Add the offset for increasing brightness to the 32 bit image.
	cv::Mat bright_normalized_32F;
	cv::Mat brightHighChannels_norm_32F[3];

	// Split image
	cv::split(image32f, brightHighChannels_norm_32F);

	// Add brightness to each channel
	for (size_t i{ 0 }; i < 3; ++i)
		cv::add(brightHighChannels_norm_32F[i], brightness_offset / 255.0, brightHighChannels_norm_32F[i]);

	// Merge channels
	cv::merge(brightHighChannels_norm_32F, 3, bright_normalized_32F);

	// Get maximum pixel intensity
	double minVal, maxVal;
	cv::minMaxLoc(bright_normalized_32F, &minVal, &maxVal);
	std::cout << "Minimum pixel intensity: " << minVal << ", maximum pixel intensity: " << maxVal << std::endl;

	bright_normalized_32F /= maxVal;

	cv::imshow("Brightness Enhanced", bright32f);
	cv::imshow("Normalized Brightness Enhanced", bright_normalized_32F);
	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}