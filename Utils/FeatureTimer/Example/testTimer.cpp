#include "../inc/FeatureTimer.h"
void setStateButton(uint32_t setTime) // check short press Button and long press button
{
    if (setTime <= 10)
    {
        std::cout << "Timer not expired!! detech long press button" << std::endl;
    }
    else if (setTime > 10)
    {
        std::cout << "Timer expired go to center Communicate" << std::endl;
    }
    std::cout << "----------------------------------" << std::endl;
}
void handleInput(Timer &actionTimer)// check receive re send data
{
    char userInput;
    while (true)
    {
        std::cin >> userInput;
        if (userInput == 's')
        {
            if (!actionTimer.isTimeout())
            {
                std::cout << "Send data" << std::endl;
            }
            else
            {
                std::cout << "App not handle" << std::endl;
                break;
            }
        }
    }
}
void checkActionTimer(Timer &actionTimer) {
    // int setTime;
    // std::cout << "Select time: ";
    // std::cin >> setTime;
    // actionTimer.setDuration(setTime);
    // actionTimer.startTimer();
    if (actionTimer.isTimerRun()) {
        std::cout << "Timer is currently running." << std::endl;
    }
    if (!actionTimer.isTimerRun()) {
        std::cout << "Timer has stopped." << std::endl;
    }
}
void myCallBack(eTimer idTimer)
{
    switch (idTimer)
    {
    case eTimer::cancelTimer :
        std::cout << "Cancel timer: Default value is 10s, can be configured between 0s to 180s." << std::endl;
        break;
    case eTimer::callbackTimer:
        std::cout << "Callback timer: Default value is 30s, can be configured between 0s to 180s" << std::endl;
    case eTimer::ActiveDataStateTimer:
        std::cout << "ActiveDataStateTimer: Default value is 60s, can be configured between 60s to 300s." << std::endl;
    default:
        break;
    }
}
void stopTimerEarly(Timer &actionTimer){
    actionTimer.setStateTimer(ActiveDataStateTimer);
    actionTimer.setDuration(30); 
    actionTimer.setCallBack([&actionTimer]() {
    actionTimer.setStateTimer(eTimer::cancelTimer);
    });
    std::cout << "Starting timer..." << std::endl;
    actionTimer.startTimer();
    checkActionTimer(actionTimer);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    std::cout << "Stopping timer early..." << std::endl;
    actionTimer.stopTimer();
    checkActionTimer(actionTimer);
    std::cout << "Timer process finished." << std::endl;
    std::cout << "restart Timer!!" << std::endl;
    actionTimer.restartTimer();
    checkActionTimer(actionTimer);
}
int main()
{
    Timer actionTimer;
    eTimer idTimer;
    // int setTime;
    // std::cout << "Select time: ";
    // std::cin >> setTime;
    stopTimerEarly(actionTimer);
    std::cout<<"nothing"<<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    std::cout<<"nothing2"<<std::endl;
    
    // while(1)
    // {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }
    return 0;
}
