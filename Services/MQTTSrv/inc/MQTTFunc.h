#ifndef MQTT_FUNC_H
#define MQTT_FUNC_H

#include "Logger.h"
#include "json.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <mosquitto.h>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "CanBus.h"
using json = nlohmann::json;

class MQTTFunc
{
    private:
    static std::shared_ptr<MQTTFunc> mInstanceMQTT;
    // static bool running;
    std::string mMessageResponse;

    std::function<void (const std::string &topic, const std::string &payload)> mOnMessageCallback;
    std::shared_ptr<struct mosquitto> mMosq;
    std::mutex mCallbackMutex;

    std::string mDataCANAdapter;

    public:
    MQTTFunc ();
    ~MQTTFunc ();

    static std::shared_ptr<MQTTFunc>getInstance ();
    void SubMQTT ();
    void PubMQTT (const std::string &command_json);
    void handleMessage (struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
    void connectToBroker (struct mosquitto *mosq, void *userdata, int result);
    void disconnectToBroker (struct mosquitto *mosq, void *userdata, int rc);
    void messageCallback (struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
    void sendMessage (const std::string &message);
    static void handleSignal (int signal);
    const std::string getResponseFromSub ();

    void handleEventSub ();
    void setOnMessageCallback (const std::function<void (const std::string &topic, const std::string &payload)> &callback);

    void setDataCANFromAdapter(const std::string &data);
    const std::string getDataCANFromAdapter();
    void testLog();
};

#endif