#ifndef GETNUM_CLASSIFY_OBJECTS_HPP
#define GETNUM_CLASSIFY_OBJECTS_HPP

#include<opencv2/opencv.hpp>


void preprocessing_plate(cv::Mat plate_img,cv::Mat& ret_img);   //번호판 전처리
void find_objects(cv::Mat sub_mat,std::vector<cv::Rect>& object_rects);     //숫자,문자 검색
void sort_rects(std::vector <cv::Rect> obj_rects,std::vector<cv::Rect>& sorted_rects);      //찾은 숫,문자 정렬
std::string classify_numbers(std::vector<cv::Mat>numbers,cv::Ptr<cv::ml::KNearest>knn[2],int K1,int K2);
void isMember(std::string license);

#endif //GETNUM_CLASSIFY_OBJECTS_HPP
