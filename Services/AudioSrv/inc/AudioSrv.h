#ifndef AUDIO_SRV_H
#define AUDIO_SRV_H

#include "service.grpc.pb.h"
#include <grpc++/grpc++.h>
#include"Logger.h"
#include"AudioFunc.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class AudioSrvImpl final : public service::AudioSrv::Service {
public:
    Status registerAudioSrv(ServerContext* context, const service::AudioServiceRequest* request, service::AudioServiceResponse* response) override;
    Status receivePlayAudioSrv(ServerContext* context, const service::AudioDataRequest* request, service::AudioDataResponse* response) override;
private:
    std::string mdatacheck;
};

void RunAudioSrv();

#endif // AUDIO_SRV_H
