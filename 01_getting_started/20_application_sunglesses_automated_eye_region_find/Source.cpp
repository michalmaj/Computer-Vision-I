// Arithmetic version
/*
 * We have to follow these steps:
 *	1. Create a binary mask with 3-channels using single channel mask
 *	2. Find the eye region from the face image (and extract roi, but look out for moving semantics in operator= for cv::Mat).
 *	3. Multiply the Mask with the eye region to create a hole in the region with the sunglasses.
 *	4. Multiply the negative of Mask with the eye region to create a hole in the eye region for the sunglasses to be placed.
 *	5. Add the masked sunglasses and eye region to get the combined eye region with the sunglasses.
 *	6. Replace the eye region in the original image with that of the output we got in the previous step.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

int main()
{
	// Load face image from disk
	cv::Mat faceImage{ cv::imread("../data/images/musk.jpg") };

	// Load sunglasses image with alpha channel
	cv::Mat glassPNG{ cv::imread("../data/images/sunglass.png", cv::IMREAD_UNCHANGED) };

	// Resize the sunglasses image to fit over the eye region
	cv::resize(glassPNG, glassPNG, cv::Size(300, 100));

	// Get width and height of the image
	auto [width, height] = glassPNG.size();

	// Separate BGR and Alpha channel
	cv::Mat glassBGRAChannels[4];
	cv::Mat glassBGRChannels[3];

	// Split BGRA image
	cv::split(glassPNG, glassBGRAChannels);

	// Add BGR channels into glassBGRChannels Mat
	for (size_t i{ 0 }; i < 3; ++i)
		// Copy B, G, R channels from BGRA to BGR
		glassBGRChannels[i] = glassBGRAChannels[i];

	// Prepare BGR image of glasses and cv::Mat for mask
	cv::Mat glassBGR, glassMask1;
	// BGR image, merge channels from glassBGRChannels
	cv::merge(glassBGRChannels, 3, glassBGR);
	// Get alpha channel from last element (4th channel) of glassBGRAChannels
	glassMask1 = glassBGRAChannels[3];

	// Make the dimensions of the mask same as the input image.
	// Since Face Image as a 3-channels image, we create a 3 channel image for the mask
	cv::Mat glassMask;
	cv::Mat glassMaskChannels[]{ glassMask1, glassMask1, glassMask1 };
	cv::merge(glassMaskChannels, 3, glassMask);

	// Make the values 0 or 1
	glassMask /= 255;

	// Make a copy of face image
	cv::Mat faceWithGlassesArithmetic = faceImage.clone();

	// Automated eye region finding
	cv::CascadeClassifier faceCascade;
	faceCascade.load("haarcascade_frontalface_default.xml");
	std::vector<cv::Rect> faces;
	faceCascade.detectMultiScale(faceImage, faces, 1.3, 5);

	std::cout << faces[0].tl() << " " << faces[0].br() << std::endl;

	// y-coordinate is abs(y1 - y2) * 0.3 + y1
	auto [x1, y1] = faces[0].tl();
	auto [x2, y2] = faces[0].br();

	int start_y = static_cast<int>(std::abs(y1 - y2) * 0.3 + y1);
	int start_x = static_cast<int>(std::abs(x2 - 2.6 * x1));


	// Get the eye region from the face image
	cv::Mat eyeROI = faceWithGlassesArithmetic(cv::Range(start_y, start_y + height), cv::Range(start_x, start_x + width));

	cv::Mat eyeROIChannels[3];
	cv::split(eyeROI, eyeROIChannels);

	cv::Mat maskedEye;
	cv::Mat maskedEyeChannels[3];

	// Use a mask to create the masked eye region
	for (size_t i{ 0 }; i < 3; ++i)
		cv::multiply(eyeROIChannels[i], (1 - glassMaskChannels[i]), maskedEyeChannels[i]);

	cv::merge(maskedEyeChannels, 3, maskedEye);

	// Use the mask to create the masked sunglasses region
	cv::Mat maskedGlass;
	cv::multiply(glassBGR, glassMask, maskedGlass);

	// Combine the sunglasses in the eye region tom get augmented image
	cv::Mat eyeROIFinal;
	cv::add(maskedEye, maskedGlass, eyeROIFinal);

	cv::imshow("Masked Eye", maskedEye);
	cv::imshow("Masked Glasses", maskedGlass);
	cv::imshow("Eye final Roi", eyeROIFinal);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Copy back region with glases to the image with entire face
	eyeROIFinal.copyTo(faceWithGlassesArithmetic(cv::Range(start_y, start_y + height), cv::Range(start_x, start_x + width)));
	cv::imshow("Face with glasses", faceWithGlassesArithmetic);
	cv::waitKey(0);
	cv::destroyWindow("Face with glasses");



	return 0;
}




//cv::CascadeClassifier faceCascade;
//faceCascade.load("haarcascade_frontalface_default.xml");
//std::vector<cv::Rect> faces;
//faceCascade.detectMultiScale(img, faces, 1.3, 5);

//std::cout << faces[0].tl() << " " << faces[0].br() << std::endl;

//cv::rectangle(img, faces[0].tl(), faces[0].br(), cv::Scalar(255, 0, 255), 3);
//cv::imshow("Img", img);
//cv::waitKey(0);
//cv::destroyWindow("Img");