/*
 * Different versions of cloning
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	// Read source image
	cv::Mat src{ cv::imread("../data/images/iloveyouticket.jpg") };

	// Read destination image
	cv::Mat dst{ cv::imread("../data/images/wood-texture.jpg") };

	// Create all white mask
	cv::Mat src_mask{ cv::Mat::ones(src.rows, src.cols, src.depth()) * 255 };

	// The location of the center of the src in the dst
	cv::Point center{ dst.cols / 2, dst.rows / 2 };

	// Seamless clone scr into dst and put the results in output
	cv::Mat normal_clone;
	cv::Mat mixed_clone;
	cv::Mat monochrome_transfer;

	cv::seamlessClone(src, dst, src_mask, center, normal_clone, cv::NORMAL_CLONE);
	cv::seamlessClone(src, dst, src_mask, center, mixed_clone, cv::MIXED_CLONE);
	cv::seamlessClone(src, dst, src_mask, center, monochrome_transfer, cv::MONOCHROME_TRANSFER);


	/*
	 * If we use Normal Cloning by using cv::NORMAL_CLONE flag, we did not use a good mask and we can see excessive smoothing between the words
	 * "I" and "Love", and between "you" and "Paa". We could have created a rough mask and improved the result. But instead we could use Mixed
	 * Cloning.
	 * In Normal Cloning the texture (gradient) of the source image is preserved in the cloned region.
	 * In Mixed Cloning, the texture (gradient) of the cloned region is determined be a combination of the source and the destination images.
	 * Mixed cloning does not produce smooth regions because it picks the dominant texture (gradient) between the source and destination images.
	 * Notice the texture for mixed cloning is no longer smooth between "I" and "Love", and "you" and"Paa".
	 */
	cv::imshow("Normal Clone Result", normal_clone);
	cv::imshow("Mixed Clone Result", mixed_clone);
	cv::imshow("Monochrome Transfer Result", monochrome_transfer);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}