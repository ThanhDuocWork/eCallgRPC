#include "FeatureTimer.h"
Timer::Timer(std::function<void()> callBack, uint32_t duration){
    mCallBack = callBack;
    mDuration = duration;
    timeRunning = false;
    timeout = true;
    mCount = 0;
}
void Timer::startTimer()
{
    if (!timeRunning)
    {
        timeRunning = true;
        timeout = false;
        printLog_I("Timer Start");
        mTimerThread = std::thread([this]()
                                   {
            while (timeRunning && mCount < mDuration) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                mCount++;
                //std::cout<<"Time at: "<<count<<"ms"<<std::endl;
                if (mCount >= mDuration) {
                    printLog_I("Timeout callback");
                    timeout=true;
                    if(mCallBack!=nullptr){
                        mCallBack();
                    }            
                }
            }
            timeRunning = false; });
        mTimerThread.detach();
    }
}
void Timer::stopTimer()
{
    mCount = 0;
    if (timeRunning)
    {
        timeRunning = false;
    }
    //    std::cout << "Timer stopped at " << count << " miliseconds." << std::endl;
}
void Timer::setDuration(uint32_t miliseconds)
{
    mDuration = miliseconds;
}
void Timer::restartTimer()
{
    startTimer();
}
bool Timer::isTimeout()
{
    return timeout;
}
bool Timer::isTimerRun()
{
    return timeRunning;
}
void Timer::setCallBack(std::function<void()> callbackFunction)
{
    mCallBack = callbackFunction;
}
void Timer::setStateTimer(eTimer idTimer)
{
    if (idTimer == eTimer::cancelTimer)
    {
        printLog_I("Cancel timer: Default value is 10s, can be configured between 0s to 180s.");
    }
    else if (idTimer == eTimer::callbackTimer)
    {
        printLog_I("Callback timer: Default value is 30s, can be configured between 0s to 180s.");
    }
    else if (idTimer == eTimer::ActiveDataStateTimer)
    {
        printLog_I("ActiveDataStateTimer: Default value is 60s, can be configured between 60s to 300s.");
    }
}
