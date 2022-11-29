/*
 * Get adn set properties of VideoCapture
 * It's possible to get some extra properties from video using cap.get(propID). We can also set
 * these properties using cap.set(propID, value). In both cases propID stands for the PropertyID
 * (https://docs.opencv.org/4.1.0/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d)
 * and value in setter is the value we want to set for the property (by propID).
 *
 * List of some common properties and their ID:
 *		Enumerator						Numerical value						Property
 *		CAP_PROP_POS_MSEC				0									Current position of the video file in ms
 *		CAP_PROP_FRAME_WIDTH			3									Width of the frame in the video stream
 *		CAP_PROP_FRAME_HEIGH			4									Height of the frame in the video stream
 *		CAP_PROP_FPS					5									Frame rate
 *		CAP_PROP_FOURCC					6									4-character code of codec
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Capture the video
	cv::VideoCapture cap("../data/chaplin.mp4");

	// Check if camera opened successfully
	if (!cap.isOpened())
	{
		std::cout << "Width of the frame in the video stream" << std::endl;
		return 0;
	}

	// Getting the video properties
	int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	std::cout << "Width of the video: " << width << ", height: " << height << std::endl;

	// Read first frame
	cv::Mat frame;
	cap >> frame;

	cv::imshow("First frame", frame);
	cv::waitKey(0);
	cv::destroyWindow("First frame");

	// Setting the video properties
	/*
	 * Warning!
	 * It might happen that the set property function gives unexpected results. This is because the setting of property may be disabled for the file or web cam.
	 * For example, you might not be able to set the properties for the video file that we are working with, but you can set the width and height of your web cam input.
	 *
	 * Even for the webcam, it is not possible to set arbitrary values. Whatever values you set should be supported by the device.
	 * ( For example, the default resolution of my web cam is 720x1280. If I want to set it to 200x200, it might not happen and get set to something else, say 640x480, which is supported by the web cam. )
	 */

	// Set position of video to 2.5 seconds
	cap.set(cv::CAP_PROP_POS_MSEC, 2500); // last parameter is in milliseconds

	//// Width
	//cap.set(cv::CAP_PROP_FRAME_WIDTH, 320); // Runtime error - can't change the size of video file! 
	//cap.set(cv::CAP_PROP_FRAME_HEIGHT, 180); // Runtime error - can't change the size of video file!

	cap >> frame;

	cv::imshow("Frame at 2.5s", frame);
	cv::waitKey(0);
	cv::destroyWindow("Frame at 2.5s");

	return 0;
}