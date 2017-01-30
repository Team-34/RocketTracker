#define WPI_DEPRECATED(msg)
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2\highgui\include\opencv2\highgui.hpp"
#include "opencv2\imgproc\include\opencv2\imgproc.hpp"
#include <string>
#include <time.h>
#include <math.h>
#include <memory>
#include "ntcore.h"
#include "networktables\NETWORKTABLE.H"
using namespace cv;
using namespace std;

using std::shared_ptr;

#define PI 3.14159265

shared_ptr<NetworkTable> startConnection() {
	NetworkTable::SetClientMode();
	NetworkTable::SetTeam(34);
	shared_ptr<NetworkTable> myTable = NetworkTable::GetTable("SmartDashboard");
	return myTable;
}

double calculateDistance(double _fov, double _width)
{
	_fov /= 2;
	double _rfov = (_fov * PI / 180.0);
	double tanned = tan(_rfov);
	double distance = tanned * _width;
	return distance;
}
int main(int argc, char** argv)
{
	//setup all my variables
	shared_ptr<NetworkTable> table = startConnection();
	double neededAngle;
	double offset;
	int width;
	double angle;
	int height;
	double diameter;
	int cameraInput;
	cout << "Resolution Width: ";
	cin >> width;
	cout << "Resolution Height: ";
	cin >> height;
	cout << "Camera FOV: ";
	cin >> angle;
	cout << "Cam Input";
	cin >> cameraInput;
	//Half is always a decimal becaues the pixels start at 0; This functions determines the midway point of the camera view
	double half = width - 1;
	half = half / 2;
	float minTargetRadius;
	cout << "Minimum Target Radius: ";
	cin >> minTargetRadius;
	int i = 0;

	angle = angle / width;
	vector<double> vec;
	VideoCapture cap(cameraInput); //capture the video from webcam
	vector<cv::Point2i> center;
	vector<int> radius;

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	cap.set(CAP_PROP_EXPOSURE, -8);
	cap.set(CAP_PROP_BRIGHTNESS, -64);

	namedWindow("Control", CV_WINDOW_NORMAL); //create a window called "Control"

	int iLowH = 44;
	int iHighH = 99;

	int iLowS = 114;
	int iHighS = 255;

	int iLowV = 93;
	int iHighV = 255;

	int brightness = -64;

	int exposure = 4;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);

	createTrackbar("Brightness", "Control", &brightness, 128);
	createTrackbar("Exposure", "Control", &exposure, 8);

	int iLastX = -1;
	int iLastY = -1;

	

	Mat imgTmp;

	namedWindow("Stream", CV_WINDOW_NORMAL);
	namedWindow("Thresholded Stream", CV_WINDOW_AUTOSIZE);

	//Capture a temporary image from the camera
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);

	while (true)
	{
		Mat imgOriginal;

		cap.read(imgOriginal); // read a new frame from video

		imshow("Stream", imgOriginal);

		Mat imgHSV;
		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		int thresh = 100;
		Mat canny_output;
		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours;
		RNG rng(12345);

		Canny(imgThresholded, canny_output, thresh, thresh * 2, 3);
		imshow("Thresholded Stream", imgThresholded);
		/// Find contours
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
		float largest = 0.0f;
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
						if (r > largest) {
							largest = r;
						}
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
			diameter = radius[nsize] * 2;
			neededAngle = offset * angle;
			//table->PutNumber("neededAngle", 45);
			double distance = calculateDistance(angle, diameter);
			//table->PutNumber("distance", 10);
			cout << neededAngle << "\t" << calculateDistance(angle, diameter) << endl;
		}

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
}
