#include "SimulateButton.h"


std::shared_ptr<SimutaleButton> SimutaleButton::mInstance = nullptr;
SimutaleButton::SimutaleButton ()
{
}
SimutaleButton::~SimutaleButton ()
{
}
void SimutaleButton::setGlobalKey (std::string key)
{
    mGlobalKey = key;
}
std::string SimutaleButton::getGlobalKey ()
{
    return mGlobalKey;
}

SimutaleButton *SimutaleButton::getInstance ()
{
    if (mInstance == nullptr)
    {
        mInstance = std::make_shared<SimutaleButton> ();
    }
    return mInstance.get ();
}
void SimutaleButton::startInputThread ()
{
    running      = true;
    mInputThread = std::thread (
    [this] ()
    {
        while (running)
        {
            std::string key;
            if (std::getline (std::cin, key))
            {
                mGlobalKey = key;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });
}
void SimutaleButton::stopInputThread()
{
    running =false;
}