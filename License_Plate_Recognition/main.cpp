#include "preprocessing.h"
#include "refine.h"
#include "SVM.h"
#include "kNN.h"
#include "classify_objects.h"

using namespace std;
using namespace cv;


int main()
{
    string license;
    KNNtrain();
    Ptr<ml::SVM> svm=ml::StatModel::load<ml::SVM>("/home/seungmin/SVMtrain.xml");   //번호판을 학습한 데이터로 svm객체 생성
    Ptr<ml::KNearest>knn[2];
    knn[0]=ml::StatModel::load<ml::KNearest>("/home/seungmin/Knn_number.xml");
    knn[1]=ml::StatModel::load<ml::KNearest>("/home/seungmin/Knn_text.xml");

    int car_no;
    cout<<"차량번호(0~20) : ";
    cin>>car_no;
    string fn=format("/home/seungmin/사진/test_car/%02d.jpg",car_no);
    Mat img=imread(fn,1);   //테스트할 차량 이미지 불러오기
    CV_Assert(img.data);

    Mat morph= preprocessing(img);
    vector<RotatedRect> candis;
    find_candis(morph,candis);
    vector<Mat>candis_img=make_candis(img,candis);

    int plate_no= classify_plate(svm,candis_img);

    if(plate_no>=0){        //번호판 숫자, 문자 검출
        vector<Rect>object_rects,sorted_rects;
        vector<Mat> nums;
        Mat plate_img,color_plate;

        preprocessing_plate(candis_img[plate_no],plate_img);
        cvtColor(plate_img,color_plate,COLOR_GRAY2BGR);

        find_objects(~plate_img,object_rects);
        sort_rects(object_rects,sorted_rects);

//        cout<<sorted_rects.size()<<endl;
        for(size_t i=0;i<sorted_rects.size();i++){
            nums.push_back(plate_img(sorted_rects[i]));
            rectangle(color_plate,sorted_rects[i],Scalar(0,0,255),1);
        }
        if(nums.size()==7){
            license=classify_numbers(nums,knn,15,15);
        }
        else cout<<"숫자(문자) 객체 검출 x"<<endl;

        imshow("번호판",color_plate);
        draw_rotatedRect(img,candis[plate_no],Scalar(0,255,0),2);
    }
    else cout<<"번호판 X"<<endl;

    imshow("img",img);

    isMember(license);
    waitKey(0);
    return 0;
}
