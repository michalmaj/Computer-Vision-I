/*
 * Apply Color Tone Adjustment using Curves with xTensor library help
 */
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <xtensor/xtensor.hpp>
#include <xtensor/xarray.hpp>


/**
 * \brief Helper function to convert from xtensor container to cv::Mat.
 * \tparam T Type of values inside xt::xtensor_container.
 * \tparam N Size of give container.
 * \param container Container that will be converted into cv::Mat.
 * \return cv::Mat after conversion.
 */
template<typename T, size_t N>
cv::Mat xt2mat(xt::xtensor_container<T, N>& container)
{
	// Convert from xt::xtensor_container to std::vector
	std::vector<uchar> temp{ container.begin(), container.end() };

	// Create cv::Mat object from std::vector that contains points after interpolation
	cv::Mat lookUpTable(temp);

	// Transpose cv::Mat 
	cv::transpose(lookUpTable, lookUpTable);

	return lookUpTable;
}

int main()
{
	// Load an image fro disk
	cv::Mat image{ cv::imread("../data/images/girl.jpg") };

	// Split image into separate B, G, R channels
	std::vector<cv::Mat> channels(3);
	cv::split(image, channels);

	// Warm filter

	// Set points to change for x-coordinates
	auto originalValues{ xt::xarray<int>{0, 50, 100, 150, 200, 255} };

	// Change points on y-axis for each channel (Red and BLue)
	auto rCurve{ xt::xarray<int>{0, 80, 150, 190, 220, 255} };
	auto bCurve{ xt::xarray<int>{0,20,40,75,150,255} };

	// Create a look up table
	auto fullRange{ xt::arange<float>(0, 256) };

	// Apply interpolation for blue channel
	auto blueInterp{ xt::interp(fullRange, originalValues, bCurve) };

	// Create blue look up table
	cv::Mat bLUT;

	// Create a cv::Mat for look up table
	cv::Mat lookUpTable{ xt2mat(blueInterp) };

	// Apply mapping and check for underflow/overflow in Blue Channel
	cv::LUT(channels[0], lookUpTable, bLUT);
	cv::min(bLUT, 255, bLUT);
	cv::max(bLUT, 0, bLUT);


	// Apply interpolation for red channel
	auto redInterp{ xt::interp(fullRange, originalValues, rCurve) };

	// Create red look up table
	cv::Mat rLUT;
	lookUpTable = xt2mat(redInterp);

	// Apply mapping and check for underflow/overflow in Blue Channel
	cv::LUT(channels[2], lookUpTable, rLUT);
	cv::min(rLUT, 255, rLUT);
	cv::max(rLUT, 0, rLUT);

	// Merge channels to get image with warm filter
	cv::Mat output;

	std::vector<cv::Mat> modifiedChannels{ bLUT, channels[1], rLUT};
	cv::merge(modifiedChannels, output);

	// Combine two images into one
	cv::Mat combined;
	cv::hconcat(image, output, combined);

	cv::imshow("Warming filter applied", combined);
	cv::waitKey(0);
	cv::destroyWindow("Warming filter applied");

	return 0;
}