#include"AudioFunc.h"


std::shared_ptr<AudioFunc> AudioFunc::mInstanceAudio = nullptr;
AudioFunc::AudioFunc ()
: pcmHandle (nullptr), params (nullptr), fileRB (nullptr), sampleRate (44100), numChannels (2), bitsPerSample (16), flagStop (false), flagPause (false),
  isPlaying (false)
{
    configureDevice();
}

AudioFunc::~AudioFunc ()
{
    stopAudio ();
    if (pcmHandle)
        snd_pcm_close (pcmHandle);
    if (fileRB)
        fclose (fileRB);
}
STATUS_AUDIO AudioFunc::openFile (const std::string &filename)
{
    if (fileRB)
    {
        fclose (fileRB);
    }
    fileRB = fopen (filename.c_str (), "rb");
    if (!fileRB)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return STATUS_AUDIO::ERROR_OPEN_FILE;
    }
    return STATUS_AUDIO::OK;
}
STATUS_AUDIO AudioFunc::configureDevice ()
{
    if (snd_pcm_open (&pcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {
        std::cerr << "Failed to open PCM device" << std::endl;
        return STATUS_AUDIO::ERROR_OPEN_DEVICE;
    }

    snd_pcm_hw_params_alloca (&params);
    snd_pcm_hw_params_any (pcmHandle, params);
    snd_pcm_hw_params_set_access (pcmHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format (pcmHandle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels (pcmHandle, params, numChannels);
    snd_pcm_hw_params_set_rate_near (pcmHandle, params, &sampleRate, 0);

    if (snd_pcm_hw_params (pcmHandle, params) < 0)
    {
        std::cerr << "Failed to set PCM hardware parameters" << std::endl;
        snd_pcm_close (pcmHandle);
        return STATUS_AUDIO::ERROR_OPEN_DEVICE;
    }

    return STATUS_AUDIO::OK ;
}

STATUS_AUDIO AudioFunc::playAudio ()
{
    if (!fileRB || !pcmHandle)
    {
        std::cerr << "Audio file or PCM device is not open." << std::endl;
        isPlaying = false;
        return STATUS_AUDIO::ERROR_OPEN_DEVICE;
    }

    flagStop  = false;
    isPlaying = true;
    flagPause = false;

    while (!flagStop)
    {
        {
            std::unique_lock<std::mutex> lock (mtx);
            cv.wait (lock, [this] () { return !flagPause || flagStop; });
            if (flagStop)
                break;
        }

        int frames = fread (buffer, sizeof (unsigned char), bufferSize, fileRB);
        if (frames <= 0)
        {
            if (feof (fileRB))
            {
                printLog_I ("End of file reached.");
                break;
            }
        }
        if (!pcmHandle)
        {
            std::cerr << "PCM device is closed. Exiting playback loop." << std::endl;
            break;
        }
        int framesToPlay = frames / (numChannels * (bitsPerSample / 8));
        int err          = snd_pcm_writei (pcmHandle, buffer, framesToPlay);
        if (err == -EPIPE)
        {
            snd_pcm_prepare (pcmHandle);
        }
        else if (err < 0)
        {
            std::cerr << "Error writing to PCM device: " << snd_strerror (err) << std::endl;
            break;
        }
    }
    if (pcmHandle)
    {
        snd_pcm_drain (pcmHandle);
    }
    isPlaying = false;
    printLog_I ("Playback finished or stopped.");
    return STATUS_AUDIO::OK;
}

STATUS_AUDIO AudioFunc::pauseAudio ()
{
    std::lock_guard<std::mutex> lock (mtx);
    if (!isPlaying || flagPause)
    {
        printLog_I ("Cannot pause, playback is not active or already paused.");
        return STATUS_AUDIO::ERROR_PLAY;
    }

    if (pcmHandle)
    {
        int err = snd_pcm_pause (pcmHandle, 1);
        if (err < 0)
        {
            std::cerr << "snd_pcm_pause not supported, stopping playback manually." << std::endl;
            snd_pcm_drop (pcmHandle);
        }
        else
        {
            printLog_I ("Playback paused via snd_pcm_pause.");
        }
    }

    flagPause = true;
    printLog_I ("Playback paused.");
    return STATUS_AUDIO::OK;
}

STATUS_AUDIO AudioFunc::resumeAudio ()
{
    {
        std::lock_guard<std::mutex> lock (mtx);
        if (!flagPause)
        {
            printLog_I ("Cannot resume, playback is not paused.");
            return STATUS_AUDIO::ERROR_PLAY;
        }

        if (pcmHandle)
        {
            int err = snd_pcm_pause (pcmHandle, 0);
            if (err < 0)
            {
                std::cerr << "snd_pcm_pause not supported, restarting playback manually." << std::endl;
                snd_pcm_prepare (pcmHandle);
            }
            else
            {
                printLog_I ("Playback resumed via snd_pcm_pause.");
            }
        }

        flagPause = false;
    }
    cv.notify_one ();
    return STATUS_AUDIO::OK;
}
STATUS_AUDIO AudioFunc::stopAudio ()
{
    {
        std::lock_guard<std::mutex> lock (mtx);
        if (!pcmHandle)
        {
            printLog_I ("PCM device is already closed.");
            return STATUS_AUDIO::ERROR_STOP;
        }

        flagStop  = true;
        flagPause = false;
    }

    if (playThread.joinable ())
    {
        playThread.join ();
    }

    if (pcmHandle)
    {
        snd_pcm_drop (pcmHandle);
        snd_pcm_close (pcmHandle);
        pcmHandle = nullptr;
    }

    if (fileRB)
    {
        fclose (fileRB);
        fileRB = nullptr;
    }

    isPlaying = false;
    printLog_I ("Playback stopped and resources freed.");
    return STATUS_AUDIO::OK;
}

std::shared_ptr<AudioFunc> AudioFunc::getInstance ()
{
    if (mInstanceAudio == nullptr)
    {
        mInstanceAudio = std::make_unique<AudioFunc> ();
    }

    return mInstanceAudio;
}

// void AudioFunc::runFileWAV (const std::string &filename)
// {
//     stopAudio();
//     if (isPlaying)
//     {
//         printLog_I ("Audio is already playing. Please wait until it finishes.");
//         return;
//     }
//     if (!openFile (filename))
//     {
//         std::cerr << "Failed to open WAV file: " << filename << std::endl;
//         return ;
//     }
//     if (!pcmHandle)
//     {
//         if (!configureDevice ())
//         {
//             std::cerr << "Failed to configure ALSA device." << std::endl;
//             return;
//         }
//     }
//     if (playThread.joinable ())
//     {
//         playThread.join ();
//     }
//     playThread = std::thread(&AudioFunc::playAudio, this);
//      playThread.detach();
//     //std::thread ([this] () { playAudio (); }).detach ();
// }
STATUS_AUDIO AudioFunc::handlePlayAudioFunc (const std::string *data)
{
    STATUS_AUDIO statusAudio = STATUS_AUDIO::OK;
    statusAudio = openFile(*data);
    if(statusAudio != STATUS_AUDIO::OK)
    {

        return STATUS_AUDIO::ERROR_OPEN_FILE;
    }
    statusAudio = configureDevice();
    if(statusAudio != STATUS_AUDIO::OK)
    {

        return STATUS_AUDIO::ERROR_OPEN_DEVICE;
    }
    statusAudio = playAudio();
    if(statusAudio != STATUS_AUDIO::OK)
    {
        return STATUS_AUDIO::ERROR_PLAY;    
    }
    return STATUS_AUDIO::OK;
}

std::mutex& AudioFunc::getMutex() 
{ 
    return audioMutex;
}
void AudioFunc::setPlayAudioFunc(const std::string &data)
{
    mdatacheck = data;
}
std::string AudioFunc::getPlayAudioFunc(){
    return mdatacheck;
}
