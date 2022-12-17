/*
 * Color Tone Adjustment using Curves
 * In image enhancement, manipulating color channels using curves is a very important technique. It maps the original colors and
 * brightness of an image to values specified by the user. This is one of the mostly used feature in Photoshop. But Photoshop is
 * manual work, why not write a piece of code for doing that?
 */

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


// Function similar to NumPy interp function. We just find the slope between two points (points on the Curve) and use the
// x-value (fullRange) to find the corresponding y-value (lut)
// Piecewise linear interpolation implemented on a particular Channel
void interp(float* fullRange, float* Curve, float* originalValue, uchar* lut)
{
	for(int i{0}; i < 256; ++i)
	{
		int j{ 0 };
		float a{ fullRange[i] };
		while (a > originalValue[j])
		{
			j++;
		}
		if(a == originalValue[j])
		{
			lut[i] = Curve[j];
			continue;
		}
		float slope{ (Curve[j] - Curve[j - 1]) / (originalValue[j] - originalValue[j - 1]) };
		float constant{ Curve[j] - slope * originalValue[j] };
		lut[i] = slope * fullRange[i] + constant;
	}
}


int main()
{
	// Load an image fro disk
	cv::Mat Image{ cv::imread("../data/images/girl.jpg") };

	// Prepare pivot points for x-coordinates
	float originalValue[]{ 0, 50, 100, 150, 200, 255 };

	// Changed points on Y-axis for each channel
	float rCurve[]{ 0, 80, 150, 190, 220, 255 };
	float bCurve[]{ 0, 20, 40, 75, 150, 255 };

	// Splitting channels
	std::vector<cv::Mat> channels(3);
	cv::split(Image, channels);

	// Create a lookup table
	float fullRange[256];
	for(int i{0}; i < std::size(fullRange); ++i)
	{
		fullRange[i] = static_cast<float>(i);
	}
	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* lut = lookUpTable.ptr();

	/*
	 * Interpolate the values specified by the pivot points and store it in the lookUpTable to get a mapping from the original value to
	 * the modified values for each intensity level. Then we use the lookUpTable to map the pixel value in the original image to the
	 * output image for both the channels and merge the modified channels to get the final output.
	 */
	cv::Mat bLUT, rLUT;

	// Apply interpolation na create look up table
	interp(fullRange, bCurve, originalValue, lut);

	// Apply mapping and check for underflow/overflow in Blue channel
	cv::LUT(channels[0], lookUpTable, bLUT);
	cv::min(bLUT, 255, bLUT);
	cv::max(bLUT, 0, bLUT);


	// Apply interpolation and create look up table
 	interp(fullRange, rCurve, originalValue, lut);

	// Apply mapping and check for underflow/overflow in Red Channel
	LUT(channels[2], lookUpTable, rLUT);
	min(rLUT, 255, rLUT);
	max(rLUT, 0, rLUT);

	// Merge channels
	cv::Mat output;
	std::vector<cv::Mat> modifiedChannels{ bLUT, channels[1], rLUT };
	cv::merge(modifiedChannels, output);

	cv::imshow("Original Image", Image);
	cv::imshow("Warm filter Image", output);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Show as one image
	cv::Mat combined;
	cv::hconcat(Image, output, combined);

	cv::imshow("Warm filter output", combined);
	cv::waitKey(0);
	cv::destroyWindow("Warm filter output");

	// Cooling filter 
	// Changed points on Y-axis for each channel
	float bCurve2[] = { 0,80,150,190,220,255 };
	float rCurve2[] = { 0,20,40,75,150,255 };

	// Splitting the channels
	cv::split(Image, channels);

	// Apply interpolation and create look up table
	interp(fullRange, bCurve2, originalValue, lut);

	// Apply mapping and check for underflow/overflow in Blue Channel
	cv::LUT(channels[0], lookUpTable, bLUT);
	cv::min(bLUT, 255, bLUT);
	cv::max(bLUT, 0, bLUT);

	// Apply interpolation and create look up table
	interp(fullRange, rCurve2, originalValue, lut);

	// Apply mapping and check for underflow/overflow in Red Channel
	cv::LUT(channels[2], lookUpTable, rLUT);
	cv::min(rLUT, 255, rLUT);
	cv::max(rLUT, 0, rLUT);

	// Merge the channels 
	modifiedChannels = { bLUT, channels[1], rLUT };
	cv::merge(modifiedChannels, output);

	// Display the images
	cv::hconcat(Image, output, combined);

	cv::imshow("Cool filter output", combined);
	cv::waitKey(0);
	cv::destroyWindow("Cool filter output");

	return 0;
}