#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <matplot/matplot.h>

/**
 * \brief Plot CRF.
 * \param mat Input image.
 */
void showPlot(const std::vector<cv::Mat>& mat)
{
	std::vector<std::vector<double>> vec{ mat.begin(), mat.end() };

	auto h = matplot::figure(true);
	h->size(1200, 500);
	matplot::plot(vec[0], "b");
	matplot::hold(matplot::on);
	matplot::plot(vec[1], "g");
	matplot::hold(matplot::on);
	matplot::plot(vec[2], "r");
	matplot::title("CRF");
	matplot::xlabel("Measured Intensity");
	matplot::ylabel("Calibrated Intensity");
	matplot::show();
}

/**
 * \brief Function helper for resizing window purposes.
 * \param name Name of the window.
 * \param mat Input image.
 * \param width Width of the window.
 * \param height Height of the window.
 */
void show(const std::string& name, const cv::Mat& mat, int width = 800, int height = 600)
{
	cv::namedWindow(name, cv::WINDOW_NORMAL);
	cv::resizeWindow(name, width, height);
	cv::imshow(name, mat);
	cv::waitKey(0);
	cv::destroyWindow(name);
}

/**
 * \brief Load four images and create two std:vectors
 * \param images std::vector that contains cv::Mat (images)
 * \param times std::vector that contains the exposure times for the images
 */
void readImagesAndTimes(std::vector<cv::Mat>& images, std::vector<float>& times)
{
	const std::vector<float> timesArray{ 1 / 30.0f, 0.25f, 2.5f, 15.0f };
	times.assign(timesArray.begin(), timesArray.end());

	const std::vector<std::string> filenames{ "img_0.033.jpg", "img_0.25.jpg", "img_2.5.jpg", "img_15.jpg" };
	const std::string DATA_PATH{ "../data/images/" };

	for (const auto& file : filenames)
	{
		cv::Mat im{ cv::imread(DATA_PATH + file) };
		images.push_back(im);
	}
}

int main()
{
	// Step 1.: Capture multiple images with different exposures
	std::vector<cv::Mat> images;
	std::vector<float> times;
	readImagesAndTimes(images, times);

	// Step 2: Align images
	cv::Ptr<cv::AlignMTB> alignMTB{ cv::createAlignMTB() };
	alignMTB->process(images, images);

	// Step 3: Recover the Camera Response Function - CalibrateRobertson() in this case
	cv::Mat responseRobertson;
	cv::Ptr<cv::CalibrateRobertson> calibrateRobertson{ cv::createCalibrateRobertson() };
	calibrateRobertson->process(images, responseRobertson, times);

	std::vector<cv::Mat> responseChannels(3);
	cv::split(responseRobertson, responseChannels);

	// Show figure to presents the CRF recovered using the images for red, green and blue channels. 
	showPlot(responseChannels);

	// Step 4: Merge Images
	cv::Mat hdrRobertson;
	cv::Ptr<cv::MergeRobertson> mergeRobertson{ cv::createMergeRobertson() };
	mergeRobertson->process(images, hdrRobertson, times, responseRobertson);

	// Step 5: Tone mapping

	// Step 5a: Drago Tonemap:
	cv::Mat ldrDrago;
	cv::Ptr<cv::TonemapDrago> tonemapDrogo = cv::createTonemapDrago(1.0, 0.7);
	tonemapDrogo->process(hdrRobertson, ldrDrago);
	ldrDrago *= 3;

	show("HDR using Drago Tone Mapping", ldrDrago);

	// Step 5b: Reinhard Tonemap:
	cv::Mat ldrReinhard;
	cv::Ptr<cv::TonemapReinhard> tonemapReinhard = cv::createTonemapReinhard(1.5, 0, 0, 0);
	tonemapReinhard->process(hdrRobertson, ldrReinhard);

	show("HDR using Reinhard Tone Mapping", ldrReinhard);

	// Step 5c: Mantiuk Tonemap:
	cv::Mat ldrMantiuk;
	cv::Ptr<cv::TonemapMantiuk> tonemapMantiuk = cv::createTonemapMantiuk(2.2, 0.85, 1.2);
	tonemapMantiuk->process(hdrRobertson, ldrMantiuk);
	ldrMantiuk *= 3;

	show("HDR using Mantiuk Tone Mapping", ldrMantiuk);

	// Show all
	std::vector<cv::Mat> ldrImages{ ldrDrago, ldrReinhard, ldrMantiuk };
	cv::Mat ldr;
	cv::hconcat(ldrImages, ldr);

	show("All ldr images", ldr, 3 * 800);

	return 0;
}