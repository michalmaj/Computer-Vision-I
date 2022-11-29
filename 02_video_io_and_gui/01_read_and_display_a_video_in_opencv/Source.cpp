/*
 * For reading a video we will use a VideoCapture() function from OpenCV library.
 * Syntax:
 *	cv::VideoCapture (const String& filename, int apiPreference=CP_ANY)
 * Parameters:
 *	- filename - it can be:
 *		* name of video file (eg. video.avi)
 *		* image sequence (eg. img_%02d.jpg, which will read samples like img_00.jpg, img_01.jpg, img_02.jpg, ...)
 *		* URL of video stream
 *	- apiPreference - preferred Capture API backed to use. Can be used to enforce a specific reader implementation if
 *	  multiple are available: e.g. cv::CAP_FFMPEG or cv::CAP_IMAGES or cv::CAP_DSHOW (https://docs.opencv.org/3.4/d4/d15/group__videoio__flags__base.html).
 *
 * Create a video reader object
 *	cv::VideoCapture cap(args)
 * Three most popular ways of reading video arfe:
 *	1. Using a web-cam (pass 0 as a argument)
 *	2. Video file (pass video name as argument)
 *	3. Sequence of images
 *
 * Reading frames:
 *	If we want to read the video frames, we simply use the cap.read method which returns a pair where the first element
 *	is a return vale and the second element is the frame.
 *
 * Displaying the frames as video
 *	Displaying next frames is like displaying images,we have to use a cv::imshow() function and cv::waitKey() to pause
 *	each frame in the video. For case of an image, we use a 0 to this function, but for playing video, we need to pass
 *	a number greater than 0. If we pass 0 we will pause the frame in the video for an infinite amount of time. So the
 *	number is equal to the time in milliseconds we want each frame to be displayed.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	cv::VideoCapture cap{ "../data/chaplin.mp4" };

	// Check if camera opened successfully using a isOpend() method
	if (!cap.isOpened())
		std::cout << "Error opening video stream or file!" << std::endl;

	// Because we are reading a video file, we will read until video is completed
	while(cap.isOpened())
	{
		cv::Mat frame; // Place for frames

		// Capture frame by frame
		cap >> frame;

		// If the frame is emmpty, break from the loop
		if(frame.empty())
			break;

		// Show frame
		cv::imshow("Frame", frame);

		// Wait for 25 milliseconds before moving on to the next frame
		cv::waitKey(25);
	}

	cap.release();
	cv::destroyAllWindows();



	

	return 0;
}