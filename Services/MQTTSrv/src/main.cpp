#include"MQTTFunc.h"
#include"MQTTSrv.h"


int main()
{
    std::thread gRPCServer(RunMQTTSrv);
    gRPCServer.detach();
    while (true)
    {
        // MQTTFunc::getInstance()->testLog();
        std::this_thread::sleep_for (std::chrono::seconds (1));
    }    
}