#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H

#include "service.grpc.pb.h"
#include <grpc++/grpc++.h>
#include "Parameter.h"
#include "Logger.h"
#include "HMIAdapter.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class ServiceManagerImpl final : public service::ServiceManager::Service {
public:
    Status registerHMIMgrService(ServerContext* context, const service::RegisterHMIRequest* request, service::RegisterHMIResponse* response) override;
    Status registerAudioMgrService(ServerContext* context, const service::RegisterAudioRequest* request, service::RegisterAudioResponse* response) override;
    Status registerMQTTMgrService(ServerContext* context, const service::RegisterMQTTRequest* request, service::RegisterMQTTResponse* response) override;
};

void RunServiceManager();

#endif // SERVICE_MANAGER_H
