/*
 * Application - Face Blending
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

int main()
{
	// Load source image
	cv::Mat src{ cv::imread("../data/images/obama.jpg") };

	// Load destination image
	cv::Mat dst{ cv::imread("../data/images/trump.jpg") };

	// Load mask image in gray-scale
	cv::Mat src_mask{ cv::imread("../data/images/obama-mask.jpg", cv::IMREAD_GRAYSCALE) };

	// Show images
	cv::imshow("Trump", dst);
	cv::imshow("Obama", src);
	cv::imshow("Obama mask", src_mask);
	cv::waitKey(0);
	cv::destroyAllWindows();

	/*
	 * First attempt using simple alpha blending with mask:
	 *	1. The lightning in the image is very different.
	 *	2. The skin tones are very different.
	 *	3. The blend will look ridiculous.
	 */
	cv::Mat alpha;
	cv::cvtColor(src_mask, alpha, cv::COLOR_GRAY2BGR);
	alpha.convertTo(alpha, CV_32FC3);
	cv::normalize(alpha, alpha, 0, 1, cv::NORM_MINMAX);

	src.convertTo(src, CV_32FC3);
	dst.convertTo(dst, CV_32FC3);

	// Split images into channels
	std::vector<cv::Mat> srcChannels(3);
	std::vector<cv::Mat> dstChannels(3);
	std::vector<cv::Mat> alphaChannels(3);
	std::vector<cv::Mat> outputBlendChannels(3);

	cv::split(src, srcChannels);
	cv::split(dst, dstChannels);
	cv::split(alpha, alphaChannels);

	for (size_t i{ 0 }; i < outputBlendChannels.size(); ++i)
		outputBlendChannels.at(i) = srcChannels.at(i).mul(alphaChannels.at(i)) + dstChannels.at(i).mul(1 - alphaChannels.at(i));

	cv::Mat output_blend;
	cv::merge(outputBlendChannels, output_blend);

	output_blend.convertTo(output_blend, CV_8UC3);

	cv::imshow("Ridiculous Result", output_blend);
	cv::waitKey(0);
	cv::destroyWindow("Ridiculous Result");

	// Second attempt using Seamless Cloning

	// Find center of the mask
	cv::Mat src_mask_bin;

	// Convert ot binary image
	cv::threshold(src_mask, src_mask_bin, 128, 255, cv::THRESH_BINARY);

	// Find blob centroid
	cv::Moments m{ cv::moments(src_mask_bin) };
	std::cout << "Center of the mask [x: " << (m.m01 / m.m00) << ", y: " << (m.m10/m.m00) << "]" << std::endl;

	// Make Point from moments
	cv::Point center(m.m01 / m.m00, m.m10 / m.m00);

	// Perform Seamless Cloning
	cv::Mat output_clone;
	src.convertTo(src, CV_8UC3);
	dst.convertTo(dst, CV_8UC3);

	// Clone seamlessly
	cv::seamlessClone(src, dst, src_mask, center, output_clone, cv::NORMAL_CLONE);

	// Show images
	cv::imshow("Trump", dst);
	cv::imshow("Obama", src);
	cv::imshow("Seamless Cloning", output_clone);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}