/*
 * High Dynamic Range (HDR) Imaging
 * Most digital cameras and displays capture or display color images as 24-bits matrices. There are 8-bits per color channel and
 * the pixel values are therefore in the range 0-255 for each channel. In other words, a regular camera or a display has a limited
 * dynamic range.
 * However, the world around us has a very large dynamic range. It can get pitch black inside a garage when the lights are turned
 * off and it can get really bright if we are looking directly at the Sun. Even without considering those extremes, in everyday
 * situations, 8-bits are barely enough to capture scene. So, the camera tries to estimate the lightning and automatically sets
 * the exposure so that the most interesting aspect of the image has good dynamic range, and the parts that are too dark and too
 * bright are clipped off to 0 and 255 respectively.
 * For example an iPhone captures an HDR image. It takes  three images at three different exposures. The images are taken in quick
 * succession so there is almost no movement between the three shots. The three images are then combined to produce the HDR image.
 */

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



void showPlot(const std::vector<cv::Mat>& mat)
{
	std::vector<std::vector<double>> vec{mat.begin(), mat.end()};

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

void show(const std::string& name, const cv::Mat& mat, int width=800, int height=600)
{
	cv::namedWindow(name, cv::WINDOW_NORMAL);
	cv::resizeWindow(name, width, height);
	cv::imshow(name, mat);
	cv::waitKey(0);
	cv::destroyWindow(name);
}

// How to create HDR images?

/*
 * Step 1.: Capture multiple images with different exposures
 * When we take a picture using camera, we have only 8-bits per channel to represent the dynamic range (brightness range) of the scene.
 * But we can take multiple images of the scene at different exposures by changing the shutter speed. Most SLR cameras have a feature
 * called Auto Exposure Bracketing (AEB) that allows us to take multiple pictures at different exposures with just one press of a button.
 * Using AEB on a camera or an auto bracketing app on the phone, we can take multiple pictures quickly one after the other so the scene
 * does not change. For example an iPhone takes three pictures:
 *	1. An underexposed image: this image is darker than the properly exposed image. The goal is the capture parts of the image that are
 *	   very bright.
 *	2. A properly exposed image: this is the regular image the camera would have taken based on the illumination it has estimated.
 *	3. An overexposed image: this image is brighter than the properly exposed image. The goal is the capture parts of the image that
 *	   very dark and the camera needs more time to capture enough light to see what is there in the dark.
 *
 * However, if the dynamic range of the scene is very large, we can take more than three pictures to compose HDR image.
 */


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

	for(const auto& file : filenames)
	{
		cv::Mat im{ cv::imread(DATA_PATH + file) };
		images.push_back(im);
	}
}

