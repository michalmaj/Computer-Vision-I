/*
 * Bilateral Filtering:
 * A Bilateral Filter is non-linear, edge-preserving and noise-reducing smoothing filter. Most smoothing filters (like Gaussian or a Box filter)
 * have a parameter called σs (the s is the subscript stands for spatial) that determines the amount of smoothing. Often this value is closely
 * related to the kernel size. A typical smoothing filter replaces the intensity value of a pixel by the weighted sum of its neighbors. The bigger
 * the neighborhood, the smoother the filtered image looks. The size of the neighborhood is directly proportional to the parameter σs.
 * In edge-preserving filters, there are two competing objectives:
 *	1. Smooth the image.
 *	2. Don't smooth the edges/color boundaries.
 * In other words, if we want to preserve edges,we cannot simply replace the color of a pixel by the weighted sum of its neighbors.
 * For example we have a 3x3 image patch like this:
 */
//tex:
//$$\begin{bmatrix}30 & 200 & 239\\31 & 220 & 235\\35 & 210 & 233\end{bmatrix}$$

/*
 * A we can see the values in the left column are much lower than the values in the center and the right columns. In other words, this patch is
 * part of a vertical edge.  In a typical filtering operation, we replace the center pixel by some weighted combination of all 9 pixels in this
 * 3x3 neighborhood. However, in this case, a better idea is to filter the center pixel based on only the center and right-hand side columns so
 * that the edge is retained and not blurred-out.
 * In bilateral filtering while calculating the contribution of any pixel to the final output, we weight the pixels that are close in terms of
 * intensity to the center pixel higher as compared to the pixels whose intensities are very different from the center pixels. We want to find
 * a weight that depends on the square of the intensity difference (Ip-Ip)^2 between the center pixel q and its neighbor p.
 * It is also possible to redefine intensity as the Gaussian function:
 */
//tex:
//$${G_\sigma}_r(I_p-I_p)$$

/*
 * Additionally, just like Gaussian filtering, we also want to weight the pixels that are closer to the center pixel higher than the pixels that
 * are farther away. So, the weights should depend on ||p-q||.
 * We can also redefine it as a Gaussian:
 */
//tex:
//$${G_\sigma}_s(||p-q||)$$

// Combine the two, a bilateral filter will output the following at center pixel q:
//tex:
//$$O_q=\frac{1}{W_p}\sum_p{{G_\sigma}_s(||p-q||){G_\sigma}_r(I_p-I_q)}$$
//Where,
//$$W_p\:-\:The\:normalization\:constant$$
//$${G_\sigma}_s\:-\:Spatial\:Gaussian\:kernel$$
//$${G_\sigma}_r\:-\:Color/Range\:Gaussian\:kernel$$
//$$q\:-\:Center\:pixel$$
//$$p\:-\:Neighboring\:pixel$$
//$$I_p\:-\:Intensity\:at\:pixel\:p$$
//$$I_q\:-\:Intensity\:at\:pixel\:q$$

/*
 * If the neighborhood pixels are edges, the difference in intensity (Ip-Iq) wil be higher. Since the Gaussian is a decreasing function, Gσr(Ip−Iq)
 * wil have lower weights for higher values. Hence, the smoothing effect will be lower for such pixels, preserving the edges.
 * For bilateral filtering, we have two parameters: σs and σr. Just like other smoothing filters σs controls amount of spatial smoothing, and σr controls
 * how dissimilar colors within the neighboring will be averaged. A higher σr results in larger regions of constant color.
 *
 * Bilateral Filter in OpenCV:
 * OpenCV provides a bilateralFilter() function.
 * Syntax:
 *	void cv::bilateralFilter(InputArray src, OutputArray dst, int d, double sigmaColor,
 *							 double sigmaSpace, int borderType=BORDER_DEFAULT)
 * Parameters:
 *	- src - source 8-bit or floating-point, 1-channel or 3-channel image.
 *	- dst - destination image of the same size and type as src.
 *	- d - diameter of each pixel neighborhood that is used during filtering. If it is non-positive, it is computed from sigmaSpace.
 *	- sigmaColor - filter sigma in color space. A larger value of the parameter means that farther colors within the pixel neighborhood will be
 *				   mixed together, resulting in larger areas of semi-equal color.
 *	- sigmaSpace - filter sigma in the coordinate space. A larger value of the parameter means that farther pixels will influence each other
 *				   as long as their colors are close enough. When d>0, it specifies the neighborhood size regardless of sigmaSpace, otherwise
 *				   d is proportional to sigmaSpace.
 *	- borderType - border mode used to extrapolate pixels outside of the image.
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	// Load an image from disk
	cv::Mat image{ cv::imread("../data/images/gaussian-noise.png") };

	// Apply filter
	cv::Mat bilateralFiltered1, bilateralFiltered2;

	// Diameter of the pixel neighborhood used during filtering
	int dia{ 15 };

	// Larger the value the distant colors will be mixed together
	// to produce areas of semi equal colors
	double sigmaColor{ 80 };

	// Larger the value more the influence of the farther placed pixels
	// as long as their color are close enough
	double sigmaSpace{ 80 };

	cv::bilateralFilter(image, bilateralFiltered1, dia, sigmaColor, sigmaSpace);

	dia = -1;
	sigmaColor = 80;
	sigmaSpace = 10;
	cv::bilateralFilter(image, bilateralFiltered2, dia, sigmaColor, sigmaSpace);


	cv::imshow("Original Image", image);
	cv::imshow("Bilateral Blur Result1", bilateralFiltered1);
	cv::imshow("Bilateral Blur Result2", bilateralFiltered2);
	cv::waitKey(0);
	cv::destroyAllWindows();



	return 0;
}