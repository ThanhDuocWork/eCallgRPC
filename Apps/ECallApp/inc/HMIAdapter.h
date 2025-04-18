#ifndef HMI_ADAPTER_H
#define HMI_ADAPTER_H

#include"IAdapter.h"
#include"Logger.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include "json.hpp"  // Nlohmann JSON library
#include <cstring>
#include "Logger.h"
#include<thread>
#include<chrono>
// #include "MQTTAdapter.h"
// #include"AudioAdapter.h"s
#include<condition_variable>
#include<mutex>



using json = nlohmann::json;
/*===================================
//HMI gRPC
===================================*/
void RunHMIAdapter();

class HMIAdapterImpl : public service::HMIAdapter::Service {
    public:
        // HMIAdapterImpl();
    
        Status requestButtonHMIAdapter(grpc::ServerContext* context, const service::HMIDataRequest* request, service::HMIDataResponse* response) override;
        
    };
/*===================================
===================================*/

class HMIAdapter : public IAdapter
{
    public:
    void registerService() override;

    static HMIAdapter *getInstance();
    void handleRevConmamndLCD();
    void onLCDScreen(const std::string &response);

    const std::string getDataReceiveHMISrv();
    void setDataReceiveHMISrv(const std::string data);
    void clearDataReceiveHMISrv ();

    private:
    std::string mDataReceiveHMISrv;



    std::string mDataButton;
    std::mutex mCommandMutex;

    std::string mSocketCommand;
    std::mutex mSocketMutex;
};
#endif