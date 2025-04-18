#ifndef IADAPTER_H
#define IADAPTER_H

#include "service.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <string>
#include <memory>
#include "Parameter.h"
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using service::RegisterHMIRequest;
using service::RegisterHMIResponse;

using service::RegisterAudioRequest;
using service::RegisterAudioResponse;

using service::RegisterMQTTRequest;
using service::RegisterMQTTResponse;

class IAdapter {
public:
    virtual ~IAdapter() {}
    virtual void registerService() = 0;
};

#endif  
