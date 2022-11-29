/*
 * Write a video using OpenCV library
 * For saving images we have a cv::imwrite() function in which we can specify format (eg. jpg or png). But for saving
 * videos more information are required. So after capturing and processing the video frame by frame we can use a
 * cv::VideoWriter() function
 * Syntax:
 *	cv::VideoWriter (const String& filename, int fourcc, double fps, Size frameSize, isColor=true)
 * Parameters:
 *	- filename - name of the output video file
 *	- fourcc - 4-character code of codec used to compress the frame. For example, VideoWriter::fourcc('P', 'I', 'M', '1')
 *	  is a MPEG-1 codec, VideoWriter::fourcc('M', 'J', 'P', 'G') is a motion-jpeg codec etc. (https://softron.zendesk.com/hc/en-us/articles/207695697-List-of-FourCC-codes-for-video-codecs)
 *	- fps - frame rate of the created video stream
 *	- frameSize - size of the video frames
 *	- isColor - if it is non-zero, the encoder will expect and encode color frames, otherwise it will work with greyscale
 *	  frames
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	// Read video from file
	cv::VideoCapture cap{ "../data/chaplin.mp4" };

	// Check if camera opened successfully
	if (!cap.isOpened())
	{
		std::cout << "Error opening video stream or file" << std::endl;
		return 0;
	}

	// Get the resolution of the frame
	int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	// cv::Mat to store frames
	cv::Mat frame;
	cap >> frame;
	bool isColor = (frame.type() == CV_8UC3);

	// Define the codec and create VideoWriter object
	cv::VideoWriter out{ "outputChaplin.mp4", cv::VideoWriter::fourcc('M', 'P', 'G', '4'), 10, cv::Size(width, height), isColor};

	// Read until video is completed
	while(cap.isOpened())
	{
		// Capture frame-by-frame
		cap >> frame;

		// If the frame is empty, break from loop
		if(frame.empty())
			break;

		// Write the frame into the file
		out.write(frame);

		// Show the frame
		cv::imshow("Frame", frame);
		cv::waitKey(25);
	}

	cap.release();
	out.release();

	cv::destroyAllWindows();
	return 0;
}