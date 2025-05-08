#ifndef EVENTDATA_H
#define EVENTDATA_H
#include<iostream>
#include<memory>
#include"Logger.h"

class EventData
{
    private:
        static std::shared_ptr<EventData> mInstanceEventData;
    protected:    
        int mMessageID;
    public:
        EventData() = default;
        EventData(uint32_t messageID);
        // EventData(uint32_t messageID):mMessageID(messageID)
        // {

        // }
        virtual ~EventData();
        int getMessageID() const;
        static EventData *getInstanceEventData();
};
#endif