#include "IDLEState.h"

std::unique_ptr<IDLEState> IDLEState::mInstanceIDLE = nullptr;

IDLEState::IDLEState () : mFlag (false)
{
}

IDLEState::~IDLEState ()
{
}

IDLEState *IDLEState::getInstance ()
{
    if (mInstanceIDLE == nullptr)
    {
        mInstanceIDLE = std::make_unique<IDLEState> ();
    }
    return mInstanceIDLE.get ();
}

void IDLEState::preEnterState ()
{
    printLog_I ("State at IDLEState preEnterState!!");
    std::shared_ptr<EventIDLE> eventEnterIDLE = std::make_shared<EventIDLE> (static_cast<uint32_t> (MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_IDLE_Enter));
    printLog_I ("Wait for user press SOS Button!!");
    eventEnterIDLE->setData ("Enter_IDLE_State");
    ManagerMessage::getInstance ()->addEventData (eventEnterIDLE);
}

void IDLEState::handleAction ()
{
    printLog_I ("State at IDLEState handleAction!!");
    std::shared_ptr<EventIDLE> eventHandleIDLE = std::make_shared<EventIDLE> (static_cast<uint32_t> (MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_IDLE_Action));
    eventHandleIDLE->setData ("Handle_IDLE_Action");
    if (!mFlag)
    {
        mThreadEventIDLE = std::thread ([this,eventHandleIDLE]()
        {
            while(!mFlag)
            {
            std::string key = HMIAdapter::getInstance()->getDataReceiveHMISrv();
            // printLog_I("value Key: %s",key.c_str());
                if(key == "ON")
                {
                    printLog_I ("------------------------");
                    printLog_I ("User press button SOS!!!");
                    std::string pathFile = "/home/duoctdt/eCallver2/Apps/piano.wav";
                    AudioAdapter::getInstance()->playAudioAdapter(pathFile);  
                    printLog_I ("Detect short Press!! -> Play (Audio piano.wav) -> turn ON led SOS!!!: %s",pathFile.c_str());
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    printLog_I ("------------------------");
                    mFlag = true;
                    mCondition.notify_all ();
                    break;
                }
                std::this_thread::sleep_for (std::chrono::milliseconds (500));
            }
            eventHandleIDLE->setData ("HandleAction_IDLE_State");
            ManagerMessage::getInstance ()->addEventData (eventHandleIDLE);
        });mThreadEventIDLE.detach ();
    }
    std::unique_lock<std::mutex> lock (mMutex);
    mCondition.wait (lock, [this] { return mFlag; });
}
void IDLEState::preReleaseState ()
{    
    printLog_I ("State at IDLEState preReleaseState!!");
    mFlag                                = false;
    std::shared_ptr<EventIDLE> eventReleaseIDLE = std::make_shared<EventIDLE> (static_cast<uint32_t> (MESSAGE_EVENT::ENUM_ESTATE_MESSAGE::eState_IDLE_Release));
    eventReleaseIDLE->setData ("Release_IDLE_State");
    ManagerMessage::getInstance ()->addEventData (eventReleaseIDLE);
}
