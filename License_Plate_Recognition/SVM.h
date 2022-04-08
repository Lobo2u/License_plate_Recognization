#ifndef CLASSIFYING_SVM_HPP
#define CLASSIFYING_SVM_HPP

#include <opencv2/opencv.hpp>

int classify_plate(cv::Ptr<cv::ml::SVM> svm,std::vector<cv::Mat> candi_img);


#endif