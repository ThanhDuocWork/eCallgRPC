#include "CancelTimerState.h"


std::unique_ptr<CancelTimerState> CancelTimerState::mInstance = nullptr;
CancelTimerState::CancelTimerState () : mFlag (false)
{
    initCancelTimer ();
}
CancelTimerState::~CancelTimerState ()
{
}
void CancelTimerState::initCancelTimer ()
{
    if (mActionTimer == nullptr)
    {
        mActionTimer = std::make_shared<Timer> ();
    }
}
void CancelTimerState::preEnterState ()
{
    printLog_I ("State at CancleTimerState preEnterState!!");
    std::shared_ptr<EventCancelTimer> eventTimer =
    std::make_shared<EventCancelTimer> (static_cast<uint32_t> (MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_CancelTimer_Enter));
    eventTimer->setData ("Enter_CancelTimer_State");
    ManagerMessage::getInstance ()->addEventData (eventTimer);
}
void CancelTimerState::handleAction ()
{
    printLog_I ("State at CancleTimerState handleAction!!");
    std::shared_ptr<EventCancelTimer> eventTimerShortPress =
    std::make_shared<EventCancelTimer> (static_cast<uint32_t> (MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_Center_Enter));
    std::shared_ptr<EventCancelTimer> eventTimerLongPress =
    std::make_shared<EventCancelTimer> (static_cast<uint32_t> (MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_IDLE_Enter));
    if (!mFlag)
    {
        mActionTimer->setDuration (10000);
        mActionTimer->startTimer ();
        mActionTimer->setCallBack (
        [this, eventTimerShortPress] ()
        {
            mActionTimer->setStateTimer (eTimer::cancelTimer);
            eventTimerShortPress->setData ("HandleAction_CancelTimer_State_ShortPress");
            ManagerMessage::getInstance ()->addEventData (eventTimerShortPress);
        });
        mCheckButton = std::thread (
        [this, eventTimerLongPress] ()
        {
            std::unique_lock<std::mutex> lock (mMutex);
            {
                while (!mFlag)
                {
                    std::string key = HMIAdapter::getInstance ()->getDataReceiveHMISrv ();
                    if (key == "OFF")
                    {

                        HMIAdapter::getInstance ()->clearDataReceiveHMISrv ();
                        // Logic Stop Audio 1
                        mActionTimer->stopTimer ();
                        //***bug audio run than more 3s
                        // std::lock_guard<std::mutex> audioLock (AudioAdapter::getInstance ()->getMutex ());
                        std::string filePath = "/home/duoctdt/eCallver2/Apps/guitar.wav";
                        AudioAdapter::getInstance()->playAudioAdapter(filePath);
                        printLog_I ("------------------------");
                        printLog_I ("Detect long Press!! -> Play (Audio 2) -> turn off led SOS");
                        printLog_I ("------------------------");
                        std::this_thread::sleep_for (std::chrono::seconds (3));
                        mCondition.notify_all ();
                        eventTimerLongPress->setData ("HandleAction_CancelTimer_State_LongPress");
                        // std::this_thread::sleep_for (std::chrono::milliseconds (10000));
                        ManagerMessage::getInstance ()->addEventData (eventTimerLongPress);
                        mFlag = true;
                        break;
                    }
                    std::this_thread::sleep_for (std::chrono::milliseconds (500));
                }
            }
        });
        mCheckButton.detach ();
        std::unique_lock<std::mutex> lock (mMutex);
        mCondition.wait (lock,
        [this]
        {
            mFlag = false;
            return mFlag;
        });
    }
}
void CancelTimerState::preReleaseState ()
{
    printLog_I ("State at CancleTimerState preReleaseState!!");
    mFlag = false;
    std::shared_ptr<EventCancelTimer> eventTimer =
    std::make_shared<EventCancelTimer> (static_cast<uint32_t> (MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_CancelTimer_Release));
    eventTimer->setData ("Release_CancelTimer_State");
    ManagerMessage::getInstance ()->addEventData (eventTimer);
}

CancelTimerState *CancelTimerState::getInstance ()
{
    if (mInstance == nullptr)
    {
        mInstance = std::make_unique <CancelTimerState> ();
    }
    return mInstance.get ();
}