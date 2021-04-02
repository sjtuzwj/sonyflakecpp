#ifndef STORAGE_IP_H
#define STORAGE_IP_H
#include <string>


namespace storage
{

class AddrInfo{
public:
    static AddrInfo & getInstance();
    //set interface to communicate like wlan0
    void setInterface(const std::string& interface);
    //get ip on the interface
    std::string getPublicIP();
private:
//get inv4 addr
//according to a specific interface like wlan0 or ens33
    void setPublicIP();
    AddrInfo() = default;
    std::string interface;
    std::string ipv4;
};
}

#endif