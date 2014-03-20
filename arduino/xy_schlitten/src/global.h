#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "Motor.h"
#include "Path.h"
#include "Pid.h"
#include "RotaryEncoder.h"

class Data {
public:
    Data();
    ~Data();

    Pid pidSpeed[CNT_MOTORS];
    Pid pidPos[CNT_MOTORS];
    Motor motor[CNT_MOTORS];
    RotaryEncoder rotaryEncoder[CNT_MOTORS];
    Path path;
    float encoderPos[CNT_MOTORS];
    float output[CNT_MOTORS];
    float setpointSpeed[CNT_MOTORS];
    float setpointPos[CNT_MOTORS];
    vec2d setpointPath;
    float smoothedSpeed[CNT_MOTORS];
    bool sendData;
};

#endif
