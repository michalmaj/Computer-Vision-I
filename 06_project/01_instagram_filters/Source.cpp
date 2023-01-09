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

cv::Mat cartoonify(const cv::Mat& image, int arguments = 0) {

    cv::Mat cartoonImage;

    /// YOUR CODE HERE

    // Convert to gray-scale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Apply median
    cv::Mat blurred;
    //cv::medianBlur(gray, blurred, 3);
    cv::GaussianBlur(gray, blurred, cv::Size(1, 1), 0, 0); // Gaussian blur giving better results

    // Get the edges from blurred image
    cv::Mat edges;
    cv::adaptiveThreshold(blurred, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 7, 7);

    // Apply bilateral filter
    cv::Mat bilateral;
    cv::bilateralFilter(image, bilateral, 7, 300, 300);

    // Add a cartooning effect
    cv::bitwise_and(bilateral, bilateral, cartoonImage, edges);

    return cartoonImage;
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
    cv::Mat cartoonImage{ cartoonify(image) };

    // Show images
    cv::imshow("Pencil Sketch", pencilSketchImage);
    cv::imshow("Cartoon Filter", cartoonImage);
    cv::waitKey(0);
    cv::destroyAllWindows();

	return 0;
}