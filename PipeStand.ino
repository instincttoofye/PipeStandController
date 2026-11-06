#include <WiFiS3.h>
#include "Config.h"
#include "ServoController.h"
#include "RequestHandler.h"
#include "WiFiManager.h"
#include "HttpClientHandler.h"

ServoController servoController;
RequestHandler requestHandler(servoController);
HttpClientHandler httpClientHandler(requestHandler);
WiFiManager wifiManager;

WiFiServer server(80);

void setup() {
    Serial.begin(9600);
    delay(2000);

    Serial.println("Starting controller");

    servoController.begin();

    wifiManager.begin();

    server.begin();
    Serial.println("HTTP Server Started");
}

void loop() {
  servoController.update();

  wifiManager.update();

  if (WiFi.status() == WL_CONNECTED) {
      httpClientHandler.update(server);
  }
}