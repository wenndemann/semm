#include <Arduino.h>
#include "data.h"

Data::Data() {
    
    //motors and encoders
    motor[0] = new Motor(M0_EN_A_PIN, M0_EN_B_PIN, M0_PWM_PIN, M0_LIM_A_PIN, M0_LIM_B_PIN);
    motor[1] = new Motor(M1_EN_A_PIN, M1_EN_B_PIN, M1_PWM_PIN, M1_LIM_A_PIN, M1_LIM_B_PIN);

    rotaryEncoder[0] = new RotaryEncoder(M0_RE_A_PIN, M0_RE_B_PIN);
    rotaryEncoder[1] = new RotaryEncoder(M1_RE_A_PIN, M1_RE_B_PIN);

    for( int i = 0; i < CNT_MOTORS; i++) {
        pidSpeed[i] = new Pid(KP_SPEED, KI_SPEED, KD_SPEED);
        pidSpeed[i]->setOutMin(15.0f);

        pidPos[i] = new Pid(KP_POS, KI_POS, KD_POS);
        pidPos[i]->setOutMax(1000.0f);
        pidPos[i]->setSampleTime(10.0f);    
        pidPos[i]->setOutMin(15.0f);
    }

    //magnet
    servo = new ServoDev(MAG_PIN);

    setpointPath = new vec2d();
    path = new Path();
}

Data::~Data() {
    delete[] pidSpeed;
    delete[] pidPos;
    delete[] motor;
    delete[] rotaryEncoder;
    delete path;
    delete setpointPath;
    delete servo;
}
