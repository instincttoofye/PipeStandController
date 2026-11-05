#pragma once

#include <WiFiS3.h>
#include "ServoController.h"

class RequestHandler {
    public:
        explicit RequestHandler(ServoController& controller);

        void handle(WiFiClient& client, const String& request);
    
    private:
        ServoController& servoController;

        int parseSpeed(const String& request);
};