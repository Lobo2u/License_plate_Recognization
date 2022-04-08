#include <opencv2/opencv.hpp>
#include "preprocessing.h"

using namespace std;
using namespace cv;

void draw_rotatedRect(Mat & img,RotatedRect mr,Scalar color,int thickness=2)
{
    Point2f pts[4];
    mr.points(pts);
    for(int i=0;i<4;i++){
        line(img,pts[i],pts[(i+1)%4],color,thickness);
    }
}

Mat preprocessing(Mat img)
{
    Mat gray,th_img,morph;
    Mat kernel(5,15,CV_8UC1,Scalar(1));
    cvtColor(img,gray,COLOR_BGR2GRAY);

    blur(gray,gray,Size(5,5));
    Sobel(gray,gray,CV_8U,1,0,3);

    threshold(gray,th_img,120,255,THRESH_BINARY);
    morphologyEx(th_img,morph,MORPH_CLOSE,kernel,Point(-1,-1),2);
    //모폴로지 닫힘연산, 2번 반복
    imshow("th_img",th_img),imshow("morph",morph);
    return morph;
}

bool vertify_plate(RotatedRect mr)
{
    float size=mr.size.area();
    float aspect =(float)mr.size.width/mr.size.height;
    if(aspect<1)
        aspect=1/aspect;
    bool ch1=size>2000&&size<30000;
    bool ch2=aspect>1.3&&aspect<6.4;

    return ch1&&ch2;
}

void find_candis(Mat img,vector<RotatedRect>& candis)
{
    vector<vector<Point>>contours;
    findContours(img.clone(),contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    for(int i=0;i<(int)contours.size();i++){
        RotatedRect rot_rect=minAreaRect(contours[i]);
        if(vertify_plate(rot_rect)){
            candis.push_back(rot_rect);
        }
    }
}






