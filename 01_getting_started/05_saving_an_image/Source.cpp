/*
 * Saving an image to the disk:
 * Syntax:
 *	cv::imwrite(filename, img, params);
 *	where:
 *	- filename is a string that provides the relative or absolute path where the image should be saved
 *	- img is a cv::Mat object
 *	- params contain additional information about quality etc. (documentation: https://docs.opencv.org/4.1.0/d4/da8/group__imgcodecs.html#ga292d81be8d76901bff7988d18d2b42ac)
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

int main()
{
	// path to the image
	std::string image_path{ "../data/images/boy.jpg" };

	// Read an image from disk
	cv::Mat img{ cv::imread(image_path, cv::IMREAD_COLOR) };

	// Save image to disk
	cv::imwrite("saved_image.png", img);

	return 0;
}