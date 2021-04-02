#include <snowflake.h>
#include <iostream>
#include <ip.h>

using namespace std;

 
int main()
{
    
    storage::AddrInfo::getInstance().setInterface("ens33");

    while(true)
    {   
        long long tmp = SnowFlake::getInstance().nextId();
        cout << tmp << endl;
    }
}