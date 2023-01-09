#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/photo.hpp>


struct BlemishRemovalParameters
{
	cv::Mat image;
	cv::Mat gray;
	const std::string WINDOW_NAME{ "Blemish Remover" };

	const int RADIUS{ 15 };

	std::vector<cv::Point> neighbors
	{
		cv::Point{-RADIUS, -RADIUS },
		cv::Point{-RADIUS, 0 },
		cv::Point{-RADIUS, RADIUS },
		cv::Point{0, -RADIUS },
		cv::Point{0, RADIUS },
		cv::Point{RADIUS, -RADIUS },
		cv::Point{RADIUS, 0 },
		cv::Point{RADIUS, RADIUS },
	};
};

cv::Point getBestPatch(int x, int y, void* userdata)
{
	auto brp{ static_cast<BlemishRemovalParameters*>(userdata) };
	cv::Point bestCor{ cv::Point(x, y) };
	double min_score{ 1.0 * INT32_MAX };

	for(size_t i{0}; i< brp->neighbors.size(); ++i)
	{
		cv::Point curP = cv::Point(x, y) + brp->neighbors.at(i);
		if(curP.x - brp->RADIUS < 0 or curP.y - brp->RADIUS < 0)
			continue;
	}
}

int main()
{
	

	return 0;
}