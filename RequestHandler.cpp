#include <Arduino.h>
#include "RequestHandler.h"
#include "Config.h"

RequestHandler::RequestHandler(ServoController& controller)
    : servoController(controller) {}

int RequestHandler::parseSpeed(const String& request) {
    int speed = SLOW;

    int queryStart = request.indexOf("speed=");

    if (queryStart != -1) {
        int start = queryStart + 6;
        int end = request.indexOf(" ", start);

        String speedParam = request.substring(start, end);

        if (speedParam == "medium") {
            speed = MEDIUM;
        } else if (speedParam == "fast") {
            speed = FAST;
        }
    }

    return speed;
}

void RequestHandler::handle(WiFiClient& client, const String& request) {
    Serial.print("Incoming request: [");
    Serial.print(request);
    Serial.println("]");

    int speed = parseSpeed(request);

    if (request.indexOf("GET /rack/status ") == 0) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
    
        client.print("{\"isMoving\":");
        client.print(servoController.isMoving() ? "true" : "false");
    
        client.print(",\"currentPulse\":");
        client.print(servoController.getCurrentPulse());
    
        client.print(",\"targetPulse\":");
        client.print(servoController.getTargetPulse());
    
        client.println("}");
        return;
    }

    if (request.indexOf("GET /rack/clockwise") != -1) {
        servoController.clockwise(speed);
    }
    else if (request.indexOf("GET /rack/counterclockwise") != -1) {
        servoController.counterclockwise(speed);
    }
    else if (request.indexOf("GET /rack/stop") != -1) {
        servoController.stop();
    }
    else {
        client.println("HTTP/1.1 404 Not Found");
        client.println("Connection: close");
        client.println();
        client.println("Unknown route");
        return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("OK");
}