/*
 * We can check the dominant colors that are  present in a given image. To do so we can use the histogram plot of the Hue channel.
 *
 * What is a Histogram?
 * It is a graphical representation of the distribution of data. An image histogram gives a graphical representation of the tonal
 * distribution in a digital image.
 *
 * The x-axis indicates the range of values the variable can take. This range can be divided into a series of intervals called
 * bins.
 * The y-axis shows the count of how many values fall within what interval of bin.
 */

#include <matplot/matplot.h> // A great graphics library for data visualization

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

/**
 * \brief Function to show histogram for Hue channel in HSV color space
 * \tparam N Number of channels in split image.
 * \param mat cv::Mat contains image in HSV color space
 * \param numBins Optional argument to set number of bins in histogram
 */
template <size_t N>
void displayHistForHue(const cv::Mat (&mat)[N], int numBins=0)
{
    // Convert Hue channel to std::vector
	std::vector<uchar>vec(mat[0].begin<uchar>(), mat[0].end<uchar>());

    // Build histogram using matplot++ hist method
    auto h = matplot::hist(vec);
    if(numBins > 0)
		h->num_bins(numBins);
    
    matplot::show();


}


int main()
{
    // Read the image from path.
    std::string image_path = cv::samples::findFile("../data/images/jersey.jpg");

    // Load image from disk
	cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

    // Check to see if image is loaded correctly
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return -1;
    }

    // Show original image
    cv::imshow("Image", img);
    cv::waitKey(0);
    cv::destroyWindow("Image");

    // Convert to HSV color space
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    // Split HSV image into three channels
    cv::Mat hsvChannels[3];
    cv::split(hsv, hsvChannels);

    // Build histogram using Matplot++ library
    // From the histogram plot, we can see that the peaks are at 0, 25, 100, and 120. These corresponds to Red, Yellow, and Blue
    // colors respectively. The highest peak is for red, though blue is the most dominant color in the image.
    // The reason for this anomaly is because of the white portions in the image. White (#FFFFFF) and Black (#000000) do not have
    // specific hue value. White and Black only depends on the Saturation and Value fields. Hence, White and Black take 0 as the
    // default hue value, and hence, it is seen in the histogram as red is dominant.
    displayHistForHue(hsvChannels);

    


    return 0;
}