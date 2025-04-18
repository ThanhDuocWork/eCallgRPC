#ifndef CANCELTIMERSTATE_H
#define CANCELTIMERSTATE_H
#include "IState.h"
#include"FeatureTimer.h" 
#include "EventTable.h"
#include "AudioAdapter.h"
class CancelTimerState : public IState
{
    private:
    static std::unique_ptr<CancelTimerState> mInstance;
    std::shared_ptr<Timer> mActionTimer;
    bool mFlag;
    std::thread mCheckButton;
    std::mutex mMutex;
    std::condition_variable mCondition;
    public:
    CancelTimerState();
    ~CancelTimerState();
    static CancelTimerState *getInstance();
    void handleAction () override;
    void preEnterState () override;
    void preReleaseState () override;
    void initCancelTimer();
};
class EventCancelTimer : public EventData
{
    private:
    std::string mDataStringEvent;

    public:
    EventCancelTimer (uint32_t messageID) : EventData (messageID)
    {
    }
    void setData(const std::string data)
    {
        mDataStringEvent = data;
    }
    std::string getData()
    {
        return mDataStringEvent;
    }
};
#endif