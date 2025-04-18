#include "HMISrv.h"
#include <chrono>
#include <iostream>

std::shared_ptr<HMISrvImpl> HMISrvImpl::mInstance = nullptr;
HMISrvImpl::HMISrvImpl ()
{
    std::string service_manager_address = "localhost:50051";
    auto channel_Service                = grpc::CreateChannel (service_manager_address, grpc::InsecureChannelCredentials ());
    stub_Service                        = service::ServiceManager::NewStub (channel_Service);

    std::string hmi_address = "localhost:50055";
    auto channel_HMI        = grpc::CreateChannel (hmi_address, grpc::InsecureChannelCredentials ());
    stub_HMIAdapter         = service::HMIAdapter::NewStub (channel_HMI);
}


Status HMISrvImpl::registerHMISrv (grpc::ServerContext *context, const service::HMIServiceRequest *request, service::HMIServiceResponse *response)
{
    printLog_I ("HMISrv received request: " + request->request_data ());
    response->set_response_data ("HMI Service successfully registered.");
    return grpc::Status::OK;
}


Status HMISrvImpl::requestButtonHMISrv (grpc::ServerContext *context, const service::HMIDataRequest *request, service::HMIDataResponse *response)
{
    service::HMIDataRequest sm_request;
    service::HMIDataResponse sm_response;

    // const std::string dataButton = HMISrvFunc::getInstance()->getSocketCommand();
    sm_request.set_data (request->data ());

    grpc::ClientContext sm_context;
    grpc::Status status = stub_HMIAdapter->requestButtonHMIAdapter (&sm_context, sm_request, &sm_response);

    if (status.ok () && sm_response.success ())
    {
        printLog_I ("Data successfully sent to HMIAdapter.");
        response->set_success (true);
    }
    else
    {
        printLog_E ("Failed to send data to HMIAdapter: " + status.error_message ());
        response->set_success (false);
    }

    return grpc::Status::OK;
}

void HMISrvImpl::sendDataToHMIAdapter (const std::string &data)
{
    // printLog_I ("SendDataToHMIAdapter() triggered with data: " + data);
    service::HMIDataRequest request;
    service::HMIDataResponse response;
    // const std::string dataButton = HMISrvFunc::getInstance ()->getSocketCommand ();
    // data = dataButton;
    // printLog_I ("check data loop:%s", data.c_str ());
    request.set_data (data);

    grpc::ClientContext context;
    grpc::Status status = stub_HMIAdapter->requestButtonHMIAdapter (&context, request, &response);
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
        printLog_I ("Data successfully sent to HMIAdapter.");
    }
    else
    {
        printLog_E ("Failed to send data to HMIAdapter: " + status.error_message ());
    }

    std::this_thread::sleep_for (std::chrono::seconds (5));
}
std::shared_ptr<HMISrvImpl> HMISrvImpl::getInstance ()
{
    if (mInstance == nullptr)
    {
        mInstance = std::make_shared<HMISrvImpl> ();
    }
    return mInstance;
}
void RunHMISrv ()
{
    std::string server_address ("0.0.0.0:50052");

    auto hmi_service = HMISrvImpl::getInstance ();

    grpc::ServerBuilder builder;
    builder.AddListeningPort (server_address, grpc::InsecureServerCredentials ());
    builder.RegisterService (hmi_service.get ());

    std::unique_ptr<grpc::Server> server (builder.BuildAndStart ());
    printLog_I ("HMISrv listening on:%s", server_address.c_str ());

    // std::thread senderThread (&HMISrvImpl::sendDataLoop, hmi_service);
    // senderThread.detach ();
    HMISrvFunc::getInstance ()->registerCallback (
    [hmi_service] (const std::string &data)
    {
        if (!hmi_service)
        {
            printLog_E ("hmi_service is nullptr! Cannot send data.");
            return;
        }
        printLog_I ("Sending data via callback: " + data);
        hmi_service->sendDataToHMIAdapter (data);
    });

    server->Wait ();
}
