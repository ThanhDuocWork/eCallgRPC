#include "HMISrv.h"
#include "HMISrvFunc.h"
#include <thread>

void handle ()
{

    HMISrvFunc::getInstance ()->initSocket ();
    HMISrvFunc::getInstance ()->runServerSocketHMI ();
}
int main ()
{
    std::thread grpcThread (RunHMISrv);
    grpcThread.detach ();

    std::thread socketThread (handle);
    socketThread.detach ();
    // RunHMISrv();
    while (true)
    {
        std::this_thread::sleep_for (std::chrono::seconds (1));
    }

    return 0;
}
