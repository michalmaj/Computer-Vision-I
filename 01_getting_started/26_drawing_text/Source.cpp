/*
 * Add Text
 * For drawing text in OpenCV library we can use cv::putText function.
 * Syntax:
 *	void cv::putText(InputOutputArray img, const String& text, Point org, int fontFace, double fontScale, Scalar color,
 *					 int thickness=1, lineType=LINE_8, bool bottomLeftOrigin=false)
 * Arguments:
 *	- img - image on which the text has to be written
 *	- text - text string to be written
 *	- org - bottom-let corner of the text string in the image
 *	- fontFace - font type (https://docs.opencv.org/4.x/d6/d6e/group__imgproc__draw.html#ga0f9314ea6e35f99bb23f29567fc16e11)
 *	- fontScale - font scale factor that is multiplied be the font-specific base size.
 *	- color - font color
 *	- the optional arguments that we are going to use are same as before
 *
 * We can automated fontScale argument from above function. There is an OpenCV function cv::getFontScaleFromHeight which
 * calculates the fontScale given the height and font.
 * Syntax:
 *	double cv::getFontScaleFromHeight(const int fontFace, const int pixelHeight, const in t thickness=1)
 * Arguments:
 *	- fontFace - font to use
 *	- pixelHeight - pixel height to compute the fontScale for
 *	- thickness - thickness of lines used to render the text
 *	- return value (output) - the font size to use in cv::putText() function
 *
 * To help us even more OpenCV library provides the function which determines the height and width of the text box. In the
 * first and second example we will printed the text at the bottom left corner. What if we want to center the text? Without
 * knowing how much area the text is going to take, it is not possible to do that. To do that we will use cv::getTextSize()
 * function:
 * Syntax:
 *	Size cv::getTextSize(const String& text, int fontFace, double fontScale, int thickness, int* baseLine)
 * Arguments:
 *	- text - input text string.
 *	- fontFace - font to use.
 *	- fontScale - font scale factor that is multiplied be the font-specific base-size.
 *	- thickness - thickness of lines used to render the text.
 *	- baseLine - y-coordinate of the baseline relative to the bottom-most text point.
 *	- textSize - the text size (width, height).
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

int main()
{
	// Load the image from disk
	cv::Mat image{ cv::imread("../data/images/boy.jpg", cv::IMREAD_COLOR) };

	// Make a copy of original image
	cv::Mat imageText = image.clone();

	// Put text on the image
	std::string textStr{ "I am studying" };
	double fontScale{ 1.5 };
	int fontFace{ cv::FONT_HERSHEY_COMPLEX };
	cv::Scalar fontColor{ cv::Scalar(250, 10, 10) };
	int fontThickness{ 2 };

	cv::putText(imageText, textStr, cv::Point(20, 350), fontFace, fontScale, fontColor, fontThickness, cv::LINE_AA);

	// Show the image with text
	cv::imshow("Image with Text", imageText);
	cv::waitKey(0);
	cv::destroyWindow("Image with Text");

	// Issues in Text annotation
	/*
	 * There are a few issues with annotating with text:
	 *	- First, the text looks pretty big. This happens very often because it is difficult to say before-hand what size text we
	 *	  should use. If the image is very large, fontScale=1.5 can look too small and vice-versa.
	 *	- Second, we can see the bottom, part of the text is slightly clipped. This happens because we don't know how big the text
	 *	  will look on the image.
	 *
	 * The solution is to get font size from pixel height of text (cv::getFontScaleFromHeight() function)
	 */

	// Make an another copy of the original image
	cv::Mat imageTextFontScale = image.clone();

	// Instead of using a fontScale, let us  use a te3xt height of 20 pixels to determine the fontScale and write the same text again.
	int pixelHeight{ 20 };

	// Calculate the fontScale
	fontScale = cv::getFontScaleFromHeight(fontFace, pixelHeight, fontThickness);

	std::cout << "fontScale = " << fontScale << std::endl;

	cv::putText(imageTextFontScale, textStr, cv::Point(20, 350), fontFace, fontScale, fontColor, fontThickness, cv::LINE_AA);

	// Show the image with text
	cv::imshow("Image with Scale Font Text", imageTextFontScale);
	cv::waitKey(0);
	cv::destroyWindow("Image with Scale Font Text");

	// Make third copy of the original image
	cv::Mat imageGetTextSize = image.clone();

	// get width and height of the image
	auto [imageWidth, imageHeight] = imageGetTextSize.size();

	// Get the text box height and width and also the baseline
	int baseline{ 0 };
	cv::Size textSize = cv::getTextSize(textStr, fontFace, fontScale, fontThickness, &baseline);

	int textWidth = textSize.width;
	int textHeight = textSize.height;

	std::cout << "Text Width: " << textWidth << ", text height: " << textHeight << ", baseline: " << baseline << std::endl;

	// Get the coordinates of text box bottom left corner
	// The x-coordinate will be such that the text is centered
	int xcoordinate{ (imageWidth - textWidth) / 2 };

	// The y-coordinate will be such that the entire box is just 10 pixels above the bottom of the image
	int ycoordinate{ imageHeight - baseline - 10 };

	std::cout << "Text Bottom Left (" << xcoordinate << "," << ycoordinate << ")" << std::endl;

	// Draw a canvas using a filled rectangle
	cv::Scalar canvasColor{ cv::Scalar(255, 255, 255) };
	cv::Point canvasBottomLeft(xcoordinate, ycoordinate + baseline);
	cv::Point canvasTopRight(xcoordinate + textWidth, ycoordinate - textHeight);

	std::cout << "Canvas Bottom Left: " << canvasBottomLeft << ", Canvas Top Right: " << canvasTopRight << std::endl;

	// Draw white rectangle
	cv::rectangle(imageGetTextSize, canvasBottomLeft, canvasTopRight, canvasColor, -1);

	// Draw the baseline
	int lineThickness{ 2 };
	cv::Point lineLeft{ xcoordinate, ycoordinate };
	cv::Point lineRight{ xcoordinate + textWidth, ycoordinate };
	cv::Scalar lineColor{ cv::Scalar(0, 255, 0) };

	cv::line(imageGetTextSize, lineLeft, lineRight, lineColor, lineThickness, cv::LINE_AA);

	// Draw the text
	cv::putText(imageGetTextSize, textStr, cv::Point(xcoordinate, ycoordinate), fontFace, fontScale, fontColor, fontThickness, cv::LINE_AA);

	// Show the image with text
	cv::imshow("Image with Get Text Size", imageGetTextSize);
	cv::waitKey(0);
	cv::destroyWindow("Image with Get Text Size");


	return 0;
}