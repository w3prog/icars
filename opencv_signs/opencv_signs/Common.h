#pragma once
#include <opencv2\opencv.hpp>

void BrightnessAndContrastAuto(const cv::Mat &src, cv::Mat &dst, float clipHistPercent = 0);