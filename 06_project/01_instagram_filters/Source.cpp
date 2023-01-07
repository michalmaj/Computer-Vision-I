/*
 * Design two Instagram-like filters:
 *	1. Pencil Sketch Filter.
 *	2. Cartoon Filter.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat pencilSketch(const cv::Mat& image, int arguments = 0) {

    cv::Mat pencilSketchImage;

    /// YOUR CODE HERE

    // Convert to gray-scale and invert
	cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grayInverse{ 255 - gray };

    // Apply a Gaussian blur
    cv::Mat blur;
    cv::GaussianBlur(grayInverse, blur, cv::Size(21, 21), 0, 0);

    // Blend gray-scale image with blurred image
    cv::Mat blended;
    cv::divide(gray, 255 - blur, blended, 255);

    // Convert back to RGB
    cv::cvtColor(blended, pencilSketchImage, cv::COLOR_GRAY2BGR);

    return pencilSketchImage;
}

int main()
{
    // Read an image
    cv::Mat image{ cv::imread("../data/images/trump.jpg") };

    // Check to see if image is loaded correctly
    if(image.empty())
    {
        std::cout << "Can't load an image\n";
        return -1;
    }

    cv::Mat pencilSketchImage{ pencilSketch(image) };

    // Show images
    cv::imshow("Pencil Sketch", pencilSketchImage);
    cv::waitKey(0);
    cv::destroyAllWindows();

	return 0;
}