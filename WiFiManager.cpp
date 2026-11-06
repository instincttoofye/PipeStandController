#include <Arduino.h>
#include "WiFiManager.h"
#include "Config.h"

void WiFiManager::begin() {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
}

void WiFiManager::update() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!wasConnected) {
            wasConnected = true;

            Serial.println("WiFi connected!");
            Serial.print("Arduino IP: ");
            Serial.println(WiFi.localIP());
        }

        return;
    }

    if (wasConnected) {
        wasConnected = false;
        Serial.println("WiFi disconnected!");
    }

    unsigned long now = millis();

    if (now - lastReconnectAttempt >= RECONNECT_INTERVAL) {
        lastReconnectAttempt = now;

        Serial.println("Attempting WiFi reconnection...");
        WiFi.begin(ssid, password);
    }
}