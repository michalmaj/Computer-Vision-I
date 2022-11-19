/*
 * Images with an alpha channel, each pixel contains an additional information
 * about a numerical transparency [0, 255]. It defines what will happen when
 * the pixel is placed over another pixel.
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

std::pair<cv::Mat, cv::Mat> splitImage(const cv::Mat& imgPNG)
{
	cv::Mat imgBGR;
	cv::Mat imgPNGChannels[4];

	// split into 4 separate channels
	cv::split(imgPNG, imgPNGChannels);

	// merge only three channels (BGR)
	cv::merge(imgPNGChannels, 3, imgBGR);

	// Create mask from last channel (alpha)
	cv::Mat mask{ imgPNGChannels[3] };

	return std::make_pair(imgBGR, mask);
}

int main()
{
	// Path to the image
	std::string image_path{ "../data/images/panther.png" };

	// Read the image in unchanged form (with 4-channels in this case)
	cv::Mat img{ cv::imread(image_path, cv::IMREAD_UNCHANGED) };
	auto [width, height] = img.size();
	int channels = img.channels();
	std::cout << "Image width: " << width << ", height: " << height <<
		", number of channels: " << channels << std::endl;

	auto [imgBGR, mask] = splitImage(img);

	cv::imshow("Unchanged", img);
	cv::imshow("BGR", imgBGR);
	cv::imshow("Mask", mask);

	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}