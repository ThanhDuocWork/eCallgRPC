#ifndef AUDIO_FUNC_H
#define AUDIO_FUNC_H
#include <iostream>
#include "Logger.h"
#include <alsa/asoundlib.h>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
enum class STATUS_AUDIO : uint16_t
{
    OK = 0,
    OK_PLAY,
    OK_STOP,
    ERROR_INIT,
    ERROR_OPEN_DEVICE,
    ERROR_OPEN_FILE,
    ERROR_READ_FILE,
    ERROR_PLAY,  
    ERROR_STOP
};
class AudioFunc
{
    private:
    snd_pcm_t *pcmHandle;
    snd_pcm_hw_params_t *params;
    std::string filename;
    FILE *fileRB;
    unsigned int sampleRate;
    int numChannels;
    int bitsPerSample;
    const int bufferSize = 4096;
    unsigned char buffer[4096];

    bool flagStop;
    bool flagPause;
    bool isPlaying;

    std::thread playThread;
    std::condition_variable cv;
    std::mutex mtx;
    static std::shared_ptr<AudioFunc> mInstanceAudio;
    // AudioAdapter();
    std::string cacheStopSignelMQTT;

    std::mutex audioMutex;

    std::string mdatacheck;



    public:
    void setPlayAudioFunc(const  std::string &data);
    std::string getPlayAudioFunc(); 
    // AudioAdapter()= default;
    AudioFunc ();
    ~AudioFunc ();
    STATUS_AUDIO openFile (const std::string &filename);
    STATUS_AUDIO configureDevice ();
    STATUS_AUDIO playAudio ();
    STATUS_AUDIO pauseAudio ();
    STATUS_AUDIO resumeAudio ();
    STATUS_AUDIO stopAudio ();
    static std::shared_ptr<AudioFunc> getInstance ();
    // void runFileWAV (const std::string &filename);
    void handleLogicRunAudio (const std::string &data);
    std::string handleStopAudifromMQTT (const std::string &data = "");

    void handleLogicAudioCommandHMI (const std::string status);
    std::mutex &getMutex ();
    STATUS_AUDIO handlePlayAudioFunc(const std::string *data);
};
#endif