int main()
{

	std::vector<cv::Mat> images;
	std::vector<float> times;
	readImagesAndTimes(images, times);

	/*
	 * Step 2: Align images
	 * Misalignment of images used in composing the HDR image can result in serve artifacts. Fortunately, OpenCV provides a way to align these
	 * images using AlignMTB() function. The algorithm converts all the images to median threshold bitmaps (MTB). An MTB for an image is calculated
	 * by assigning the value 1 to pixels brighter than median luminance and 0 otherwise. An MTB is invariant to the exposure time. Therefore, the
	 * MTBs can be aligned without requiring us to specify the exposure time.
	 * Syntax:
	 *	void cv::AlignMTB::process(InputArrayOfArrays src, std::vector<cv::Mat>& dst, InputArray times, InputArray response)
	 * Parameters:
	 *	- src - vector of input images.
	 *	- dst - vector of alignment images.
	 *	- times - vector of exposure time values for each image.
	 *	- response - 256x1 matrix with inverse camera response function for each pixel value, it should have the same number of channels as images.
	 */
	cv::Ptr<cv::AlignMTB> alignMTB = cv::createAlignMTB();
	alignMTB->process(images, images);

	/*
	 * Step 3: Recover the Camera Response Function
	 * The response of a typical camera is not linear to scene brightness. Suppose, two objects are photographed by a camera and one of them is twice
	 * an bright as the other in the real world. When we measure the pixel intensities of the two objects in the photograph, the pixel values of the
	 * brighter object will not be twice that of the darker object! Without estimating the Camera Response Function (CRF), we will not be able to merge
	 * the images into one HDR image.
	 * What does it mean to merge multiple exposure images into an HDR image?
	 * Consider one pixel at some location (x, y) of the image. If the CRF was linear, the pixel value would be directly proportional to the exposure
	 * time unless the pixel is too dark (nearly 0) or too bright (nearly 255) is a particular image. We can filter out these bad pixels (to dark or
	 * too bright), and estimate the brightness at a pixel by dividing the pixel value by the exposure time and then averaging this brightness value
	 * across all images where the pixel is not bad (too dark or too bright). We can do this for all pixels and obtain a single image where all
	 * pixels are obtained by averaging "good" pixels.
	 * But the CRF is not linear and we need to make the image intensities linear before we can merge/average them by first estimating the CRF.
	 * CRF can be estimated from the images if we know the exposure times for each image. Like mane problems in computer vision, the problem of finding
	 * the CRF is set up as an optimization problem, where the goal is minimize an objective function consisting of a data term and a smoothness term.
	 * These problems usually reduce to a linear least squares problem which are solved using Singular Value Decomposition (SVD)
	 * Finding the CRF is done using one of two OpenCV functions: CalibrateDebevec() or CalibrateRobertson().
	 * Syntax:
	 *	cv::Ptr<cv::calibrateDebevec> cv::createCalibrateDebevec(int samples=70, float lambda=10.0f, bool random=false)
	 *	void cv::CalibrateCRF::process(InputArrayOfArrays src, OutputArray dst, InputArray times)
	 * Parameters:
	 *	- src - vector of input images.
	 *	- times - vector of exposure time values for each image.
	 *	- dst - optional 256x1 matrix with inverse camera response function.
	 */
	cv::Mat responseDebevec;
	cv::Ptr<cv::CalibrateDebevec> calibrateDebevec = cv::createCalibrateDebevec();
	calibrateDebevec->process(images, responseDebevec, times);

	std::vector<cv::Mat> responseChannels(3);
	cv::split(responseDebevec, responseChannels);

	// Show figure to presents the CRF recovered using the images for red, green and blue channels. 
	showPlot(responseChannels);

	/*
	 * Step 4: Merge Images
	 * Once the CRF has been estimated, we can merge the exposure images into one HDR image using MergeDebevec
	 */

	cv::Mat hdrDebevec;
	cv::Ptr<cv::MergeDebevec> mergeDebevec = cv::createMergeDebevec();
	mergeDebevec->process(images, hdrDebevec, times, responseDebevec);

	/*
	 * Step 5: Tone mapping
	 * The process of converting a High Dynamic Range (HDR) image to an 8-bit channel image while preserving asa much detail as possible is called Tone mapping.
	 * The minimum value is 0 for a pitch black condition, the maximum value is theoretical infinite.
	 * Even though we have recovered the relative brightness information using multiple images, we now have the challenge of saving this information as a 24-bit
	 * image for display purposes.
	 * There are several tone mapping algorithm. OpenCV implements few of them. The thing to keep in mind is that there is no right way to do tone mapping.
	 * Usually, we want to see more detail in the tonemapped image than in any one of the exposure images. Sometimes the goal of tone mapping is to produce
	 * realistic images and often times the goal is to produce surreal images.
	 * We have some options:
	 *  - gamma - this parameter compress the dynamic range by applying a gamma correction. When gamma is equal to 1, bo correction applied. A gamma less than
	 *			  1 darkness the image, while gamma greater than 1 brightness the image.
	 *	- saturation - this parameter is used to increase or decrease the amount of saturation. When saturation is high, the colors are richer and more intense
	 *				   Saturation value closer to zero, makes colors fade away to gray-scale.
	 *	- contrast - controls the contrast (i.e. log(maxPixelValue/minPixelValue)) of the output image
	 */

	/*
	 * Step 5a: Drago Tonemap:
	 * Ptr<TonemapDrago> cv::createTonemapDrago(float gamma = 1.0f, float saturation = 1.0f, float bias = 0.85f)
	 * Bias is the value for bias function in [0, 1] range. Values from 0.7 to 0.9 usually give the best results. The default value is 0.85.
	 */
	cv::Mat ldrDrago;
	cv::Ptr<cv::TonemapDrago> tonemapDrogo = cv::createTonemapDrago(1.0, 0.7);
	tonemapDrogo->process(hdrDebevec, ldrDrago);
	ldrDrago *= 3;

	show("HDR using Drago Tone Mapping", ldrDrago);


	/*
	 * Step 5b: Reinhard Tonemap:
	 * Ptr<TonemapReinhard> cv::createTonemapReinhard(float gamma = 1.0f, float intensity = 0.0f, float light_adapt = 1.0f, float color_adapt = 0.0f)
	 * Parameters:
	 *	- intensity - should be in the [-8, 8] range. Greater intensity value produces brighter results.
	 *	- light_adapt - controls the light adaptation and is in the [0, 1] range. A value 1 indicates adaptation based only on pixel value and a value of 0
	 *					indicates global adaptation. An in-between value can be used for a weighted combination of the two.
	 *	- color_adapt - control chromatic adaptation and is in the [0, 1] range. the channels are treated independently if the value is set to 1 and the
	 *					adaptation level is the same for every channel if the value set to 0. An in-between value can be used for weighted combination of the two.
	 */

	cv::Mat ldrReinhard;
	cv::Ptr<cv::TonemapReinhard> tonemapReinhard = cv::createTonemapReinhard(1.5, 0, 0, 0);
	tonemapReinhard->process(hdrDebevec, ldrReinhard);

	show("HDR using Reinhard Tone Mapping", ldrReinhard);


	/*
	 * Step 5c: Mantiuk Tonemap:
	 * Ptr<TonemapMantiuk> cv::createTonemapMantiuk(float gamma = 1.0f, float scale = 0.7f, float saturation = 1.0f)
	 * The parameters scale is the contrast scale factor. Values from 0.6 to 0.9 produce best results.
	 */
	cv::Mat ldrMantiuk;
	cv::Ptr<cv::TonemapMantiuk> tonemapMantiuk = cv::createTonemapMantiuk(2.2, 0.85, 1.2);
	tonemapMantiuk->process(hdrDebevec, ldrMantiuk);
	ldrMantiuk *= 3;

	show("HDR using Mantiuk Tone Mapping", ldrMantiuk);

	// Show all
	std::vector<cv::Mat> ldrImages{ ldrDrago, ldrReinhard, ldrMantiuk };
	cv::Mat ldr;
	cv::hconcat(ldrImages, ldr);

	show("All ldr images", ldr, 3 * 800);


	return 0;
}
