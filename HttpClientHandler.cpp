#include <Arduino.h>
#include "HttpClientHandler.h"

HttpClientHandler::HttpClientHandler(RequestHandler& handler) 
    : requestHandler(handler) {}

void HttpClientHandler::update(WiFiServer& server) {
    if(!activeClient) {
        acceptClient(server);
    }

    if(activeClient) {
        processClient();
    }
}

void HttpClientHandler::acceptClient(WiFiServer& server) {
    WiFiClient client = server.available();

    if (!client) return;

    activeClient = client;
    requestLine = "";
    connectionStartedAt = millis();

    Serial.println("HTTP client connected");
}

void HttpClientHandler::processClient() {
    if (millis() - connectionStartedAt >= CLIENT_TIMEOUT) {
        Serial.println("HTTP client timed out");
        closeClient();
        return;
    }

    if (!activeClient.connected() && !activeClient.available()) {
        closeClient();
        return;
    }

    while (activeClient.available() > 0) {
        char character = activeClient.read();

        if (character == '\n') {
            requestLine.trim();

            requestHandler.handle(activeClient, requestLine);

            closeClient();
            return;
        }

        if (requestLine.length() >= MAX_REQUEST_LENGTH) {
            Serial.println("HTTP request too long");
            closeClient();
            return;
        }

        requestLine += character;
    }
}

void HttpClientHandler::closeClient() {
    activeClient.stop();

    activeClient = WiFiClient();
    requestLine = "";
    connectionStartedAt = 0;

    Serial.println("HTTP client disconnected");
}