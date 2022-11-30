/*
 * Graphical User Interface in OpenCV
 * OpenCV contains Highgui module for handle GUI.
 *
 * Highgui module provides an interfaqce to:
 *  - create and manipulate windows that can display images and "remember" their content;
 *	- add track-bars to the window, handle simple mouse events as well as keyboard commands;
 *	- read and write images to/from disk or memory;
 *	- read video from camera or file and write video to a file.
 *
 * Keyboard usage in OpenCV:
 * Getting the input from the keyboard is done using the cv::waitKey() function.
 * Syntax:
 *	int cv::waitKey(int delay=0)
 * Parameters
 *	- delay - delay in milliseconds, 0 is the special value that means "forever".
 *
 * If no key is pressed cv::waitKey() function returns -1
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	// Open web-cam (0 parameter in cv::VideoCapture object)
	cv::VideoCapture cap(0);

	// If no camera detected or something is wrong with the input exit program
	if(!cap.isOpened())
	{
		std::cout << "Unable to detect web-cam" << std::endl;
		return 0;
	}

	// Create cv:Mat to store frames
	cv::Mat frame;

	// Create variable to get keyboard input
	int k{ 0 };


	// Get input from camera and show it until 'ESC' button is pressed
	while(true)
	{
		cap >> frame;

		// The following series of if statements is used to check which key
		// is pressed. We use a waitKey() function to detect the input and
		// respond only if one of the following letters: 'e', 'E', 'z', Z'
		// are pressed.

		// If 'ESC' pressed break from the loop ('ESC' in ASCII code is 27)
		if(k == 27)
			break;

		// Check if 'e' or 'E' is pressed
		if (k == 101 or k == 69)
			cv::putText(frame, "E is pressed", cv::Point(100, 180), cv::FONT_HERSHEY_COMPLEX,
				1, cv::Scalar(0, 255, 0), 3);

		// Check if 'z' or 'Z' is pressed (we can either use ASCII code: 'z' = 122, 'Z' = 90 or simply characters like 'z' or 'Z')
		if (k == 'z' or k == 'Z')
			cv::putText(frame, "Z is pressed", cv::Point(100, 180), cv::FONT_HERSHEY_COMPLEX,
				1, cv::Scalar(0, 0, 255), 3);
		

		cv::imshow("Frame", frame);
		k = cv::waitKey(25) & 0xFF;

		
	}


	cap.release();
	cv::destroyAllWindows();

	return 0;
}