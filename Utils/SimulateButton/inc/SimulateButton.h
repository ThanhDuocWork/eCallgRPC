#ifndef SIMULATEBUTTON_H
#define SIMULATEBUTTON_H
#include<iostream>
#include<string>
#include"Logger.h"
#include<mutex>
#include<memory>
#include<thread>

class SimutaleButton
{
    private:
        std::string mGlobalKey;
        std::mutex mMutex;
        static std::shared_ptr<SimutaleButton> mInstance;
        bool running;
        std::thread mInputThread;
    public:
        SimutaleButton();
        ~SimutaleButton();
        static SimutaleButton *getInstance();
        void setGlobalKey(std::string key);
        std::string getGlobalKey();    
        void startInputThread();
        void stopInputThread();
};

#endif