#pragma once

#include <WiFiS3.h>

class WiFiManager {
public:
    void begin();
    void update();

private:
    unsigned long lastReconnectAttempt = 0;
    bool wasConnected = false;

    static constexpr unsigned long RECONNECT_INTERVAL = 5000;
};