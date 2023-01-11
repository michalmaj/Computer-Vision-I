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
	cv::Mat imgClone;
	cv::Mat gray;
	const std::string WINDOW_NAME{ "Blemish Remover" };

	const int RADIUS{ 10 };

	std::vector<cv::Point> neighbors
	{
		cv::Point{-RADIUS, -RADIUS },
		cv::Point{-RADIUS, 0 },
		cv::Point{-RADIUS, RADIUS },
		cv::Point{0, -RADIUS },
		cv::Point{0, -RADIUS },
		cv::Point{RADIUS, -RADIUS },
		cv::Point{RADIUS, 0 },
		cv::Point{RADIUS, RADIUS },
	};
};

double getMean(const cv::Mat& gradImg)
{
	cv::Scalar m{ cv::mean(gradImg) };
	return m.val[0];
}

cv::Mat getPatch(const cv::Point& center, void* userdata)
{
	auto brp{ static_cast<BlemishRemovalParameters*>(userdata) };
	cv::Rect roi;
	roi.x = center.x - brp->RADIUS - brp->RADIUS / 2;
	roi.y = center.y - brp->RADIUS - brp->RADIUS / 2;
	roi.height = brp->RADIUS * 2;
	roi.width = brp->RADIUS * 2;
	cv::Mat cropImg{ brp->gray(roi).clone() };
	return cropImg;
}

double getSobelScore(const cv::Mat& curPatch)
{
	double score{ 0.0 };
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y;

	cv::Sobel(curPatch, grad_x, CV_32F, 2, 0, 3, 1, 0);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	cv::Sobel(curPatch, grad_y, CV_32F, 0, 2, 3, 1, 0);
	cv::convertScaleAbs(grad_y, abs_grad_y);

	cv::Mat grad;
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	score = getMean(grad);
	return score;
}

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

		cv::Mat tmpPatch = getPatch(curP, brp);
		double tmp_score = getSobelScore(tmpPatch);

		if(tmp_score < min_score)
		{
			min_score = tmp_score;
			bestCor = curP;
		}
	}
	return bestCor;
}

cv::Mat getPatchColor(const cv::Point& best_patch_color, void* userdata)
{
	auto brp{ static_cast<BlemishRemovalParameters*>(userdata) };
	cv::Rect roi;
	roi.x = best_patch_color.x - brp->RADIUS - brp->RADIUS / 2;
	roi.y = best_patch_color.y - brp->RADIUS - brp->RADIUS / 2;
	roi.height = brp->RADIUS * 2;
	roi.width = brp->RADIUS * 2;
	cv::Mat cropImg{ brp->image(roi).clone() };
	return cropImg;
}

void mouse_click_callback(int action, int x, int y, int flags, void* userdata)
{
	auto brp{ static_cast<BlemishRemovalParameters*>(userdata) };

	if(action == cv::EVENT_LBUTTONDOWN)
	{
		cv::cvtColor(brp->image, brp->gray, cv::COLOR_BGR2GRAY);
		// Blemish point
		cv::Point blemishCenter = cv::Point(x, y);
		cv::Point bestPatchCoordinates{ getBestPatch(x, y, brp) };
		
		cv::Mat bestPatch{ getPatchColor(bestPatchCoordinates, brp) };

		cv::Mat mask = 255 * cv::Mat::ones(bestPatch.rows, bestPatch.cols, bestPatch.depth());

		cv::seamlessClone(bestPatch, brp->image, mask, blemishCenter, brp->image, cv::NORMAL_CLONE);
		cv::imshow(brp->WINDOW_NAME, brp->image);
	}
}

int main()
{
	BlemishRemovalParameters brp;

	brp.image = cv::imread("../data/images/blemish.png");
	brp.imgClone = brp.image.clone();

	// Check to see if image is correctly loaded
	if(brp.image.empty())
	{
		std::cout << "Can't load image" << std::endl;
		return -1;
	}

	// Creating a named window
	cv::namedWindow(brp.WINDOW_NAME);
	cv::imshow(brp.WINDOW_NAME, brp.image);
	
	cv::setMouseCallback(brp.WINDOW_NAME, mouse_click_callback, &brp);
	cv::waitKey(0);


	cv::destroyWindow(brp.WINDOW_NAME);

	return 0;
}