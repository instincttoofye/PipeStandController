#include <WiFiS3.h>
#include "Config.h"
#include "ServoController.h"
#include "RequestHandler.h"

ServoController servoController;
RequestHandler requestHandler(servoController);

WiFiServer server(80);

void setup() {
    Serial.begin(9600);
    delay(2000);

    Serial.println("Starting controller");

    servoController.begin();

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");

    IPAddress ip = WiFi.localIP();

    while (ip == IPAddress(0, 0, 0, 0)) {
        delay(500);
        ip = WiFi.localIP();
    }

    Serial.print("Arduino IP: ");
    Serial.println(ip);

    server.begin();
    Serial.println("HTTP Server Started");
}

void loop() {
    servoController.update();

    WiFiClient client = server.available();

    if (!client) {
        return;
    }

    client.setTimeout(1000);

    String request = client.readStringUntil('\n');
    request.trim();

    requestHandler.handle(client, request);

    client.flush();
    client.stop();
}