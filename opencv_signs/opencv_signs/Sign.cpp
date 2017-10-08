#include "Common.h"

using namespace std;
using namespace cv;

Size imgSize(1000, 700);
int clipHist = 40;
Size kernelSize(3, 3);
int cannyThreashMin = 0;
int minSignSize = 5;


Scalar blueLow(110, 50, 50);
Scalar blueHigh(130, 255, 255);
Scalar redLow1(0, 70, 50);
Scalar redHigh1(10, 255, 255);
Scalar redLow2(170, 70, 50);
Scalar redHigh2(180, 255, 255);

int Sign(int argc, char* argv[]) {

	if (argc < 2) return -1;
	Mat srcRaw = imread(argv[1]);
	if (!srcRaw.data) return -1;

	Mat src;
	resize(srcRaw, srcRaw, imgSize);

	//auto brightness
	BrightnessAndContrastAuto(srcRaw, src, clipHist);

	//hsv mask
	Mat hsv, maskBlue, maskRed1, maskRed2;
	cvtColor(src, hsv, CV_RGB2HSV);

	inRange(hsv, blueLow, blueHigh, maskBlue);
	inRange(hsv, redLow1, redHigh1, maskRed1);
	inRange(hsv, redLow2, redHigh2, maskRed2);

	Mat mask = maskBlue | maskRed1 | maskRed2;
	Mat maskResultHSV;
	bitwise_and(hsv, hsv, maskResultHSV, mask);

	imshow("Source", srcRaw);
	imshow("Auto Brightness", src);

	Mat maskResultRGB;
	cvtColor(maskResultHSV, maskResultRGB, CV_HSV2RGB);

	imshow("Mask HSV Result", maskResultRGB);

	//morphology
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, kernelSize);

	Mat morp;
	morphologyEx(mask, morp, MORPH_OPEN, kernel);

	imshow("Morphology", morp);

	//hsvMorph to gray
	Mat hsvMorph, src_color, src_gray;
	bitwise_and(hsv, hsv, hsvMorph, morp);

	cvtColor(hsvMorph, src_color, CV_HSV2RGB);
	cvtColor(src_color, src_gray, CV_RGB2GRAY);

	//detect contours
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Canny(src_gray, canny_output, cannyThreashMin, 255);
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	//draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	imshow("Contours", drawing);



	//Cascades
	Mat imgCascades = srcRaw;

	CascadeClassifier cascadeRedTriangle;
	if (!cascadeRedTriangle.load("data\\redTriangle\\cascade.xml")) { printf("--(!)Error loading\n"); return -1; };

	//for each found contour
	for each (vector<Point> vect in contours)
	{
		//find rectangle with this contour
		int x0 = INT_MAX, y0 = INT_MAX, x1 = 0, y1 = 0;
		for each (Point p in vect)
		{
			if (p.x <= x0)
				x0 = p.x;
			else if (p.x >= x1)
				x1 = p.x;

			if (p.y <= y0)
				y0 = p.y;
			else if (p.y >= y1)
				y1 = p.y;
		}

		//skip too small rectangles and with wrong shape
		Rect r(x0, y0, x1 - x0, y1 - y0);
		if (r.height < minSignSize || r.width < minSignSize) continue;
		//if (r.height / r.width < 0.5 || r.height / r.width > 2) continue;
		Mat rect = srcRaw(r);

		cout << r.height << " " << r.width << endl;

		std::vector<Rect> foundSigns;

		cascadeRedTriangle.detectMultiScale(rect, foundSigns, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE | CASCADE_SCALE_IMAGE , Size(minSignSize, minSignSize));

		if (foundSigns.size() > 0) {
			Point center(foundSigns[0].x + foundSigns[0].width*0.5 + x0, foundSigns[0].y + foundSigns[0].height*0.5 + y0);
			ellipse(imgCascades, center, Size(foundSigns[0].width*0.5, foundSigns[0].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 3, 8);
		}
	}

	imshow("Cascades", imgCascades);

	waitKey(0);
	return 0;
}