#include <iostream>
#include"cmserver.h"
#include"sharedata.h"
#include<string>
using namespace std;

int main()
{
    CMServer cmserver;
    if(!cmserver.StartServer())
    {
        cout<<"start server success!"<<sizeof(int)<<endl;
    }
    else
    {
        cout<<cmserver.GetLastError()<<endl;
        cout<<"start server failed!"<<endl;
        return -1;
    }

    string cmd;
    while(true)
    {
        cin>>cmd;
        if(cmd=="exit")
        {
            cout<<"Exit Server"<<endl;
            break;
        }
    }
    return 0;
}
