#ifndef HMI_SRV_H
#define HMI_SRV_H

#include "service.grpc.pb.h"
#include <grpc++/grpc++.h>
#include<thread>
#include "Logger.h"
#include <iostream>
#include"HMISrvFunc.h"
#include <memory>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class HMISrvImpl final : public service::HMISrv::Service {
public:
    HMISrvImpl();
    Status registerHMISrv(ServerContext* context, const service::HMIServiceRequest* request, service::HMIServiceResponse* response) override;
    Status requestButtonHMISrv(ServerContext* context, const service::HMIDataRequest* request, service::HMIDataResponse* response) override;
    void sendDataToHMIAdapter(const std::string &data);
    static std::shared_ptr<HMISrvImpl> getInstance();
private:
    std::unique_ptr<service::HMIAdapter::Stub> stub_HMIAdapter; 
    std::unique_ptr<service::ServiceManager::Stub> stub_Service; 

    static std::shared_ptr<HMISrvImpl> mInstance;
};

void RunHMISrv();

#endif // HMI_SRV_H
