#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Path to the image
	std::string image_path{ "../data/images/boy.jpg" };

	// Load the image from disk
	cv::Mat boy{ cv::imread(image_path, cv::IMREAD_UNCHANGED) };

	// Copy of the image
	cv::Mat copied_boy = boy.clone();

	// Get the roi (face in this case)
	cv::Mat roi{ boy(cv::Range(40, 200), cv::Range(180, 320)) };

	// Find Roi width and height
	auto [width, height] = roi.size();

	// Copy to left side of the face
	roi.copyTo(copied_boy(cv::Range(40, 40 + height), cv::Range(10, 10 + width)));
	roi.copyTo(copied_boy(cv::Range(40, 40 + height), cv::Range(330, 330 + width)));


	cv::imshow("Boy", boy);
	cv::imshow("Copied boy", copied_boy);
	cv::waitKey(0);
	cv::destroyAllWindows();


	return 0;
}

