#ifndef HMI_SRV_FUNC
#define HMI_SRV_FUNC
#include<thread>
#include "Logger.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include<chrono>
#include <arpa/inet.h> // socket
#include <condition_variable>
#include<functional>

#define SOCKET_PORT 1234

class HMISrvFunc
{
    public:
    HMISrvFunc();
    void initSocket();
    void runServerSocketHMI();
    void handleRevConmamndLCD();
    static HMISrvFunc *getInstance();
    const std::string getSocketCommand();
    void setSocketCommand(const std::string &command);
    void registerCallback(std::function<void(const std::string&)> callback);

    private:
    bool mData;
    int server_fd;
    int mClientSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    std::string mSocketCommand;
    std::condition_variable mDataCV;
    std::mutex mDataCommandMutex;
    std::function<void(const std::string&)> mCallback; 
};

#endif