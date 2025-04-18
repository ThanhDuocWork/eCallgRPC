#include "CanBus.h"

CanBus::CanBus ()
{
    initSocketCan ();
}
std::shared_ptr<CanBus> CanBus::getIntance ()
{
    std::shared_ptr<CanBus> instance;
    if (instance == nullptr)
    {
        instance = std::make_shared<CanBus> ();
    }
    return instance;
}
void CanBus::initSocketCan ()
{
    sock = socket (PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0)
    {
        printLog_E ("Can't open socket CAN!!!");
        return;
    }

    // CAN interface (`slcan0`)
    strcpy (ifr.ifr_name, CAN_INTERFACE);
    if (ioctl (sock, SIOCGIFINDEX, &ifr) < 0)
    {
        printLog_E ("Can't find interface CAN: %s", CAN_INTERFACE);
        close (sock);
        return;
    }

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Connect socket with protocol CAN (`slcan0`)
    if (bind (sock, (struct sockaddr *)&addr, sizeof (addr)) < 0)
    {
        printLog_E ("Can't connect CAN bus!");
        close (sock);
        return;
    }

    printLog_I ("listening data from CAN bus: %s", CAN_INTERFACE);
}
CanInfo CanBus::getDataCan()
{
    CanInfo result = {0 ,0, 0, ""};
    bool gotLocation = false;
    bool gotTimestamp = false;
    for (int i = 0; i < 4; i++)
    {
        int mReadCan = read(sock, &frame, sizeof(struct can_frame));
        if (mReadCan > 0)
        {
            std::stringstream PrintDataCan;
            for (int j = 0; j < frame.can_dlc; j++)
            {
                char hexByte[4];
                snprintf(hexByte, sizeof(hexByte), "%02X ", frame.data[j]);
                PrintDataCan << hexByte;
            }

            std::string rawData = PrintDataCan.str();
            result.CanID = frame.can_id;
            if (frame.can_id == 0x4)
            {
                if (frame.data[0] != 0xFF  && !gotLocation) // location
                {
                    std::string gps = convertDataCanToLocation(rawData);
                    sscanf(gps.c_str(), "%f, %f", &result.latitude, &result.longitude);
                    gotLocation = true;
                }
                else if (frame.data[0] == 0xFF && !gotTimestamp) // timestamp
                {
                    std::string dataTime = convertDataCanToTimestamp(rawData);
                    result.timestamp = dataTime;
                    gotTimestamp = true;
                }
            }
        }
        usleep(100000);
    }
    printLog_I("Final CAN Result - lat: %.6f, lon: %.6f, time: %s", result.latitude, result.longitude, result.timestamp.c_str());

    close(sock);
    return result;
}


std::string CanBus::convertDataCanToLocation (const std::string &input)
{
    std::stringstream scanInput (input);
    std::string number;
    std::vector<int> vectorBitData;
    // cout >> vector data
    while (scanInput >> number)
    {
        vectorBitData.push_back (stoi (number, nullptr, 16));
    }

    if (vectorBitData.size () < 8)
    {
        return "Invalid Data size (bit)!";
    }

    uint8_t latIneger = (uint8_t)vectorBitData[0];    
    uint32_t latDecimal = (uint32_t)(vectorBitData[1] | (vectorBitData[2] << 8) | (vectorBitData[3] << 16));   
    float latitude = latIneger + (float)latDecimal / 1000000.0f;
 
    uint8_t lonIneger = (uint8_t)vectorBitData[4];    
    uint32_t lonDecimal = (uint32_t)(vectorBitData[5] | (vectorBitData[6] << 8) | (vectorBitData[7] << 16));  
    float longitude = lonIneger + (float)lonDecimal / 1000000.0f;

    return std::to_string(latitude) + ", " + std::to_string(longitude);
}
std::string CanBus::convertDataCanToTimestamp (const std::string &input)
{
    std::stringstream scanInput (input);
    std::string number;
    std::vector<int> vectorBitData;
    // cout >> vector data
    while (scanInput >> number)
    {
        vectorBitData.push_back (stoi (number, nullptr, 16));
    }

    if (vectorBitData.size () < 8)
    {
        return "Invalid Data size (bit)!";
    } 
    uint32_t time = (vectorBitData[1]) | (vectorBitData[2] << 8) | (vectorBitData[3] << 16);
    uint8_t hours = time / 10000; 
    uint8_t minutes = (time / 100) % 100; 
    uint8_t seconds =time % 100; 

    std::stringstream timeStampSS;
    timeStampSS << (int)hours << ":" << (int)minutes << ":" << (int)seconds;
    std::string timeStamp = timeStampSS.str();
    return timeStamp;
}
