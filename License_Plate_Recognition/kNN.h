#ifndef GETNUM_KNN_HPP
#define GETNUM_KNN_HPP

#include<opencv2/opencv.hpp>

void find_histoPos(cv::Mat img,int& start,int &end,int direct);     //투영 히스토그램으로 시작,종료 좌표 찾기
cv::Mat find_number(cv::Mat part);
cv::Mat place_middle(cv::Mat number,cv::Size new_size);     //분류 정확도 향상
cv::Ptr<cv::ml::KNearest> kNN_train(std::string train_img,int K,int Nclass,int Nsample);
void KNNtrain();
//train_img=학습할 이미지, k=선출할 이웃 수, Nclass=분류 기준, 기준하나당 학습량


#endif //GETNUM_KNN_HPP
