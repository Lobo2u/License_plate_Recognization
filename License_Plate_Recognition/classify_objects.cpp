#include<opencv2/opencv.hpp>
#include<cstring>
#include"classify_objects.h"
#include "kNN.h"

using namespace cv;
using namespace std;

void preprocessing_plate(Mat plate_img,Mat& ret_img)
{
    resize(plate_img,plate_img,Size(180,35));   //번호판의 크기 지정
    threshold(plate_img,plate_img,32,255,THRESH_BINARY|cv::THRESH_OTSU);

    Point pt1=Point(6,3);   //왼쪽 위 모서리 기준
    Point pt2=plate_img.size()-Size(pt1);   //오른쪽 아래 모서리 기준
    ret_img=plate_img(Rect(pt1,pt2)).clone();  // 모서리 기준으로 사각형 만들고 그 범위를 복사
}

void find_objects(Mat sub_mat,vector<Rect>& object_rects)
{
    vector<vector<Point>>contours;
    //객체의 외곽선 검출
    findContours(sub_mat,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    vector<Rect>text_rects;
   // cout<<(int)contours.size()<<endl;
    for(int i=0;i<(int)contours.size();i++){
        Rect r= boundingRect(contours[i]);      // 검출된 번호판의 후보 사각형
        if(r.width/(float)r.height > 2.5) continue;     //종횡비가 2.5 이상 = 가로가 너무 길다

        if(r.x>45&&r.x<80&&r.area()>60)          // 45 < r.x < 80 , r.area() > 60 는 문자
            text_rects.push_back(r);            //객체 저장
        else if(r.area()>150)                   //r.area() > 150 은 숫자
            object_rects.push_back(r);          // 객체 저장
    }
//    cout<<text_rects.size()<<endl;
    if(text_rects.size()>0){    //문자 객체 발견
        for(size_t i=1;i<(int)text_rects.size();i++){
            text_rects[0] |=text_rects[i];   //0번쨰로 이동= 객체 한개로 누적
        }
        object_rects.push_back(text_rects[0]);
    }
  //  for(vector<Rect>::const_iterator iter=object_rects.cbegin();iter!=object_rects.cend();iter++)
   //     cout<<iter->x<<" ";
    //cout<<endl;
  //  cout<<object_rects.size()<<endl;
}

void sort_rects(vector <Rect> obj_rects,vector<Rect>& sorted_rects)
{
    Mat pos_x;
    for(size_t i=0;i<obj_rects.size();i++){
        pos_x.push_back(obj_rects[i].x);  //검출한 사각형의 x 좌표를 pos_x에 저장
    }

    sortIdx(pos_x,pos_x,SORT_EVERY_COLUMN+cv::SORT_ASCENDING);  // 열단위, 오름차순으로 정렬
   for(int i=0;i<pos_x.rows;i++){
        int idx=pos_x.at<int>(i,0); //정렬한 사각형의 좌표를 기준으로 사각형의 인덱스를 매김
        sorted_rects.push_back(obj_rects[idx]); //그 인덱스대로 사각형을 정렬해서 저장
     // cout<<sorted_rects[i].x<<endl;
    }
}

string classify_numbers(vector<Mat>numbers,Ptr<ml::KNearest>knn[2],int K1,int K2)
{
    string text_valus[]={
            "가","나","다","라","마",
            "거","너","더","러","머",
            "고","노","도","로","모",
            "구","누","두","루","무",
            "바","사","아","자","허",
            "버","서","어","저","보",
            "소","오","조","호","하",
            "배","부","수","우","주"
    };      //레이블 값에 따른 문자 분류
   cout<<"분류 결과: ";
   // cout<<numbers.size()<<endl;
    string license; //번호판 문자열
    for(int i=0;i<(int)numbers.size();i++){
        Mat num= find_number(numbers[i]);
        Mat data= place_middle(num,Size(40,40));
       // cout<<num.size();
        Mat results;
        if(i==2){
            knn[1]->findNearest(data,K1,results);
           // cout<<text_valus[(int)results.at<float>(0)]<<" ";
            license+= text_valus[(int)results.at<float>(0)];
        }
        else{
            knn[0]->findNearest(data,K2,results);
           // cout<<results.at<float>(0)<<" ";
            license+= to_string((int)results.at<float>(0));
        }
    }
    cout<<license<<endl;
    return license;
}