#ifndef AUDIO_ADAPTER_H
#define AUDIO_ADAPTER_H

#include"IAdapter.h"
#include"Logger.h"
#include<mutex>
#include<queue>
#include<thread>
#include<condition_variable>
#include"EventData.h"
#include"ManagerMessage.h"
/*===================================
//Audio gRPC
===================================*/
void RunAudioAdapter();
class AudioAdapterImpl : public service::MQTTAdapter::Service
{
    public:
    AudioAdapterImpl();
    ~AudioAdapterImpl();
    // Status sendTriggerDataCANToMQTTSrv(grpc::ServerContext* context, const service::MQTTDataRequest* request, service::MQTTDataResponse* response) override; 
    void playAudioAdapterImpl(const std::string &data);
    void stopAudioAdapterImpl();
    static std::shared_ptr<AudioAdapterImpl> getInstance();
    void addDatatoQueueAudio(const std::string &data);
    void processQueue();

    private:

    static std::shared_ptr<AudioAdapterImpl> mInstance;
    std::unique_ptr<service::ServiceManager::Stub> stub_Service;
    std::unique_ptr<service::AudioSrv::Stub> stub_Audio;
    std::mutex mAddQueueMutex;
    std::queue<std::string> mDataQueue;
    std::condition_variable mDataQueue_Cv;
};
class AudioAdapter : public IAdapter
{
    public:
    void registerService() override;
    void playAudioAdapter(const std::string &data);
    void stopAudioAdapter();
    static AudioAdapter *getInstance();

};
#endif