#ifndef CAN_BUS_H
#define CAN_BUS_H
#include <iostream>
#include <cstring>
#include <cstdlib>   // system()
#include <unistd.h>  // read(), close()
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <memory>
#include"Logger.h"
#include<string>
#include<vector>

#define CAN_INTERFACE "slcan0"
struct CanInfo
{   
    uint16_t CanID;
    float latitude;
    float longitude;
    std::string timestamp;
};
class CanBus
{
    private:
    int sock;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    int mReadCan;
    std::stringstream mPrintDataCan;
    // std::stringstream mScan (const std::string &input);
    // std::vector<int> mVectorBitData;

    public:
    CanBus();
    void initSocketCan();
    CanInfo getDataCan();
    // std::pair<float, float>  getDataCanTime();
    static std::shared_ptr<CanBus> getIntance();
    std::string convertDataCanToLocation(const std::string &input);
    std::string convertDataCanToTimestamp(const std::string &input);
};
#endif