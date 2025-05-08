#include "AudioAdapter.h"

std::shared_ptr<AudioAdapterImpl> AudioAdapterImpl::mInstance = nullptr;
void RunAudioAdapter ()
{
    std::string server_address ("0.0.0.0:50056");

    auto Audio_AdapterImpl = AudioAdapterImpl::getInstance ();

    grpc::ServerBuilder builder;
    builder.AddListeningPort (server_address, grpc::InsecureServerCredentials ());
    builder.RegisterService (Audio_AdapterImpl.get ());

    std::unique_ptr<grpc::Server> server (builder.BuildAndStart ());
    printLog_I ("MQTTSrv listening on:%s", server_address.c_str ());

    std::thread senderThread (&AudioAdapterImpl::processQueue, Audio_AdapterImpl);
    senderThread.detach ();

    server->Wait ();
}

AudioAdapterImpl::AudioAdapterImpl ()
{
    // default port is 50051 to connect to port server
    std::string server_address = "localhost:50051";
    auto channel               = grpc::CreateChannel (server_address, grpc::InsecureChannelCredentials ());
    stub_Service               = service::ServiceManager::NewStub (channel); // set to get data

    std::string Audiosrv_address = "localhost:50053";
    auto channel_Audiosrv        = grpc::CreateChannel (Audiosrv_address, grpc::InsecureChannelCredentials ());
    stub_Audio                   = service::AudioSrv::NewStub (channel_Audiosrv);
}
AudioAdapterImpl::~AudioAdapterImpl ()
{
}
void AudioAdapterImpl::addDatatoQueueAudio (const std::string &data)
{
    {
        std::lock_guard<std::mutex> lock (mAddQueueMutex);
        mDataQueue.push (data);
    }
    mDataQueue_Cv.notify_one ();
}
void AudioAdapterImpl::processQueue ()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock (mAddQueueMutex);
        if (mDataQueue.empty ())
        {
            printLog_E ("processQueue mDataQueue audio empty!!");
        }
        mDataQueue_Cv.wait (lock, [this] { return !mDataQueue.empty (); });
        std::string dataQueue = mDataQueue.front ();
        mDataQueue.pop ();
        lock.unlock ();
        printLog_I ("Processing data: %s", dataQueue.c_str());
        playAudioAdapterImpl (dataQueue);
    }
}
std::shared_ptr<AudioAdapterImpl> AudioAdapterImpl::getInstance ()
{
    if (mInstance == nullptr)
    {
        mInstance = std::make_shared<AudioAdapterImpl> ();
    }
    return mInstance;
}
void AudioAdapterImpl::playAudioAdapterImpl (const std::string &data)
{
    service::AudioDataRequest request;
    service::AudioDataResponse response;
    request.set_data (data);

    grpc::ClientContext context;
    grpc::Status status = stub_Audio->receivePlayAudioSrv (&context, request, &response);
    if (status.ok () && response.success ())
    {
        printLog_I ("Data successfully sent to AudioSrv.");
    }
    else
    {
        printLog_E ("Failed to send data to AudioSrv: " + status.error_message ());
    }
}
/*===================================
===================================*/
void AudioAdapter::registerService ()
{
    // default port is 50051 to connect to port server
    std::string server_address = "localhost:50051";
    auto channel               = grpc::CreateChannel (server_address, grpc::InsecureChannelCredentials ());
    auto stub                  = service::ServiceManager::NewStub (channel);
    service::RegisterAudioRequest request;
    service::RegisterAudioResponse response;
    request.set_client_id (gRPC_Service::gRPC_AUDIO_SERVICE_NAME);

    grpc::ClientContext context;
    Status status = stub->registerAudioMgrService (&context, request, &response);

    if (status.ok () && response.success ())
    {
        printLog_I ("Registered successfully!!!");
    }
    else
    {
        
        printLog_I ("Failed to register!!!" + status.error_message ());
    }
}
void AudioAdapter::playAudioAdapter (const std::string &data)
{
    AudioAdapterImpl::getInstance ()->addDatatoQueueAudio (data);
}
AudioAdapter *AudioAdapter::getInstance ()
{
    static std::shared_ptr<AudioAdapter> instance;
    if (instance == nullptr)
    {
        instance = std::make_shared<AudioAdapter> ();
    }
    return instance.get ();
}