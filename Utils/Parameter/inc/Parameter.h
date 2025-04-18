#ifndef PARAMETER_H
#define PARAMETER_H
#include <iostream>
namespace MESSAGE_EVENT
{
    enum class ENUM_ESTATE_MESSAGE : uint32_t
    {
        eState_IDLE_Enter = 0,
        eState_IDLE_Action,
        eState_IDLE_Release,
        eState_CancelTimer_Enter,
        eState_CancelTimer_Action,
        eState_CancelTimer_Release,
        eState_Center_Enter,
        eState_Center_Action,
        eState_Center_Release
    };

}
namespace gRPC_Service
{
    constexpr const char* gRPC_HMI_SERVICE_NAME = "HMIMgrSrv";
    constexpr const char* gRPC_AUDIO_SERVICE_NAME = "AUDIOMgrSrv";
    constexpr const char* gRPC_MQTT_SERVICE_NAME = "MQTTMgrSrv";
} // namespace gRPC_Service

namespace MESSAGE_MQTT
{
    constexpr const char* MESSSAGE_GPS = "Trigger GPS"; 
}
#endif 