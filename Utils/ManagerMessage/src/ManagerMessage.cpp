#include "ManagerMessage.h"

std::shared_ptr<ManagerMessage> ManagerMessage::mInstanceMessage = nullptr;
ManagerMessage::ManagerMessage (int maxChillThread) : mMaxChillThread (maxChillThread)
{
    mMaxChillThread = maxChillThread;
    mStop           = false;
}
ManagerMessage::~ManagerMessage ()
{
    stopThreadMessage ();
    printLog_I ("stopThreadMessage");
}
void ManagerMessage::startThreadMessage ()
{
    printLog_I("startThreadMessage!!!");
    for (int i = 0; i < mMaxChillThread; i++)
    {
        mChillThread.emplace_back ([this] { threadWorker (); });
    }
}
void ManagerMessage::stopThreadMessage ()
{
    {
        std::lock_guard<std::mutex> lock (mMutex);
        mStop = true;
    }
    for (std::thread &thread : mChillThread)
    {
        if (thread.joinable ())
        {
            thread.join ();
        }
    }
}
void ManagerMessage::addEventData (std::shared_ptr<EventData> eventData)
{
    if (eventData == nullptr)
    {
        printLog_I ("eventData nullptr!!");
        return;
    }
    std::lock_guard<std::mutex> lock(mMutex);
    {
        std::shared_ptr<EventData> evenList = std::move(eventData);
        mEventDataList.emplace(evenList);
    }
}
void ManagerMessage::threadWorker ()
{
    while (!mStop)
    {
        std::shared_ptr<EventData> eventDataWorker;
        {
            std::lock_guard<std::mutex> lockwork (mMutex);
            if (mStop && mEventDataList.empty ())
            {
                printLog_I ("mEventDataList empty!!");
                printLog_I ("mStop is:%s", mStop ? "true" : "false");
            }
            if (!mEventDataList.empty ())
            {
                eventDataWorker = mEventDataList.front ();
                mEventDataList.pop ();
            }
        }
        if (eventDataWorker.get () != nullptr)
        {
            std::this_thread::sleep_for (std::chrono::milliseconds (500));
            mFuncEventHandler (eventDataWorker);
        }
        // else
        // {
        //     printLog_I ("eventDataWorker empty");
        // }
    }
}
void ManagerMessage::registerEventHandler (EventHandler eventHandler)
{
    mFuncEventHandler = eventHandler;
}
ManagerMessage *ManagerMessage::getInstance ()
{
    if (mInstanceMessage == nullptr)
    {
        mInstanceMessage = std::make_shared<ManagerMessage> (10);
    }
    return mInstanceMessage.get();

}