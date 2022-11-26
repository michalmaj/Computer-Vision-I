/*
 * OpenCV library contains 4 bitwise operations:
 * AND: cv::bitwise_and
 * OR: cv::bitwise_or
 * NOT: cv::bitwise_not
 * XOR: cv::bitwise_xor
 *
 * Syntax:
 *	void cv::bitwise_XXX(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray())
 * Parameters:
 *	- src1 - first input
 *	- src2 - second input
 *	- dst - output array that has the same size and type as the input array
 *	- mask - optional operation mask, 8-bit single channel array, that specifies elements of the output array to be changed.
 *	  The operations is applied only on those pixels of the input images where the mask is non-zero.
 *
 * The operation is applied element-wise between two matrices. The two inputs should be of the same size for this operation.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main()
{
	// Load images (face and sunglasses)

	// Load Face Image
	cv::Mat faceImage{ cv::imread("../data/images/musk.jpg") };

	// Make a copy
	cv::Mat faceWithGlassesBitwise = faceImage.clone();

	// Load sunglasses Image with Alpha channel
	cv::Mat glassPNG{ cv::imread("../data/images/sunglass.png", cv::IMREAD_UNCHANGED) };

	// Resize the sunglasses image to fit over the eye region
	cv::resize(glassPNG, glassPNG, cv::Size(300, 100));

	// Get width and height of the sunglasses image
	auto [width, height] = glassPNG.size();

	std::cout << "Sunglasses height: " << height << ", width: " << width << ", number of channels: " << glassPNG.channels() << std::endl;

	// Separate BRG colors and alpha channel
	cv::Mat glassBGRAChannels[4];
	cv::Mat glassBGRChannels[3];
	cv::split(glassPNG, glassBGRAChannels);
	// Copy B, G, R channels
	for (size_t i{ 0 }; i < 3; ++i)
		glassBGRChannels[i] = glassBGRAChannels[i];

	// Prepare BGR and mask images 
	cv::Mat glassBGR, glassMask1;

	// BGR
	cv::merge(glassBGRChannels, 3, glassBGR);

	// Mask
	glassMask1 = glassBGRAChannels[3];

	cv::imshow("Glass BGR", glassBGR);
	cv::imshow("Mask", glassMask1);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Get the masked outputs
	// get the eye region from the face image
	cv::Mat eyeROI = faceWithGlassesBitwise(cv::Range(150, 150 + height), cv::Range(140, 140 + width));

	// Make the dimensions of the sunglasses mask as the input image
	cv::Mat glassMask;
	cv::Mat glassMaskChannels[]{ glassMask1, glassMask1, glassMask1 };
	cv::merge(glassMaskChannels, 3, glassMask);

	// use the mask to create the masked eye region
	cv::Mat eye;
	cv::Mat glassMaskNOT;
	// Swap white to black and black to white from original image
	cv::bitwise_not(glassMask1, glassMaskNOT);

	cv::imshow("Glass Mask Not", glassMaskNOT);
	cv::waitKey(0);
	cv::destroyWindow("Glass Mask Not");

	cv::Mat eyeChannels[3];
	cv::Mat eyeROIChannels[3];
	cv::Mat maskedGlass;
	cv::Mat eyeRoiFinal;

	cv::split(eyeROI, eyeROIChannels);

	// Add mask to eye roi
	for (int i{ 0 }; i < 3; ++i)
		cv::bitwise_and(eyeROIChannels[i], glassMaskNOT, eyeROIChannels[i]);

	cv::merge(eyeROIChannels, 3, eye);

	cv::imshow("Eye with glass mask", eye);
	cv::waitKey(0);
	cv::destroyWindow("Eye with glass mask");

	// Make the dimensions of the sunglasses mask not as the input image
	cv::Mat glassMatNOTChannels[]{ glassMaskNOT, glassMaskNOT, glassMaskNOT };
	cv::Mat glassMaskNOTMerged;
	cv::merge(glassMatNOTChannels, 3, glassMaskNOTMerged);

	cv::bitwise_and(eyeROI, glassMaskNOTMerged, eye);

	cv::imshow("Eye", eye);
	cv::waitKey(0);
	cv::destroyWindow("Eye");

	// Use the mask to create the masked sunglasses region
	cv::Mat sunglass;
	cv::Mat sunglassChannels[3];

	cv::split(glassBGR, glassBGRChannels);

	for (size_t i{ 0 }; i < 3; ++i)
		cv::bitwise_and(glassBGRChannels[i], glassMask1, sunglassChannels[i]);

	cv::merge(sunglassChannels, 3, sunglass);

	// combine the sunglasses in the eye region to get the augmented image
	cv::bitwise_or(eye, sunglass, eyeRoiFinal);

	cv::imshow("Eye", eye);
	cv::imshow("Sunglasses", sunglass);
	cv::imshow("Eye ROI", eyeRoiFinal);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Final result
	eyeRoiFinal.copyTo(faceWithGlassesBitwise(cv::Range(150, 150 + height), cv::Range(140, 140 + width)));
	cv::imshow("Face with Sunglasses", faceWithGlassesBitwise);
	cv::waitKey(0);
	cv::destroyWindow("Face with Sunglasses");

	return 0;
}