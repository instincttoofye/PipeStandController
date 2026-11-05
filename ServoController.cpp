#include <Arduino.h>
#include "ServoController.h"

void ServoController::begin() {
    rackServo.attach(SERVO_PIN);
    rackServo.writeMicroseconds(STOP);
}

void ServoController::clockwise(int speed) {
    targetPulse = STOP - speed;

    rotationStartedAt = millis();

    isRotating = true;

    Serial.println("Clockwise");
}

void ServoController::counterclockwise(int speed) {
    targetPulse = STOP + speed;

    rotationStartedAt = millis();
    isRotating = true;

    Serial.println("Counterclockwise");
}

void ServoController::stop() {
    targetPulse = STOP;
    isRotating = false;

    Serial.println("Stopped");
}

void ServoController::checkTimeout() {
    if (isRotating && millis() - rotationStartedAt >= MAX_RUN_TIME) {
        targetPulse = STOP;
        currentPulse = STOP;

        rackServo.writeMicroseconds(STOP);

        isRotating = false;

        Serial.println("Safety timeout: Servo stopped");
    }
}

void ServoController::updateRamp() {
    if (currentPulse == targetPulse)
        return;

    unsigned long now = millis();

    if (now - lastRampUpdate < RAMP_INTERVAL)
        return;

    lastRampUpdate = now;

    if (currentPulse < targetPulse) {
        currentPulse = min(currentPulse + RAMP_STEP, targetPulse);
    } else {
        currentPulse = max(currentPulse - RAMP_STEP, targetPulse);
    }

    rackServo.writeMicroseconds(currentPulse);
}

void ServoController::update() {
    checkTimeout();
    updateRamp();
}

bool ServoController::isMoving() const {
    return currentPulse != STOP || targetPulse != STOP;
}

int ServoController::getCurrentPulse() const {
    return currentPulse;
}

int ServoController::getTargetPulse() const {
    return targetPulse;
}