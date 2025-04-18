#ifndef MANAGERMESSAGE_H
#define MANAGERMESSAGE_H
#include<iostream>
#include<functional>
#include"Logger.h"
#include"EventData.h"
#include<thread>
#include<mutex>
#include<memory>
#include<vector>
#include<list>
#include<queue>
using EventHandler = std::function<void(std::shared_ptr<EventData>)>;
class ManagerMessage
{
    private:
        int mMaxChillThread;
        bool mStop;
        std::vector<std::thread> mChillThread;
        std::queue<std::shared_ptr<EventData>> mEventDataList;
        std::mutex mMutex;
        EventHandler mFuncEventHandler;
        static std::shared_ptr<ManagerMessage> mInstanceMessage;
    public:
        ManagerMessage() = default;
        ManagerMessage(int maxThread);
        ~ManagerMessage();
        void startThreadMessage();
        void stopThreadMessage();
        void addEventData(std::shared_ptr<EventData> eventData);
        void threadWorker();
        void registerEventHandler(EventHandler eventHandler);
        static ManagerMessage *getInstance();
};
#endif