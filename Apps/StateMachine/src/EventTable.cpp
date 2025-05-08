#include "EventTable.h"

std::shared_ptr<EventTable> EventTable::mInstatnceTableEvent = nullptr;
EventTable::EventTable ()
{
    initializeHandlers ();
}

void EventTable::initializeHandlers ()
{
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_IDLE_Enter]   = [this] (std::shared_ptr<EventData> eventData) { handleEnterIDLE (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_IDLE_Action]  = [this] (std::shared_ptr<EventData> eventData) { handleActionIDLE (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_IDLE_Release] = [this] (std::shared_ptr<EventData> eventData) { handleReleaseIDLE (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_CancelTimer_Enter] = [this] (std::shared_ptr<EventData> eventData) { handleEnterCancelTimer (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_CancelTimer_Action] = [this] (std::shared_ptr<EventData> eventData) { handleActionCancelTimer (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_CancelTimer_Release] = [this] (std::shared_ptr<EventData> eventData) { handleReleaseCancelTimer (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_Center_Enter] = [this] (std::shared_ptr<EventData> eventData) { handleEnterCenter (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_Center_Action]  = [this] (std::shared_ptr<EventData> eventData) { handleActionCenter (eventData); };
    stateHandlers[MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_Center_Release] = [this] (std::shared_ptr<EventData> eventData) { handleReleaseCenter (eventData); };

    // messageMap[MESSAGE_EVENT::ENUM_MESSAGE_RESPONSE::EVENT_MESSAGE_MQTT_SUB] = [this] (std::shared_ptr<EventData> eventData) { handleResponseMQTT (eventData); };
}

void EventTable::eventStateFunction (std::shared_ptr<EventData> event)
{
    if (event != nullptr)
    {
        MESSAGE_EVENT::ENUM_ESTATE_MESSAGE messageID = static_cast<MESSAGE_EVENT::ENUM_ESTATE_MESSAGE> (event->getMessageID ());
        if (stateHandlers.find (messageID) != stateHandlers.end ())
        {
            stateHandlers[messageID](event);
            printLog_I("MessageID check state: %d",messageID);
        }
    }
}
std::function<void (std::shared_ptr<EventData>)> EventTable::getEventStateFunction ()
{
    return [this] (std::shared_ptr<EventData> event) { this->eventStateFunction (event); };
}
EventTable *EventTable::getInstance ()
{
    if (mInstatnceTableEvent == nullptr)
    {
        mInstatnceTableEvent = std::make_shared<EventTable> ();
    }
    return mInstatnceTableEvent.get ();
}
void EventTable::handleEnterIDLE (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventIDLE> IDLEEvent = std::static_pointer_cast<EventIDLE> (eventData);
    printLog_I ("case receive data Enter IDLE");
    printLog_I ("IDLE data: %s", IDLEEvent->getData ().c_str ());
    IDLEState::getInstance ()->handleAction ();
}

void EventTable::handleActionIDLE (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventIDLE> IDLEEvent = std::static_pointer_cast<EventIDLE> (eventData);
    printLog_I ("case receive data Action IDLE");
    printLog_I ("IDLE data: %s", IDLEEvent->getData ().c_str ());
    IDLEState::getInstance ()->preReleaseState ();
}

void EventTable::handleReleaseIDLE (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventIDLE> IDLEEvent = std::static_pointer_cast<EventIDLE> (eventData);
    printLog_I ("case receive data Release IDLE");
    printLog_I ("IDLE data: %s", IDLEEvent->getData ().c_str ());
    CancelTimerState::getInstance ()->preEnterState ();
}

void EventTable::handleEnterCancelTimer (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventCancelTimer> CancelTimerEvent = std::static_pointer_cast<EventCancelTimer> (eventData);
    printLog_I ("case receive data Enter CancelTimer");
    printLog_I ("CancelTimer data: %s", CancelTimerEvent->getData ().c_str ());
    CancelTimerState::getInstance ()->handleAction ();
}

void EventTable::handleActionCancelTimer (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventCancelTimer> CancelTimerEvent = std::static_pointer_cast<EventCancelTimer> (eventData);
    printLog_I ("case receive data Action CancelTimer");
    printLog_I ("CancelTimer data: %s", CancelTimerEvent->getData ().c_str ());
    if (CancelTimerEvent->getData () == "HandleAction_CancelTimer_State_ShortPress")
    {
        CancelTimerState::getInstance ()->preReleaseState ();
    }
    else if (CancelTimerEvent->getData () == "HandleAction_CancelTimer_State_LongPress")
    {
        IDLEState::getInstance ()->preEnterState ();
    }
}
void EventTable::handleReleaseCancelTimer (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventCancelTimer> CancelTimerEvent = std::static_pointer_cast<EventCancelTimer> (eventData);
    printLog_I ("case receive data Release CancelTimer");
    printLog_I ("CancelTimer data: %s", CancelTimerEvent->getData ().c_str ());
    CenterState::getInstance ()->preEnterState ();
}
void EventTable::handleEnterCenter (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventCenter> CenterEvent = std::static_pointer_cast<EventCenter> (eventData);
    printLog_I ("case receive data Enter CenterState");
    printLog_I ("CenterState data: %s", CenterEvent->getData ().c_str ());
    CenterState::getInstance ()->handleAction ();
}
void EventTable::handleActionCenter (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventCenter> CenterEvent = std::static_pointer_cast<EventCenter> (eventData);
    printLog_I ("case receive data Action CenterState");
    printLog_I ("CenterState data: %s", CenterEvent->getData ().c_str ());
    CenterState::getInstance ()->preReleaseState ();
}

void EventTable::handleReleaseCenter (std::shared_ptr<EventData> eventData)
{
    std::shared_ptr<EventCenter> CenterEvent = std::static_pointer_cast<EventCenter> (eventData);
    printLog_I ("case receive data Release CenterState");
    printLog_I ("CenterState data: %s", CenterEvent->getData ().c_str ());
    // CenterState::getInstance ()->preEnterState ();
}

// void EventTable::handleResponseMQTT(std::shared_ptr<EventData> eventData)
// {
//     std::shared_ptr<EventMQTT> MQTTEvent = std::static_pointer_cast<EventMQTT> (eventData);
//     printLog_I ("case receive data MQTT Sub");
//     printLog_I ("MQTT Sub data: %s", MQTTEvent->getData ().c_str ());    
// }