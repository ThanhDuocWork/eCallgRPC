#include "AudioSrv.h"
#include <iostream>

Status AudioSrvImpl::registerAudioSrv (ServerContext *context, const service::AudioServiceRequest *request, service::AudioServiceResponse *response)
{
    printLog_I ("AudioSrv received request: " + request->request_data ());
    response->set_response_data ("Audio Service successfully registered.");
    return Status::OK;
}
Status AudioSrvImpl::receivePlayAudioSrv (ServerContext *context, const service::AudioDataRequest *request, service::AudioDataResponse *response)
{
    auto audio_check = AudioFunc::getInstance();
    audio_check->handlePlayAudioFunc(&request->data ());
    response->set_success (true);
    return Status::OK;
}
void RunAudioSrv ()
{
    std::string server_address ("0.0.0.0:50053");
    AudioSrvImpl audio_service;

    ServerBuilder builder;
    builder.AddListeningPort (server_address, grpc::InsecureServerCredentials ());
    builder.RegisterService (&audio_service);

    std::unique_ptr<Server> server (builder.BuildAndStart ());
    printLog_I ("AudioSrv listening on:%s", server_address.c_str ());
    server->Wait ();
}
