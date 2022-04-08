#ifndef refine_PREPROCESS_HPP
#define refine_PREPROCESS_HPP

#include <opencv2/opencv.hpp>

cv::Mat preprocessing(cv::Mat img); //전처리
bool vertify_plate(cv::RotatedRect mr); //
void find_candis(cv::Mat img, std::vector<cv::RotatedRect> & candis);
void draw_rotatedRect(cv::Mat & img,cv::RotatedRect mr,cv::Scalar color,int thickness);

#endif //CAR1_PREPROCESS_HPP
