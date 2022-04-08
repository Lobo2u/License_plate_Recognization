#include "kNN.h"

using namespace cv;
using namespace std;

void find_histoPos(Mat img,int& start,int &end,int direct)
{
    reduce(img,img,direct,REDUCE_AVG);  //평균으로 가로,세로 방향 감축

    int minFound=0;
    for(int i=0;i<(int)img.total();i++)
    {
        if(img.at<uchar>(i)<250)    //0이 검은색이니까 250보다 작으면 차있다고 인식
        {
            end=i;                      //끝점 인식
            if(!minFound){  //if(minFound==0)   //시작점 인식전에 minFound는 거짓
                start=i;                //시작점 인식
                minFound=1;             //minFound 참으로 변경
            }
        }
    }
}

Mat find_number(Mat part)
{
    Point start,end;
    find_histoPos(part,start.x,end.x,0);  // 수직 투영
    find_histoPos(part,start.y,end.y,1);  // 수평 투영

    return part(Rect(start,end));       //시작,끝 좌표로 사각형 지정
}

Mat place_middle(Mat number,Size new_size)  //number=투영한 숫자
{
    int big=max(number.cols,number.rows);   //40 X 40
    Mat square(big,big,number.type(),Scalar(255));  //정방
    Point start=(square.size()-number.size())/2;    //
    Rect middle(start,number.size());
//    cout<<number.size<<endl;
    number.copyTo(square(middle));

    resize(square,square,new_size);
    square.convertTo(square,CV_32F);

    return square.reshape(0,1);
}

Ptr<ml::KNearest> kNN_train(string train_img,int K,int Nclass,int Nsample)
{
    Size size(40,40);       //한 셀의 크기
    Mat trainData,classLabel;
    Mat training_img=imread(train_img,0);
    CV_Assert(training_img.data);
    //학습할 이미지 이진화
    threshold(training_img,training_img,32,255,THRESH_BINARY);
    for(int i=0,k=0;i<Nclass;i++){
        for(int j=0;j<Nsample;j++,k++){
            Point pt(j*size.width,i*size.height);//40X40을 기준으로 칸 이동
            Rect roi(pt,size);              //학습할 부분 지정
            Mat part = training_img(roi);   //학습할 부분 생성

            Mat num=find_number(part);      //숫자객체 검출
            Mat data=place_middle(num,size);    //40X40의 중심에 숫자 놓기
            trainData.push_back(data);          //학습 데이터 저장
            classLabel.push_back(i);            //레이블 저장
        }
    }
    Ptr<ml::KNearest> knn=ml::KNearest::create();
    knn->train(trainData,ml::ROW_SAMPLE,classLabel);
    return knn;
}

void KNNtrain()
{
    int k1=15,k2=15;    //15개 씩 검색
    Ptr<ml::KNearest>knn[2];
    knn[0]=kNN_train("/home/seungmin/사진/trainimage/train_numbers3.png",k1,10,40);
    knn[0]->save("/home/seungmin/Knn_number.xml");

    knn[1]=kNN_train("/home/seungmin/사진/trainimage/train_texts_f.png",k2,40,20);
    knn[1]->save("/home/seungmin/Knn_text.xml");

}
