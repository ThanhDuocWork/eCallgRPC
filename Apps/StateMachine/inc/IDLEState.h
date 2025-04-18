#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "HMIAdapter.h"
#include "IState.h"
#include"HMIAdapter.h"
#include"AudioAdapter.h"
class IDLEState : public IState
{
    private:
    static std::unique_ptr <IDLEState> mInstanceIDLE;
    bool mFlag;
    std::thread mThreadEventIDLE;
    std::mutex mMutex;
    std::condition_variable mCondition;

    public:
    IDLEState();
    ~IDLEState();
    void handleAction () override;
    void preEnterState () override;
    void preReleaseState () override;    
    static IDLEState *getInstance();
};
class EventIDLE : public EventData
{
    private:
    std::string mDataStringEvent;

    public:
    EventIDLE (uint32_t messageID)
    : EventData (messageID) 
    {

      };
    ~EventIDLE()
    {

    };
    void setData(const std::string data)
    {
        mDataStringEvent = data;
    }
    const std::string getData()
    {
        return mDataStringEvent;
    }
};
#endif