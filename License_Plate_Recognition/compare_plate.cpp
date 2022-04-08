#include"compare_plate.h"

using namespace cv;
using namespace std;


void addMember(string license,int index)
{
    FileStorage fs("/home/seungmin/Licenses.xml",FileStorage::APPEND);
    fs.isOpened();
    fs<<"members"+to_string(index+1)<<license;
    fs.release();
}


void isMember(string license)
{
    vector<string> basevec;
    FileStorage fs("/home/seungmin/Licenses.xml",FileStorage::READ);
    CV_Assert(fs.isOpened());

    int i=1;
    while((fs["members"+to_string(i)]).name().find(to_string(i),0)==7)//xml데이터이름에 i의 위치(7번째)확인해 데이터가 있는지 판단
    {
        basevec.push_back(fs["members"+to_string(i)]);
        i++;
    }// basevec 생성, xml 데이터 저장

    //검출 시작
    for(i=0;i<basevec.size();){
        if(basevec[i]==license){
            cout<<"회원 확인"<<endl;    //xml에서 해당 번호판 찾음
            break;
        }
        i++;
    }

    if(i==basevec.size())   // 외부차량!
    {
        char yorn;
        cout << "외부 차량" << endl;
        cout<<"회원 추가? y or n"<<endl;
        cin>>yorn;
        if(yorn=='y')// 회원 추가
            addMember(license,i);
    }
    fs.release();
}
