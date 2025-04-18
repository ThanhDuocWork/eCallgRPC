#include "CenterState.h"

std::shared_ptr<CenterState> CenterState::mInstance = nullptr;

CenterState::CenterState()
{
    

}
CenterState::~CenterState()
{

}
void CenterState::preEnterState()
{
    printLog_I("State at CenterState preEnterState");
    std::shared_ptr<EventCenter> eventEntCenter = std::make_shared<EventCenter>(static_cast<uint32_t>(MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_Center_Enter));
    eventEntCenter->setData("Enter_CenterState_State");
    ManagerMessage::getInstance()->addEventData(eventEntCenter);
}
void CenterState::handleAction()
{
    printLog_I("State at CenterState handleAction");
    std::shared_ptr<EventCenter> eventActCenter = std::make_shared<EventCenter>(static_cast<uint32_t>(MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_Center_Action));
    eventActCenter->setData("HandleAction_CenterState_State");
    // std::string key = HMIAdapter::getInstance ()->getSocketCommand ();
    // printLog_I("key:%s",key.c_str());
    // MQTTAdapter::getInstance()->PubMQTT(key);

    MQTTAdapterImpl::getInstance()->addDataToQueue(MESSAGE_MQTT::MESSSAGE_GPS);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ManagerMessage::getInstance()->addEventData(eventActCenter);
}
void CenterState::preReleaseState()
{
    printLog_I("State at CenterState preReleaseState");
    std::shared_ptr<EventCenter> eventRelCenter = std::make_shared<EventCenter>(static_cast<uint32_t>(MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_Center_Release));    
    eventRelCenter->setData("PreReleaseState_CenterState_State");
    ManagerMessage::getInstance()->addEventData(eventRelCenter);
}
CenterState *CenterState::getInstance()
{
    if(mInstance==nullptr)
    {
        mInstance = std::make_shared<CenterState>();
    }
    return mInstance.get ();
}

void CenterState::messageTriggerMQTTData(std::string data)
{

}