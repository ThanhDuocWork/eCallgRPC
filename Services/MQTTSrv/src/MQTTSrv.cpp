#include"MQTTSrv.h"
#include <iostream>

Status MQTTSrvImpl::registerMQTTSrv(ServerContext* context, const service::MQTTServiceRequest* request, service::MQTTServiceResponse* response) {
    printLog_I("MQTTSrv received request: "+request->request_data());
    response->set_response_data("MQTT Service successfully registered.");
    return Status::OK;
}
Status MQTTSrvImpl::receiveTriggerDataCAN (grpc::ServerContext *context, const service::MQTTDataRequest *request, service::MQTTDataResponse *response)
{
    auto MQTTT_Func = MQTTFunc::getInstance ();
    if (MQTTT_Func)
    {
        MQTTT_Func->PubMQTT (request->data ());
        // printLog_I ("Data trigger: " +  MQTTT_Func->getDataCANFromAdapter());
        response->set_success (true);
    }
    else
    {
        printLog_E ("MQTTFunc instance is null.");
        response->set_success (false);
    }
    return Status::OK;
}
void RunMQTTSrv() {
    std::string server_address("0.0.0.0:50054");
    MQTTSrvImpl mqtt_service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&mqtt_service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    printLog_I("MQTTSrv listening on:%s",server_address.c_str());
    server->Wait();
}

