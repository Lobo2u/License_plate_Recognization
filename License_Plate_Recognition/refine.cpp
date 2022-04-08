#include "refine.h"

using namespace std;
using namespace cv;

void refine_candis(Mat img,RotatedRect& candi)
{
    Mat fill(img.size()+Size(2,2),CV_8UC1,Scalar(0));
    Scalar dif1(25,25,25),dif2(25,25,25);
    int flags=4+0xff00;
    flags+=FLOODFILL_FIXED_RANGE+FLOODFILL_MASK_ONLY;
    vector<Point2f>rand_pt(15);
    randn(rand_pt,0,7);
    Rect img_rect(Point(0,0),img.size());
    for(int i=0;i<rand_pt.size();i++){
        Point2f seed=candi.center+rand_pt[i];
        Rect rect;
        if(img_rect.contains(seed)){
            floodFill(img,fill,seed,Scalar(0),&rect,dif1,dif2,flags);
        imshow("flood",img);
        }
    }
    vector<Point> fill_pts;
    int k=0;
    for(int i=0;i<fill.rows;i++){
        for(int j=0;j<fill.cols;j++){
            if(fill.at<uchar>(i,j)==255)
            {
                fill_pts.push_back(Point(j,i));

                k++;
            }
        }
    }
    candi=minAreaRect(fill_pts);
}

void rotate_plate(Mat img,Mat & corp_img,RotatedRect candi)
{
    float aspect=(float)candi.size.width/candi.size.height;
    float angle=candi.angle;
    if(aspect<1){
        swap(candi.size.width,candi.size.height);
        angle+=90;
    }
    Mat rotmat= getRotationMatrix2D(candi.center,angle,1);
    warpAffine(img,corp_img,rotmat,img.size(),INTER_CUBIC);
    getRectSubPix(corp_img,candi.size,candi.center,corp_img);
}

vector<Mat>make_candis(Mat img,vector<RotatedRect>& candis)
{
    vector<Mat> candis_img;
    for(int i=0;i<(int)candis.size();){
        refine_candis(img,candis[i]);
        if(vertify_plate(candis[i])){
            Mat corp_img;
            rotate_plate(img,corp_img,candis[i]);
            cvtColor(corp_img,corp_img,COLOR_BGR2GRAY);
            resize(corp_img,corp_img,Size(144,28),0,0,INTER_CUBIC);
            candis_img.push_back(corp_img);
            i++;
        }
        else{
            candis.erase(candis.begin()+i);
        }
    }
    return candis_img;
}

