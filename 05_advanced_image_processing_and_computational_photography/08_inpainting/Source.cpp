/*
 * Image Inpainting
 * Image inpainting is a class of algorithms in computer vision where the objective is to fill regions inside an image or a video.
 * The region is identified using a binary mask, and the filling is usually done by propagating information from the boundary of
 * the region that need to be filled.
 * The most common application of image inpainting is restoration of old scanned photos. It is also used for removing small
 * unwanted objects in an image.
 *
 * In OpenCV library we have two algorithms: Navier-Stokes and Fast Marching Method, for these we have a function inpaint()
 * Syntax:
 *	void inpaint(InputArray src, InputArray inpaintMask, OutputArray dst, double inpaintRadius, int flags)
 * Parameters:
 *	- src - source image.
 *	- inpaintMask - a binary mask indicating pixels to be inpainted.
 *	- dst - destination image.
 *	- inpaintRadius - neighborhood around pixel to inpaint. Typically, if the regions to be inpainted are thin, smaller values
 *		   			  produce better results (less blurry).
 *	- flags - INPAINT_NS (Navier-Stokes based method) or INPAINT_TELEA (Fast Marching Based Method).
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/photo.hpp>

struct InpaintParameters
{
	// Declare Mat object for original image
	cv::Mat img;

	// Declare Mat object for mask
	cv::Mat inpaintMask;

	// Declare Mat object for result output
	cv::Mat res;

	// Set points
	cv::Point prevPt{-1, -1};
};

// onMouse function for Mouse Handling
// Used to draw regions required to inpaint
void onMouse(int event, int x, int y, int flags, void* userdata)
{
	auto ip{ static_cast<InpaintParameters*>(userdata) };

	if (event == cv::EVENT_LBUTTONUP or !(flags & cv::EVENT_FLAG_LBUTTON))
		ip->prevPt = cv::Point{ -1, -1 };
	else if (event == cv::EVENT_LBUTTONDOWN)
		ip->prevPt = cv::Point{ x, y };
	else if(event == cv::EVENT_MOUSEMOVE and (flags & cv::EVENT_FLAG_LBUTTON))
	{
		cv::Point pt{ x, y };
		if (ip->prevPt.x < 0)
			ip->prevPt = pt;
		cv::line(ip->inpaintMask, ip->prevPt, pt, cv::Scalar::all(255), 5, 8, 0);
		cv::line(ip->img, ip->prevPt, pt, cv::Scalar::all(255), 5, 8, 0);
		ip->prevPt = pt;
		cv::imshow("Image", ip->img);
		cv::imshow("Image: Mask", ip->inpaintMask);
	}
}

int main()
{
	InpaintParameters ip;

	// Read image in color mode
	ip.img = cv::imread("../data/images/Lincoln.jpg", cv::IMREAD_COLOR);

	// Check to see if image is loaded
	if(ip.img.empty())
	{
		std::cout << "Failed to load image" << std::endl;
		return -1;
	}

	// Create a copy of the original image
	cv::Mat img_mask = ip.img.clone();

	// Create a named window
	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);

	// Initialize mask
	ip.inpaintMask = cv::Mat::zeros(img_mask.size(), CV_8U);

	// Show the original image
	cv::imshow("Image", ip.img);

	// Set mouse callback
	cv::setMouseCallback("Image", onMouse, &ip);

	while(true)
	{
		char c = static_cast<char>(cv::waitKey());

		if(c == 't')
		{
			// Use algorithm proposed by Alexandra Telea
			cv::inpaint(ip.img, ip.inpaintMask, ip.res, 3, cv::INPAINT_TELEA);
			cv::imshow("Inpaint Output using FMM", ip.res);
		}

		if(c == 'n')
		{
			// Use algorithm proposed by Bertalmio et. al.
			cv::inpaint(ip.img, ip.inpaintMask, ip.res, 3, cv::INPAINT_NS);
			cv::imshow("Inpaint Output using NS Technique", ip.res);
		}

		if(c == 'r')
		{
			ip.inpaintMask = cv::Scalar::all(0);
			img_mask.copyTo(ip.img);
			cv::imshow("Image", ip.img);
		}

		if(c == 27) // ESC
			break;
	}

	cv::destroyAllWindows();

	return 0;
}