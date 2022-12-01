/*
 * Trackbars in OpneCV
 * For createTrackbar() function we have to specify a named window and we have to prepare a callback function.
 * That function needs to be specified for detecting events on the trackbar.
 *
 * The trackbars are created using cv::createTrackbar() function:
 * Syntax:
 *	int cv::createTrackbar(const String& trackbarname, const String& winname, int* value, int count,
 *						   TrackBarCallback onChange=0, void* userdata=0)
 * Parameters:
 *	- trackbarname - name of the created trackbar.
 *	- winname - name of the window that will be used as a parent of the created trackbar.
 *	- value - optional pointer to an integer variable whose value reflects the position of the slider. Upon
 *			  creation, the slider position is defined by this variable.
 *	- count - maximal position of the slider. The minimal position is 0.
 *	- onChange - pointer to the function to be called  every time the slider changes position. This function
 *				 should be prototyped as void Foo(int void*), where the first parameter is the trackbar position
 *				 and the second is the userdata.
 *	- userdata - User data that is passed as is to the callback. It can be used to handle trackbar events without
 *				 using global variables.
 *
 * In this case we will create two trackbars:
 *	1. For getting the scaling type.
 *	2. For getting the percentage of scaling to be done.
 */

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// Structure helper to handle global variables.
struct TrackBarsParams
{
	// 
	int maxScaleUp; // maximum value for the scale trackbar
	int scaleFactor; // variable where the value will be updated whenever there is an event on the trackbar
	int scaleType; // variable where the value will be updated whenever there is an event on the trackbar
	int maxType; // maximum value for the type trackbar
	std::string windowName; // is the name of the window where the trackbars are displayed
	cv::Mat im; // cv::Mast to store image 

	// Constructor with default values.
	TrackBarsParams(int mS=100, int sF=1, int sT=0, int mT=1, std::string wN="Resize Image")
		: maxScaleUp(mS), scaleFactor(sF), scaleType(sT), maxType(mT), windowName(wN)
	{}
};

void scaleImage(int, void* userdata);

int main()
{
	TrackBarsParams tbp;

	// Names of two trackbars
	std::string trackbarValue = "Scale";
	std::string trackbarType = "Type: \n 0: Scale Up \n 1: Scale Down";


	// load an image
	tbp.im = cv::imread("../data/truth.png");

	// Create a window to display results
	cv::namedWindow(tbp.windowName, cv::WINDOW_AUTOSIZE);

	// Create Trackbars and associate a callback function:

	// Create trackbar to choose percentage of scaling
	cv::createTrackbar(trackbarValue, tbp.windowName, &tbp.scaleFactor, tbp.maxScaleUp, scaleImage, &tbp);
	// Create trackbar to choose type of scalling (up or down)
	cv::createTrackbar(trackbarType, tbp.windowName, &tbp.scaleType, tbp.maxType, scaleImage, &tbp);

	scaleImage(25, &tbp);

	while (true) {
		int c;
		c = cv::waitKey(20);
		if (static_cast<char>(c) == 27) // Hit 'ESC' to break from the loop
			break;
	}

	cv::destroyAllWindows();

	return 0;
}

void scaleImage(int, void* userdata)
{
	auto tbp = static_cast<TrackBarsParams*>(userdata);

	double scaleFactorDouble;

	if(tbp->scaleType)
		scaleFactorDouble = 1 - (tbp->scaleFactor) / 100.0;
	else
		scaleFactorDouble = 1 + (tbp->scaleFactor) / 100.0;

	if (scaleFactorDouble == 0)
		scaleFactorDouble = 1;

	cv::Mat scaledImage;

	cv::resize(tbp->im, scaledImage, cv::Size(), scaleFactorDouble, scaleFactorDouble, cv::INTER_LINEAR);
	cv::imshow(tbp->windowName, scaledImage);

}