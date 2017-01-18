#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <time.h>
#include "opencv2/gpu/gpu.hpp"

using namespace cv;
using namespace std;
using namespace gpu;




int main(int argc, char** argv)
{

	//setup all my variables
	double neededAngle;
	double offset;
	int width;
	double angle;
	int height;
	cout << "Resolution Width: ";
	cin >> width;
	cout << "Resolution Height: ";
	cin >> height;
	cout << "Camera FOV: ";
	cin >> angle;
	//Half is always a decimal becaues the pixels start at 0; This functions determines the midway point of the camera view
	double half = width - 1;
	half = half / 2;
	float minTargetRadius;
	cout << "Minimum Target Radius: ";
	cin >> minTargetRadius;
	int i = 0;

	angle = angle / width;
	vector<double> vec;
	VideoCapture cap(0); //capture the video from webcam
	vector<cv::Point2i> center;
	vector<int> radius;

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 44;
	int iHighH = 99;

	int iLowS = 114;
	int iHighS = 255;

	int iLowV = 93;
	int iHighV = 255;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);

	int iLastX = -1;
	int iLastY = -1;

	Mat imgTmp;
	GpuMat g_imgTmp;


	namedWindow("Stream", CV_WINDOW_NORMAL);

	//Capture a temporary image from the camera
	cap.read(imgTmp);

    g_imgTmp.upload(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);


	while (true)
	{
		Mat imgOriginal;

		cap.read(imgOriginal); // read a new frame from video

        imshow("Stream", imgOriginal);

        GpuMat g_imgOriginal;
        g_imgOriginal.upload(imgOriginal);
		GpuMat g_imgHSV;
        Mat imgHSV;
		cvtColor(g_imgOriginal, g_imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		g_imgHSV.download(imgHSV);
		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
		GpuMat g_imgThresholded(imgThresholded);
		//morphological opening (removes small objects from the foreground)
		gpu::erode(g_imgThresholded, g_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		gpu::dilate(g_imgThresholded, g_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		gpu::dilate(g_imgThresholded, g_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		gpu::erode(g_imgThresholded, g_imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		int thresh = 100;
		GpuMat g_canny_output;
		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours;
		RNG rng(12345);

		gpu::Canny(g_imgThresholded, g_canny_output, thresh, thresh * 2, 3);
		/// Find contours
		Mat canny_output;
		g_canny_output.download(canny_output);
		findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		/// Draw contours
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		for (int i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}

		/*This function finds if there are any contours, and if there are
		Then we will first find the center, then add that to a vector(array); This is because there can be more
		than one contours */
		if (contours.size() > 0) {
			for (int i = 0; i < contours.size(); i++) {
				if (cv::contourArea(contours[i]) > 1) {
					cv::Point2f c;
					float r;
					cv::minEnclosingCircle(contours[i], c, r);

					if (r >= minTargetRadius)
					{
						center.push_back(c);
						radius.push_back(r);
					}
				}
			}
			size_t count = center.size();
		}

		imshow("contours", drawing);

		int ssize = center.size();
		int nsize = center.size() - 1;
		//This is an equation I got from Cheesy Poofs to find out what angle we need to be at
		if (ssize > 0) {
			offset = center[nsize].x - half;
			neededAngle = offset * angle;
			cout << neededAngle << endl;
		}



		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
}
