#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "Motor.h"
#include "Path.h"
#include "Pid.h"
#include "RotaryEncoder.h"
#include "local_defs.h"
//#include <Servo.h>
#include "servoDev.h"

class Data {
public:
    Data();
    ~Data();

    static Data d;

    Pid* pidSpeed[CNT_MOTORS];
    Pid* pidPos[CNT_MOTORS];
    Motor* motor[CNT_MOTORS];
    RotaryEncoder* rotaryEncoder[CNT_MOTORS];
    Path* path;
    vec2d* setpointPath;
    ServoDev* servo;
    float encoderPos[CNT_MOTORS];
    float output[CNT_MOTORS];
    float setpointSpeed[CNT_MOTORS];
    float setpointPos[CNT_MOTORS];
    float smoothedSpeed[CNT_MOTORS];
    bool sendData;
};

#endif
