#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <format>
#include <string>

// Helper class to store source image and points for rectangle 
class MouseParams
{
public:
	cv::Mat source;
	cv::Point topLeft{-1, -1};
	cv::Point bottomRight{-1, -1};
};

void drawRectangle(int action, int x, int y, int flags, void* userdata)
{
	// Get access to members of MouseParams class.
	auto mp = static_cast<MouseParams*>(userdata);

	// If both points (topRight and bottomLeft) have correct data, don't draw another rectangle
	if (action == cv::EVENT_LBUTTONDOWN and mp->topLeft.x != -1 and mp->bottomRight.x != -1)
	{
		std::cout << "Rectangle created, wait for fresh copy of the image!" << std::endl;
		return;
	}

	// If left mouse button is pressed and topLeft point doesn't contain proper data
	if(action == cv::EVENT_LBUTTONDOWN and mp->topLeft.x == -1)
	{
		mp->topLeft = cv::Point(x, y);
		cv::circle(mp->source, mp->topLeft, 1, cv::Scalar(255, 0, 255), -1, cv::LINE_AA);
		return;
	}

	// If left mouse button is pressed and topLeft point contains proper data
	if(action == cv::EVENT_LBUTTONDOWN and mp->topLeft.x != -1)
	{
		mp->bottomRight = cv::Point(x, y);
		cv::circle(mp->source, mp->bottomRight, 1, cv::Scalar(255, 0, 255), -1, cv::LINE_AA);

		cv::rectangle(mp->source, mp->topLeft, mp->bottomRight, cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
		cv::imshow("Window", mp->source);
	}


}


int main()
{
	// Create an object to share source image and points (top left and bottom right coordinates)
	MouseParams mp;

	// Load an image from disk
	mp.source = cv::imread("../data/boy.jpg");

	// Copy an original image will be useful to clear the drawings 
	cv::Mat dummy{ mp.source.clone() };

	cv::namedWindow("Window");

	cv::setMouseCallback("Window", drawRectangle, &mp);

	int k{ 0 };
	while(k!=27)
	{
		cv::imshow("Window", mp.source);
		k = cv::waitKey(10) & 0xFF;

		// If 'c' clear annotations and set topLeft and bottomRight to (-1,-1)
		if (k == 'c')
		{
			dummy.copyTo(mp.source);
			mp.topLeft = cv::Point(-1, -1);
			mp.bottomRight = cv::Point(-1, -1);
		}

		// If 's' save cropped region
		if(k == 's')
		{
			// check if both points are set up
			if(mp.topLeft.x == -1 or mp.bottomRight.x == -1)
			{
				std::cout << "Create a rectangle firs!" << std::endl;
				continue;
			}
			size_t num{ 0 };
			cv::Mat new_image = mp.source(cv::Range(mp.topLeft.y, mp.topLeft.x), cv::Range(mp.bottomRight.y, mp.bottomRight.x));
			std::string name = std::format("boy_annotate_{}.png", num);
			cv::imwrite(name, new_image);
			num++;
		}
	}

	cv::destroyAllWindows();

	return 0;
}