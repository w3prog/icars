#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

using std::vector;
using std::pair;
using namespace cv;

Mat maskColor(Mat img, vector<pair<Scalar, Scalar> > colors)
{
	Mat res;
	vector<pair<Scalar, Scalar> >::iterator it;
	for(it = colors.begin(); it != colors.end(); it++)    {
		Mat cur;
		inRange(img, (*it).first, (*it).second, cur);
		if(it == colors.begin())
			res = cur;
		else
			res = res | cur;
	}
return res;
}

Mat equalizeIntensity(const Mat& inputImage)
{
    if(inputImage.channels() >= 3)
    {
        Mat ycrcb;
        cvtColor(inputImage,ycrcb,CV_BGR2YCrCb);

        vector<Mat> channels;
        split(ycrcb,channels);

        equalizeHist(channels[0], channels[0]);

        Mat result;
        merge(channels,ycrcb);
        cvtColor(ycrcb,result,CV_YCrCb2BGR);

        return result;
    }

    return Mat();
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], 1 );

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

Mat HSV;

image = equalizeIntensity(image);

cvtColor(image, HSV, CV_BGR2HSV);

//red
Scalar red_hsv_upper_l(170, 150, 150);
Scalar red_hsv_upper_h(180, 255, 255);

Scalar red_hsv_lower_l(0, 150, 150);
Scalar red_hsv_lower_h(10, 255, 255);

//blue
Scalar blue_hsv_l(90, 70, 56);
Scalar blue_hsv_h(130, 255, 255);


vector<pair<Scalar, Scalar> > colors;
colors.push_back(pair<Scalar,Scalar>(red_hsv_upper_l, red_hsv_upper_h));
colors.push_back(pair<Scalar,Scalar>(red_hsv_lower_l, red_hsv_lower_h));
colors.push_back(pair<Scalar,Scalar>(blue_hsv_l, blue_hsv_h));


Mat whole_red_hue = maskColor(HSV, colors);

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", whole_red_hue);

    waitKey(0);

    return 0;
}
