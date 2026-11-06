#pragma once

#include <WiFiS3.h>
#include "RequestHandler.h"

class HttpClientHandler {
public:
    explicit HttpClientHandler(RequestHandler& requestHandler);

    void update(WiFiServer& server);

private:
    RequestHandler& requestHandler;

    WiFiClient activeClient;
    String requestLine;

    unsigned long connectionStartedAt = 0;

    static constexpr unsigned long CLIENT_TIMEOUT = 2000;
    static constexpr size_t MAX_REQUEST_LENGTH = 256;

    void acceptClient(WiFiServer& server);
    void processClient();
    void closeClient();
};