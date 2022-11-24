#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

int main()
{
	// Load image from disk
	cv::Mat img{ cv::imread("../data/images/musk.jpg") };

	// Convert image to CV_32F
	img.convertTo(img, CV_32F, 1 / 255.0);

	// Load sunglasses with alpha channel
	cv::Mat glassPNG{ cv::imread("../data/images/sunglass.png", cv::IMREAD_UNCHANGED) };

	// Convert this image to CV_32F as well
	glassPNG.convertTo(glassPNG, CV_32F, 1 / 255.0);

	// resize the image to fit over the eye region
	cv::resize(glassPNG, glassPNG, cv::Size(), 0.5, 0.5);

	std::cout << "Image dimension: " << glassPNG.size() << std::endl;
	std::cout << "Number of channels: " << glassPNG.channels() << std::endl;

	auto [glassWidth, glassHeight] = glassPNG.size();

	// Separate the color and alpha channels
	cv::Mat glassBGRAChannels[4];
	cv::Mat glassBGRChannels[3];

	cv::split(glassPNG, glassBGRAChannels); // Split into 4 separated channels

	for(size_t i{0}; i < 3; ++i)
	{
		// Copy B, G, R channels from BGRA to BGR
		glassBGRChannels[i] = glassBGRAChannels[i];
	}

	cv::Mat glassBGR, glassMask1;
	// Prepare BGR image
	cv::merge(glassBGRChannels, 3, glassBGR);
	// Alpha channel is the 4th channel in BGRA image
	glassMask1 = glassBGRAChannels[3];

	cv::imshow("Musk", img);
	cv::imshow("Sun Glasses", glassBGR);
	cv::imshow("Glass Mask", glassMask1);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Naive version - just make o copy
	cv::Mat faceWithGlassesNaive = img.clone();

	// Because some compilers when we are using std::move make a copy instead of reference, we have to do something else
	cv::Mat roiFace = faceWithGlassesNaive(cv::Range(150, 250), cv::Range(140, 440)); // Creates a clone() in msc

	// We can copy glasses directly to the face image
	//glassBGR.copyTo(faceWithGlassesNaive(cv::Range(150, 150 + glassHeight), cv::Range(140, 140 + glassWidth)));

	// Replace the eye region with the sunglasses image
	glassBGR.copyTo(roiFace);
	// Or we can use roi first and then copy roi to the original image
	roiFace.copyTo(faceWithGlassesNaive(cv::Range(150, 150 + glassHeight), cv::Range(140, 140 + glassWidth)));
	cv::imshow("Naive Method", faceWithGlassesNaive);
	cv::waitKey(0);
	cv::destroyWindow("Naive Method");

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

	// 1. Make the dimension of the mask same as the input image.
	// Since Face Image is a 3-channel image, we create a 3 channels image from the mask





	//cv::CascadeClassifier faceCascade;
	//faceCascade.load("haarcascade_frontalface_default.xml");
	//std::vector<cv::Rect> faces;
	//faceCascade.detectMultiScale(img, faces, 1.3, 5);

	//std::cout << faces[0].tl() << " " << faces[0].br() << std::endl;

	//cv::rectangle(img, faces[0].tl(), faces[0].br(), cv::Scalar(255, 0, 255), 3);
	//cv::imshow("Img", img);
	//cv::waitKey(0);
	//cv::destroyWindow("Img");

	return 0;
}