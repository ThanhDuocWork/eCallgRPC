#include"IAdapter.h"
#include"HMIAdapter.h"
#include"AudioAdapter.h"
#include"MQTTAdapter.h"

#include "EventTable.h"
#include "IState.h"
#include "ManagerMessage.h"
#include "Logger.h"
#include "EventData.h"
int main()
{
    printLog_I("check data: %d",EventData::getInstanceEventData()->getMessageID());

    std::thread MQTTImplThread (RunMQTTAdapter);
    MQTTImplThread.detach ();

    std::thread HMIImplThread(RunHMIAdapter);
    HMIImplThread.detach();

    std::thread AudioImplThread(RunAudioAdapter);
    AudioImplThread.detach();

    HMIAdapter::getInstance()->registerService();
    AudioAdapter::getInstance()->registerService();
    MQTTAdapter::getInstance()->registerService();
    
    ManagerMessage::getInstance ()->startThreadMessage ();
    ManagerMessage::getInstance ()->registerEventHandler (EventTable::getInstance ()->getEventStateFunction ());
    IDLEState::getInstance ()->preEnterState ();

    SimutaleButton::getInstance ()->startInputThread ();
    
    
    while (true)
    {
        // printLog_I("while loop");
        
        std::this_thread::sleep_for (std::chrono::milliseconds (500));
    }
    
}