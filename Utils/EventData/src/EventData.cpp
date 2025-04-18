#include"EventData.h"
std::shared_ptr<EventData> EventData::mInstanceEventData = nullptr;
EventData::EventData(uint32_t messgaID): mMessageID(messgaID)
{
    
}
EventData::~EventData()
{

}
int EventData::getMessageID()
{
    return mMessageID;
}
EventData *EventData::getInstanceEventData()
{
    if(mInstanceEventData==nullptr)
    {
        mInstanceEventData = std::make_shared<EventData>();
    }
    return mInstanceEventData.get();
}