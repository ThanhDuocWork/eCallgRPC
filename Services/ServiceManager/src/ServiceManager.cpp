#include "ServiceManager.h"
#include <iostream>

Status ServiceManagerImpl::registerHMIMgrService (ServerContext *context, const service::RegisterHMIRequest *request, service::RegisterHMIResponse *response)
{
    printLog_I("Received HMI registration request from: "+request->client_id ());

    if (request->client_id () != gRPC_Service::gRPC_HMI_SERVICE_NAME)
    {
        printLog_E("Invalid service registration request:"+request->client_id ());
        response->set_success (false);
        return Status::CANCELLED;
    }

    std::string hmi_server_address = "localhost:50052";
    auto hmi_channel               = grpc::CreateChannel (hmi_server_address, grpc::InsecureChannelCredentials ());
    auto hmi_stub                  = service::HMISrv::NewStub (hmi_channel);

    service::HMIServiceRequest hmi_request;
    service::HMIServiceResponse hmi_response;

    hmi_request.set_request_data ("Registering service: " + request->client_id ());

    grpc::ClientContext hmi_context;
    Status hmi_status = hmi_stub->registerHMISrv (&hmi_context, hmi_request, &hmi_response);

    if (hmi_status.ok ())
    {
        printLog_I("HMISrv responded:"+ hmi_response.response_data ());
        response->set_success (true);
    }
    else
    {
        printLog_E("Failed to communicate with HMISrv:"+hmi_status.error_message ());
        response->set_success (false);
    }

    return Status::OK;
}

Status ServiceManagerImpl::registerAudioMgrService (ServerContext *context, const service::RegisterAudioRequest *request, service::RegisterAudioResponse *response)
{
    printLog_I("Received Audio registration request from:"+request->client_id ());
    if (request->client_id () != gRPC_Service::gRPC_AUDIO_SERVICE_NAME)
    {
        printLog_E("Invalid service registration request: " + request->client_id ());
        response->set_success (false);
        return Status::CANCELLED;
    }

    std::string audio_server_address = "localhost:50053";
    auto audio_channel               = grpc::CreateChannel (audio_server_address, grpc::InsecureChannelCredentials ());
    auto audio_stub                  = service::AudioSrv::NewStub (audio_channel);

    service::AudioServiceRequest audio_request;
    service::AudioServiceResponse audio_response;

    audio_request.set_request_data ("Registering service: " + request->client_id ());

    grpc::ClientContext audio_context;
    Status audio_status = audio_stub->registerAudioSrv (&audio_context,audio_request, &audio_response);

    if (audio_status.ok ())
    {
        printLog_E("AudioSrv responded: " + audio_response.response_data ());
        response->set_success (true);
    }
    else
    {
        printLog_E("Failed to communicate with HMISrv:" + audio_status.error_message ());
        response->set_success (false);
    }

    return Status::OK;
}

Status ServiceManagerImpl::registerMQTTMgrService (ServerContext *context, const service::RegisterMQTTRequest *request, service::RegisterMQTTResponse *response)
{
    printLog_I("Received MQTT registration request from: " + request->client_id ());
    if (request->client_id () != gRPC_Service::gRPC_MQTT_SERVICE_NAME)
    {
        printLog_E("Invalid service registration request: " + request->client_id ());
        response->set_success (false);
        return Status::CANCELLED;
    }

    std::string mqtt_server_address = "localhost:50054";
    auto mqtt_channel               = grpc::CreateChannel (mqtt_server_address, grpc::InsecureChannelCredentials ());
    auto mqtt_stub                  = service::MQTTSrv::NewStub (mqtt_channel);

    service::MQTTServiceRequest mqtt_request;
    service::MQTTServiceResponse mqtt_response;

    mqtt_request.set_request_data ("Registering service: " + request->client_id ());

    grpc::ClientContext mqtt_context;
    Status mqtt_status = mqtt_stub->registerMQTTSrv (&mqtt_context,mqtt_request, &mqtt_response);

    if (mqtt_status.ok ())
    {
        printLog_I("MQTTSrv responded: " + mqtt_response.response_data ());
        response->set_success (true);
    }
    else
    {
        printLog_E("Failed to communicate with MQTTSrv: " + mqtt_status.error_message ());
        response->set_success (false);
    }

    return Status::OK;
}
void RunServiceManager ()
{
    std::string server_address ("0.0.0.0:50051");
    ServiceManagerImpl service_manager;

    ServerBuilder builder;
    builder.AddListeningPort (server_address, grpc::InsecureServerCredentials ());
    builder.RegisterService (&service_manager);

    std::unique_ptr<Server> server (builder.BuildAndStart ());
    printLog_I("ServiceManager listening on:"+ server_address);
    server->Wait ();
}

int main ()
{
    RunServiceManager ();
    return 0;
}