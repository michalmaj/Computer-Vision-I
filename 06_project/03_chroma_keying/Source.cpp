#include <iostream>
#include <vector>
#include <xtensor/xarray.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// Parameters for events
struct ChromaParameters
{
	cv::Mat frame; // matrix for image
	const std::string WINDOW_NAME{ "Frame" }; // name of the window
	cv::Scalar color{ 0, 0, 0 }; // For color picker
	std::vector<uchar> u_green{ 97, 200, 78 };
	std::vector<uchar> l_green{ 60, 30, 0 };
};

void mouseBGR(int event, int x, int y, int flags, void* userdata)
{
	auto cp = static_cast<ChromaParameters*>(userdata);

	if(event == cv::EVENT_LBUTTONDOWN)
	{
		cp->color = cp->frame.at<cv::Vec3b>(x, y);
		cv::imshow(cp->WINDOW_NAME, cp->frame);
	}
}



int main()
{
	ChromaParameters cp;
	cv::VideoCapture cap{ "../data/videos/greenscreen-demo.mp4" };

	// Check to see if camera opened successfully
	if(!cap.isOpened())
	{
		std::cout << "Error opening video stream or file!" << std::endl;
		return -1;
	}

	/*cap.read(cp.frame);
	cv::setMouseCallback(cp.WINDOW_NAME, on_mouse_click, &cp.frame);*/
	cv::namedWindow(cp.WINDOW_NAME);

	while (cap.isOpened())
	{
		cap >> cp.frame;
		if (cp.frame.empty())
			break;

		std::cout << cp.color << std::endl;

		cv::Mat mask;
		cv::inRange(cp.frame, cp.l_green, cp.u_green, mask);

		cv::Mat res;
		cv::bitwise_and(cp.frame, cp.frame, res, mask);

		cv::Mat f = cp.frame - res;

		cv::imshow(cp.WINDOW_NAME, cp.frame);
		cv::imshow("Mask", f);
		cv::setMouseCallback(cp.WINDOW_NAME, mouseBGR, &cp.frame);
		cv::waitKey(25);
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}