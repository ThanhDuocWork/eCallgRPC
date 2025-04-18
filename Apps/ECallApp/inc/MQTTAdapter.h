#ifndef MQTT_ADAPTER_H
#define MQTT_ADAPTER_H

#include"IAdapter.h"
#include"Logger.h"
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<string>
/*===================================
//MQTT gRPC
===================================*/
void RunMQTTAdapter();
class MQTTAdapterImpl : public service::MQTTAdapter::Service {
    public:
        MQTTAdapterImpl();    
        Status sendTriggerDataCANToMQTTSrv(grpc::ServerContext* context, const service::MQTTDataRequest* request, service::MQTTDataResponse* response) override;    
        void sendLocatedandTimestamp(const std::string &data);
        void processQueue();
        void addDataToQueue(const std::string &data);
        static std::shared_ptr<MQTTAdapterImpl> getInstance();

    private:
    std::unique_ptr<service::MQTTSrv::Stub> stub_MQTTSrv; 
    std::unique_ptr<service::ServiceManager::Stub> stub_Service; 

    static std::shared_ptr<MQTTAdapterImpl> mInstance;
    std::queue<std::string> dataQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    };
/*===================================
===================================*/

class MQTTAdapter : public IAdapter
{
    public:
    void registerService() override;
    static MQTTAdapter *getInstance();
};
#endif