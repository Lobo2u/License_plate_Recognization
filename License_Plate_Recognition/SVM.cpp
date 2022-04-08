#include "SVM.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int classify_plate(cv::Ptr<cv::ml::SVM> svm,std::vector<cv::Mat> candi_img)
{
    for(int i=0;i<(int)candi_img.size();i++){
        cv::Mat onerow=candi_img[i].reshape(1,1);
        onerow.convertTo(onerow,CV_32F);

        cv::Mat results;
        svm->predict(onerow,results);
 //       cout<<"예측: "<<results.at<float>(0)<<endl;
        if(results.at<float>(0)==1)
            return i;
    }
    return -1;
}