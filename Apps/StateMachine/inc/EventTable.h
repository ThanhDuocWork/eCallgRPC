#ifndef EVENTTABLE_H
#define EVENTTABLE_H
#include "AudioAdapter.h"
#include "CancelTimerState.h"
#include "CenterState.h"
#include "EventData.h"
#include "IDLEState.h"
#include <functional>
#include <iostream>
#include <map>
#include"Parameter.h"
#include"MQTTAdapter.h"


class EventTable
{
    private:
    std::map<MESSAGE_EVENT::ENUM_ESTATE_MESSAGE, std::function<void (std::shared_ptr<EventData>)>> stateHandlers;

    // std::map<MESSAGE_EVENT::ENUM_MESSAGE_RESPONSE, std::function<void (std::shared_ptr<EventData>)>> messageMap;
    static std::unique_ptr<EventTable> mInstatnceTableEvent;

    public:
    EventTable ();
    void initializeHandlers ();
    void eventStateFunction (std::shared_ptr<EventData> event);
    static EventTable *getInstance ();
    std::function<void (std::shared_ptr<EventData>)> getEventStateFunction ();

    void handleEnterIDLE (std::shared_ptr<EventData> eventData);
    void handleActionIDLE (std::shared_ptr<EventData> eventData);
    void handleReleaseIDLE (std::shared_ptr<EventData> eventData);
    void handleEnterCancelTimer (std::shared_ptr<EventData> eventData);
    void handleActionCancelTimer (std::shared_ptr<EventData> eventData);
    void handleReleaseCancelTimer (std::shared_ptr<EventData> eventData);
    void handleEnterCenter (std::shared_ptr<EventData> eventData);
    void handleActionCenter (std::shared_ptr<EventData> eventData);
    void handleReleaseCenter (std::shared_ptr<EventData> eventData);

    void handleResponseMQTT (std::shared_ptr<EventData> eventData);
};

#endif