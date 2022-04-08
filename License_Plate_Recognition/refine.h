#ifndef REFINE_REFINES_HPP
#define REFINE_REFINES_HPP

#include <opencv2/opencv.hpp>
#include "preprocessing.h"

void refine_candis(cv::Mat img,cv::RotatedRect& candi);
void rotate_plate(cv::Mat img,cv::Mat & corp_img,cv::RotatedRect candi);
std::vector<cv::Mat>make_candis(cv::Mat img,std::vector<cv::RotatedRect>& candis);




#endif //REFINE_REFINES_HPP
