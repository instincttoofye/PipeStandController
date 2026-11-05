#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Servo.h>
#include "Config.h"

class ServoController {
    public:
        void begin();

        void clockwise(int speed);
        void counterclockwise(int speed);
        void stop();
        void update();

        bool isMoving() const;
        int getCurrentPulse() const;
        int getTargetPulse() const;

    private:
        Servo rackServo;
        unsigned long rotationStartedAt = 0;
        unsigned long lastRampUpdate = 0;

        bool isRotating = false;

        int currentPulse = STOP;
        int targetPulse = STOP;

        void updateRamp();
        void checkTimeout();
};
#endif