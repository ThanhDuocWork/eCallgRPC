#ifndef MQTT_SRV_H
#define MQTT_SRV_H

#include "service.grpc.pb.h"
#include <grpc++/grpc++.h>
#include"Logger.h"
#include"MQTTFunc.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class MQTTSrvImpl final : public service::MQTTSrv::Service {
public:
    Status registerMQTTSrv(ServerContext* context, const service::MQTTServiceRequest* request, service::MQTTServiceResponse* response) override;
    Status receiveTriggerDataCAN(ServerContext* context, const service::MQTTDataRequest* request, service::MQTTDataResponse* response) override;
};

void RunMQTTSrv();

#endif // MQTT_SRV_H
