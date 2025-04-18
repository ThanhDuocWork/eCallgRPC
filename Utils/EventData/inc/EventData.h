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
        EventData(uint32_t messgaID);
        virtual ~EventData();
        int getMessageID();
        static EventData *getInstanceEventData();
};
#endif