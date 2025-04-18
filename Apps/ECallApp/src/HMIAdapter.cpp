#include "HMIAdapter.h"
// start Server HMI
void RunHMIAdapter ()
{
    std::string hmi_server_address ("0.0.0.0:50055");
    HMIAdapterImpl hmi_adapter_service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort (hmi_server_address, grpc::InsecureServerCredentials ());
    builder.RegisterService (&hmi_adapter_service);

    std::unique_ptr<grpc::Server> server (builder.BuildAndStart ());
    printLog_I ("HMIAdapter listening on: %s", hmi_server_address.c_str ());

    server->Wait ();
}
Status HMIAdapterImpl::requestButtonHMIAdapter (grpc::ServerContext *context, const service::HMIDataRequest *request, service::HMIDataResponse *response)
{
    // printLog_I ("ServiceManager received data from HMISrv: " + request->data ());
    auto hmi_adapter = HMIAdapter::getInstance ();
    if (hmi_adapter)
    {
        hmi_adapter->setDataReceiveHMISrv (request->data ());
        printLog_I ("ServiceManager received data from HMISrv: " +  hmi_adapter->getDataReceiveHMISrv());
        response->set_success (true);
    }
    else
    {
        printLog_E ("HMIAdapter instance is null.");
        response->set_success (false);
    }
    return Status::OK;
}
void HMIAdapter::setDataReceiveHMISrv(const std::string data)
{
    mDataReceiveHMISrv = data;
}
const std::string HMIAdapter::getDataReceiveHMISrv ()
{
    return mDataReceiveHMISrv;
}
void HMIAdapter::clearDataReceiveHMISrv ()
{
    // std::lock_guard<std::mutex> lock (mCommandMutex);
    mDataReceiveHMISrv.clear ();
}
void HMIAdapter::registerService ()
{
    // default port is 50051 to connect to port server
    std::string server_address = "localhost:50051";
    auto channel               = grpc::CreateChannel (server_address, grpc::InsecureChannelCredentials ());
    auto stub                  = service::ServiceManager::NewStub (channel);

    service::RegisterHMIRequest request;
    service::RegisterHMIResponse response;
    request.set_client_id (gRPC_Service::gRPC_HMI_SERVICE_NAME);

    grpc::ClientContext context;
    Status status = stub->registerHMIMgrService (&context, request, &response);

    if (status.ok () && response.success ())
    {
        printLog_I ("Registered successfully!!!");
    }
    else
    {
        printLog_I ("Failed to register!!!");
    }
}

void HMIAdapter::handleRevConmamndLCD ()
{
    std::string dataReceive = getDataReceiveHMISrv();
    printLog_I ("Status LED:%s", dataReceive.c_str ());
}


void HMIAdapter::onLCDScreen (const std::string &response)
{

}

HMIAdapter *HMIAdapter::getInstance ()
{
    static std::shared_ptr<HMIAdapter> instance;
    if (instance == nullptr)
    {
        instance = std::make_shared<HMIAdapter> ();
    }
    return instance.get ();
}