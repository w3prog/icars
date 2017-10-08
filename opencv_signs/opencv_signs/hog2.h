#pragma once
#include "Common.h"
#include <vector>
#include <opencv2/core.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

int mainHog2(int argc, char** argv);
void get_svm_detector(const Ptr< SVM > & svm, vector< float > & hog_detector);
void convert_to_ml(const std::vector< Mat > & train_samples, Mat& trainData);
void load_images(const String & dirname, vector< Mat > & img_lst, bool showImages);
void sample_neg(const vector< Mat > & full_neg_lst, vector< Mat > & neg_lst, const Size & size);
void computeHOGs(const Size wsize, const vector< Mat > & img_lst, vector< Mat > & gradient_lst);
int test_trained_detector(String obj_det_filename, String test_dir, String videofilename);