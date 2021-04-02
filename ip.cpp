/**
 * get IPv4 address and subnet mask of a network interface
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string>
#include <ip.h>
using std::string;
extern std::string local_addr;
extern std::string public_addr;

namespace storage
{
std::string get_interface_ip(std::string interface)
{
        int rc = 0;
        struct sockaddr_in *addr = NULL;

        struct ifreq ifr;
        memset(&ifr, 0, sizeof(struct ifreq));

        /* create a socket */
        int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (fd == -1)
                return std::string();

        /* set type of address to retrieve : IPv4 */
        ifr.ifr_addr.sa_family = AF_INET;

        /* copy interface name to ifreq structure */
        strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ - 1);

        /* get the IP address */
        if ((rc = ioctl(fd, SIOCGIFADDR, &ifr)) != 0)
        {

        close(fd);

        return std::string();
        }

        char ipv4[16] = { 0 };
        addr = (struct sockaddr_in *)&ifr.ifr_addr;
        strncpy(ipv4, inet_ntoa(addr->sin_addr), sizeof(ipv4));

        /* get the mask */
        if ((rc = ioctl(fd, SIOCGIFNETMASK, &ifr)) != 0)
        {
        close(fd);
        return std::string(ipv4);
        }

        char mask[16] = { 0 };
        addr = (struct sockaddr_in *)&ifr.ifr_addr;
        strncpy(mask, inet_ntoa(addr->sin_addr), sizeof(mask));

        /* close the socket */
        close(fd);

        return std::string(ipv4);
}


    AddrInfo & AddrInfo::getInstance()
    {
        static AddrInfo instance;
        return instance;
    }

    void AddrInfo::setInterface(const std::string& in_interface)
    {
        interface = in_interface;
        setPublicIP ();
    }

    std::string AddrInfo::getPublicIP()
    {
        return ipv4;
    }

    void AddrInfo::setPublicIP()
    {
        ipv4 = get_interface_ip(interface);
    }
}