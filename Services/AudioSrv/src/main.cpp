#include"AudioFunc.h"
#include"AudioSrv.h"

int main()
{
    std::thread gRPCServerAudioSrv(RunAudioSrv);
    gRPCServerAudioSrv.detach();
    while (true)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds (500));
    } 
}