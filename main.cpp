#include <iostream>
#include"cmserver.h"
#include<string>
using namespace std;

int main()
{
    CMServer cmserver;
    if(!cmserver.StartServer())
    {
        cout<<"start server success!"<<endl;
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
