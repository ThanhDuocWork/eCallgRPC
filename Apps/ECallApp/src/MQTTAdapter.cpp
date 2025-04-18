#include "MQTTAdapter.h"

std::shared_ptr<MQTTAdapterImpl> MQTTAdapterImpl::mInstance = nullptr;
MQTTAdapterImpl::MQTTAdapterImpl ()
{
    // default port is 50051 to connect to port manager server
    std::string server_address = "localhost:50051";
    auto channel               = grpc::CreateChannel (server_address, grpc::InsecureChannelCredentials ());
    stub_Service                 = service::ServiceManager::NewStub (channel);

    std::string MQTTsrv_address = "localhost:50054";
    auto channel_MQTTsrv        = grpc::CreateChannel (MQTTsrv_address, grpc::InsecureChannelCredentials ());
    stub_MQTTSrv                = service::MQTTSrv::NewStub (channel_MQTTsrv);
}
Status MQTTAdapterImpl::sendTriggerDataCANToMQTTSrv (grpc::ServerContext *context, const service::MQTTDataRequest *request, service::MQTTDataResponse *response)
{
    service::MQTTDataRequest sm_request;
    service::MQTTDataResponse sm_response;

    // const std::string dataButton = HMISrvFunc::getInstance()->getSocketCommand();
    sm_request.set_data (request->data ());

    grpc::ClientContext sm_context;
    grpc::Status status = stub_MQTTSrv->receiveTriggerDataCAN (&sm_context, sm_request, &sm_response);

    if (status.ok () && sm_response.success ())
    {
        printLog_I ("Data successfully sent to MQTTsrv.");
        response->set_success (true);
    }
    else
    {
        printLog_E ("Failed to send data to MQTTsrv: " + status.error_message ());
        response->set_success (false);
    }

    return grpc::Status::OK;
}
std::shared_ptr<MQTTAdapterImpl> MQTTAdapterImpl::getInstance ()
{
    if (mInstance == nullptr)
    {
        mInstance = std::make_shared<MQTTAdapterImpl> ();
    }
    return mInstance;
}
void MQTTAdapterImpl::sendLocatedandTimestamp (const std::string &data)
{
    // printLog_I ("SendDataToHMIAdapter() triggered with data: " + data);
    service::MQTTDataRequest request;
    service::MQTTDataResponse response;
    // const std::string dataButton = HMISrvFunc::getInstance ()->getSocketCommand ();
    // data = dataButton;
    // printLog_I ("check data loop:%s", data.c_str ());
    // data = "test log locaton";
    request.set_data (data);

    grpc::ClientContext context;
    grpc::Status status = stub_MQTTSrv->receiveTriggerDataCAN (&context, request, &response);
    // if (data.empty ())
    // {
    //     printLog_E ("SendDataLoop: No data to send! Waiting for new data...");
    // }
    // else
    // {
    //     printLog_I ("SendDataLoop: Sending data: " + dataButton);
    // }
    if (status.ok () && response.success ())
    {
        printLog_I ("Data successfully sent to MQTTSrv.");
    }
    else
    {
        printLog_E ("Failed to send data to MQTTSrv: " + status.error_message ());
    }

    // std::this_thread::sleep_for (std::chrono::seconds (5));
}
void MQTTAdapter::registerService ()
{
    // default port is 50051 to connect to port server
    std::string server_address = "localhost:50051";
    auto channel               = grpc::CreateChannel (server_address, grpc::InsecureChannelCredentials ());
    auto stub                  = service::ServiceManager::NewStub (channel);

    service::RegisterMQTTRequest request;
    service::RegisterMQTTResponse response;
    request.set_client_id (gRPC_Service::gRPC_MQTT_SERVICE_NAME);

    grpc::ClientContext context;
    Status status = stub->registerMQTTMgrService (&context, request, &response);

    if (status.ok () && response.success ())
    {
        printLog_I ("Registered successfully!!!");
    }
    else
    {
        printLog_I ("Failed to register!!!" + status.error_message ());
    }
}

MQTTAdapter *MQTTAdapter::getInstance ()
{
    static std::shared_ptr<MQTTAdapter> instance;
    if (instance == nullptr)
    {
        instance = std::make_shared<MQTTAdapter> ();
    }
    return instance.get ();
}
void MQTTAdapterImpl::addDataToQueue (const std::string &data)
{
    {
        std::lock_guard<std::mutex> lock (queueMutex);
        dataQueue.push (data);
    }
    queueCV.notify_one ();
}

void MQTTAdapterImpl::processQueue ()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock (queueMutex);
        queueCV.wait (lock, [this] { return !dataQueue.empty (); });

        std::string data = dataQueue.front ();
        dataQueue.pop ();
        lock.unlock ();

        printLog_I ("Processing data: " + data);
        sendLocatedandTimestamp (data);
    }
}
void RunMQTTAdapter ()
{
    std::string server_address ("0.0.0.0:50057");

    auto MQTT_Adapter = MQTTAdapterImpl::getInstance ();

    grpc::ServerBuilder builder;
    builder.AddListeningPort (server_address, grpc::InsecureServerCredentials ());
    builder.RegisterService (MQTT_Adapter.get ());

    std::unique_ptr<grpc::Server> server (builder.BuildAndStart ());
    printLog_I ("MQTTSrv listening on:%s", server_address.c_str ());

    std::thread senderThread (&MQTTAdapterImpl::processQueue, MQTT_Adapter);
    senderThread.detach ();

    server->Wait ();
}
