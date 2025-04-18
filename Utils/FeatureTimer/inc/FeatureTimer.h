#ifndef FEATURETIMER_H
#define FEATURETIMER_H
#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include "Logger.h"
enum eTimer : uint32_t
{
    cancelTimer,
    callbackTimer,
    ActiveDataStateTimer
};
class Timer
{
private:
    std::thread mTimerThread;
    std::function<void()> mCallBack;
    uint32_t mDuration;
    bool timeRunning;
    bool timeout;
    uint32_t mCount;
    eTimer idTimer;

public:
    Timer() = default;
    Timer(std::function<void()> callBack, uint32_t duration);
    ~Timer() = default;
    void startTimer();
    void stopTimer();
    void restartTimer();
    bool isTimeout();
    bool isTimerRun();
    void setDuration(uint32_t miliseconds);
    void setStateTimer(eTimer idTimer);
    void setCallBack(std::function<void()> callbackFunction);
};
#endif