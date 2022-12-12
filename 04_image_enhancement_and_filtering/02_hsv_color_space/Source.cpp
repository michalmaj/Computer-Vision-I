/*
 * The HSV color space:
 * This is the most popular color space used in image processing after the RGB color space. There are three components:
 *	1. Hue - indicates the color/tint of the pixel.
 *	2. Saturation - indicates the purity (or richness) of the color.
 *	3. Value - indicates the amount of brightness of the pixel.
 *
 * Hue:
 * The hue channel refers to the color and its values, ranging from 0 to 180 in OpenCV. Since the HSV color space is represented
 * in a cylindrical coordinate system, the values from Hue wrap around 180. For example, the Hue for red color is near 180. So,
 * some tints of red can wrap around 180 and have values around 0.
 *
 * Saturation:
 * Saturation refers to how pure the color is. Pure red has high saturation. Different shades of a color correspond to different
 * saturation levels. Saturation of 0 corresponds to white color which indicates that the color shade is at the lowest ir the
 * color is absent.
 *
 * Value:
 * Value refers to lightness or brightness. It indicates how dark or bright the color is. It also signifies the amount of light
 * that might have fallen on the object.
 *
 * The HSV color space converts the RGB color space from cartesian coordinates (x, y, z) to cylindrical coordinates (ρ, φ, z).
 * It is more intuitive than the RGB color space because it separates the color and brightness into different axes. This makes it
 * easier to describe any color directly.
 *
 * OpenCV library contains a cvtColor() function for conversions between color spaces.
 * Syntax:
 *	void cv::cvtColor(InputArray src, OutputArray dst, int code, int dstCn=0)
 * Parameters:
 *	- src - input image: CV_8U or CV_16UC or single-precision floating point;
 *	- dst - output image of the same size and depth as src;
 *	- code - color space conversion code (https://docs.opencv.org/4.x/d8/d01/group__imgproc__color__conversions.html);
 *	- dstCn - number of channels in the destination image; if the parameter is 0, the number of the channels is derived automatically
 *			  from src and code.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <format>

int main()
{
	// Load an image from disk
	cv::Mat bgr{ cv::imread("../data/images/capsicum.jpg") };

	// Convert to HVS color space
	cv::Mat hsv;
	cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);

	// Show images
	cv::imshow("BGR", bgr);
	cv::imshow("HSV", hsv);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Split channels of HSV image
	cv::Mat hsvChannels[3];
	cv::split(hsv, hsvChannels);

	// Show each channel separately
	// We to to notice that:
	// 1. Red pepper in the middle has a very high (180) and very low (0) Hue value.
	cv::imshow("Hue Channel", hsvChannels[0]);

	// 2. Green and red peppers are highly saturated (these colors are in their purest form).
	// The yellow pepper has relatively low saturation.
	cv::imshow("Saturation Channel", hsvChannels[1]);

	// 3. Value channels of red and yellow peppers are much brighter as compared to the green pepper.
	cv::imshow("Value Channel", hsvChannels[2]);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Tests

	// 1. Value test - all components of HSV color space are 0 at the beginning. We will keep the H and S channels as 0,
	// and increment the V channel.
	cv::Mat imH, imS, imV, imhsv, imbgr;
	std::vector<cv::Mat> channels;

	for(int i{0}; i < 7; ++i)
	{
		// Create 250x250 HSV channels
		// Set Hue=0, Saturation=0, and Value to i * 40;
		imH = cv::Mat::zeros(cv::Size(250, 250), CV_8U);
		imS = cv::Mat::zeros(cv::Size(250, 250), CV_8U);
		imV = cv::Mat::ones(cv::Size(250, 250), CV_8U) * (i * 40);

		channels = { imH, imS, imV };

		cv::merge(channels, imhsv);

		// Convert from HSV to BGR
		cv::cvtColor(imhsv, imbgr, cv::COLOR_HSV2BGR);

		// Show images:
		// Value controls the brightness of the image. When value is 0, the image is black and when it close
		// to 255, the image is white.
		cv::imshow(std::format("V={}", i * 40), imbgr);
	}
	cv::waitKey(0);
	cv::destroyAllWindows();

	// 2. Saturation test - we will set H=0, V=128 and increment S from 0 to 240
	for(int i{0}; i < 7; ++i)
	{
		// Create 250x250 HVS channels
		// Set Hue=0 and Value=128
		imH = cv::Mat::zeros(cv::Size(250, 250), CV_8U);
		imS = cv::Mat::ones(cv::Size(250, 250), CV_8U) * (i * 40);
		imV = cv::Mat::ones(cv::Size(250, 250), CV_8U) * 128;

		channels = { imH, imS, imV };

		// Merge HVS channels
		cv::merge(channels, imhsv);

		// Convert from HSV to BGR
		cv::cvtColor(imhsv, imbgr, cv::COLOR_HSV2BGR);

		// Show images:
		// Saturation controls  how rich a color is. If the color faded, it is less saturated. When it is intense
		// and deep, it is more saturated. When saturation is 0, we lose all color information and the image looks
		// like grayscale.
		cv::imshow(std::format("S={}", i * 40), imbgr);
	}
	cv::waitKey(0);
	cv::destroyAllWindows();

	// 3. Hue test - we will set S=128, V=128 and increment Hue from 0 to 180
	for(int i{0}; i < 7; ++i)
	{
		// Create 250x250 HSV channels
		// Set Saturation=128, Value=128
		imH = cv::Mat::ones(cv::Size(250, 250), CV_8U) * (i * 30);
		imS = cv::Mat::ones(cv::Size(250, 250), CV_8U) * 128;
		imV = cv::Mat::ones(cv::Size(250, 250), CV_8U) * 128;

		channels = { imH, imS, imV };

		// Merge HSV channels
		cv::merge(channels, imhsv);

		// Convert from HSV to BGR
		cv::cvtColor(imhsv, imbgr, cv::COLOR_HLS2BGR);

		// Show images:
		// Hue controls the perception of color. It is represented as an angle where a hue of 0 is red, green is 120 degrees
		// (60 in OpenCV), and blue is at 240 degrees (120 in OpenCV). Hue goes from 0 to 180 intensities values where one
		// grayscale intensity change represents 2 degrees.
		// Because hue is angle, we get red color from both H=0 and H=360 (180 in OpenCV representation).
		cv::imshow(std::format("V={}", i * 40), imbgr);
	}
	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}