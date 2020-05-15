#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "iostream"

using namespace std;

using namespace cv; 

int main(int argc, char** argv)
{
	Mat src, gray, dst, abs_dst;
	
	src = imread("bird.jpg");
	imshow("Original Image", src);
	imwrite(" Originalimage.jpg ", src);
	/// Remove noise by blurring with a Gaussian filter
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(src, gray, CV_RGB2GRAY);

	/// Apply Laplace function
	Laplacian(gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(dst, abs_dst);
	imshow("Laplace Output", abs_dst);
	imwrite("Laplaceoutput.jpg", abs_dst);
	

	// Create a kernel that we will use to sharpen our image
	Mat kernel = (Mat_<float>(3, 3) <<
		1, 1, 1,
		1, -9, 1,
		1, 1, 1);

	Mat img;
	filter2D(src, img, CV_32F, kernel);

	Mat sharp;
	src.convertTo(sharp, CV_32F);
	Mat imgResult = sharp - img;
	// convert back to 8bits gray scale
	imgResult.convertTo(imgResult, CV_8UC3);
	src.convertTo(img, CV_8UC3);
	// imshow( "Laplace Filtered Image", imgLaplacian );
	imshow("New Sharped Image", imgResult);
	imwrite(" NewSharpedImage.jpg", imgResult);
	// Create binary image from source image

	cvtColor(imgResult, gray, COLOR_BGR2GRAY);
	threshold(gray, gray, 40, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("Binary Image", gray);
	imwrite("Binaryimage.jpg", gray);
	// Perform the distance transform algorithm
	

	
	Mat dist;
	distanceTransform(gray, dist, DIST_L1, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);
	imshow("Distance Transform Image", dist);
	imwrite("LaplaceDistanceTransform.jpg", dist);
	waitKey(0);


	Mat grad;
	/*Mat gray1;
	cvtColor(src, gray1, CV_RGB2GRAY);*/
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	int c;
	Mat grad_xis, grad_yis;
	Mat abs_grad_xis, abs_grad_yis;

	/// Gradient X
	
	Sobel(gray, grad_xis, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_xis, abs_grad_xis);

	/// Gradient Y
	
	Sobel(gray, grad_yis, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_yis, abs_grad_yis);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_xis, 0.5, abs_grad_yis, 0.5, 0, grad);

	imshow("Sobel Output", grad);
	imwrite("SobelOutput.jpg",grad);

	grad = 255 - grad;

	Mat dist1;
	distanceTransform(grad, dist1, DIST_L1, 3);
	normalize(dist1, dist1, 0, 1.0, NORM_MINMAX);
	imshow("Distance Transform Image Sobel", dist1);
	imwrite("SobelDistanceTransform.jpg", dist1);
	waitKey(0);

	Mat edge;
	Mat gray2;
	cvtColor(src, gray2, CV_RGB2GRAY);
	Canny(gray2, edge, 50, 200, 3);

	edge.convertTo(gray2, CV_8U);

	edge = 255 - edge;
	
	imshow("Canny Output", gray2);
	imwrite("CannyOutput.jpg", gray2);
	Mat dist2;
	distanceTransform(edge, dist2, DIST_L1, 3);
	normalize(dist2, dist2, 0, 1.0, NORM_MINMAX);
	imshow("Distance Transform Image Canny", dist2);
	imwrite("CannyDistanceTransform.jpg", dist2);

	waitKey(0);

	return 0;
}