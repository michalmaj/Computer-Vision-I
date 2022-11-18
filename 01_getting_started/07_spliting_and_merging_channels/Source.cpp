#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

std::vector<cv::Mat> mergeBGR(const std::vector<cv::Mat>& img)
{
	std::vector<cv::Mat> output;
	output.push_back(img[0]);
	output.push_back(img[1]);
	output.push_back(img[2]);

	return output;
}

int main()
{
	// path to the image
	std::string image_path{ "../data/images/musk.jpg" };

	// Read the image
	cv::Mat img{ cv::imread(image_path, cv::IMREAD_UNCHANGED) };

	// Spit image into channels
	std::vector<cv::Mat> img_channels(3); // std::vector<cv::Mat> as a container for BGR channels
	cv::split(img, img_channels);

	cv::imshow("Blue Channel", img_channels[0]);
	cv::imshow("Green Channel", img_channels[1]);
	cv::imshow("Red Channel", img_channels[2]);
	cv::waitKey(0);
	cv::destroyAllWindows();

	// Merge channels
	cv::Mat output; // cv::Mat for merged image
	cv::merge(mergeBGR(img_channels), output);
	cv::imshow("Merged", output);
	cv::waitKey(0);
	cv::destroyAllWindows();
}