/*
 * Mouse callback in OpenCV
 * It is possible to detect mouse events like left-click, right-click or position of the mouse on the window.
 * For these operations we have to create a named window and assign a callback function to the window.
 *
 * In this case we have a drawCirce function, it is our callback function. It is called when there is a mouse
 * event like left click (cv::EVENT_LBUTTONDOWN). The coordinates relative to the namedWindow is captured by
 * this function in the variables (x, y). The function records the points of the circle's center and a point
 * on the circumference, hence allowing us to draw the desired circle on the image.
 *
 * For using a callback function we can use a cv::setMouseCallback function cv::setMouseCallback
 * Syntax:
 *	void cv::setMouseCallback(const String& winame, MouseCallback onMouse, void* userdata=0)
 * Parameters:
 *	- winname - name of the window
 *	- onMouse - callback function for mouse events
 *	- userdata - the optional parameter passed to the callback
 */

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// Class storing parameters 
class MouseParams
{
public:
	// Source image
	cv::Mat source;

	// Points to store the center of the circle
	cv::Point center;

	// Point on the circumference
	cv::Point circumference;
};

/**
 * \brief Function which will called on mouse input. It is the callback function associated with namedwindow.
 * \param action Store mouse event like left click (cv::EVENT_LBUTTONDOWN).
 * \param x x-coordinate captured by this function.
 * \param y y-coordinate captured by this function.
 * \param flags optional parameter.
 * \param userdata Lets us send object of MouseParams to work on the same data.
 */
void drawCircle(int action, int x, int y, int flags, void* userdata)
{
	// We can share same object contains source image, and points (center and circumference)
	auto mp = static_cast<MouseParams*>(userdata);

	// If left mouse button is pressed
	if(action == cv::EVENT_LBUTTONDOWN)
	{
		mp->center = cv::Point(x, y);

		// Mark the center
		cv::circle(mp->source, mp->center, 1, cv::Scalar(255, 255, 0), 2, cv::LINE_AA);
	}
	// If left mouse button is released
	else if(action == cv::EVENT_LBUTTONUP)
	{
		mp->circumference = cv::Point(x, y);

		// Calculate radius of the circle
		double radius{ sqrt(pow(mp->center.x - mp->circumference.x, 2) + pow(mp->center.y - mp->circumference.y, 2)) };

		// Draw the circle
		cv::circle(mp->source, mp->center, radius, cv::Scalar(255, 255, 0), 2, cv::LINE_AA);

		// Show image
		cv::imshow("Window", mp->source);
	}
}


int main()
{
	// Create object that store source image, and two points (center and circumference) to share with our callback function
	MouseParams mp;

	// Read the image from disk
	mp.source = cv::imread("../data/boy.jpg");

	// Copy clear image, will be useful to clear the drawing
	cv::Mat dummy = mp.source.clone();

	// Set name of the window
	cv::namedWindow("Window");

	// Highgui function called when mouse events occur
	cv::setMouseCallback("Window", drawCircle, &mp);

	int k = 0;
	// loop until escape character is pressed
	while (k != 27)
	{
		cv::imshow("Window", mp.source);
		cv::putText(mp.source, "Choose center, and drag, Press ESC to exit and c to clear", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
		k = cv::waitKey(20) & 0xFF;
		if (k == 99) // 'c' is ASCII = 99
			// Another way of cloning
			dummy.copyTo(mp.source);
	}

	cv::destroyAllWindows();
	return 0;
}