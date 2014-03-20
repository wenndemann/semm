#include <Arduino.h>
#include "Path.h"

Path::Path() {
    m_t0 = millis() / 1000.0f;
    m_t1 = millis() / 1000.0f;
    m_t2 = millis() / 1000.0f;
    m_t3 = millis() / 1000.0f;

    m_s0 = 0;
    m_s1 = 0;
    m_s2 = 0;
    m_s3 = 0; 

    m_v0 = 0;

    m_aAcc = 2000;
    m_aDec = -500;
    m_vLimit = 900;
    m_vMax = 0;

    m_ratio.x = 0;
    m_ratio.y = 0;

    m_running = true;
}

void Path::newSetpoint(const vec2d& setpoint, const vec2d& actPos, float actSpeed) {
    float sDelta = sqrt(pow((actPos.x - setpoint.x),2) + pow((actPos.y - setpoint.y),2));
    if(sDelta == 0) return;

    m_start = actPos;
    m_running = true;

    m_ratio.x = (setpoint.x - actPos.x) / sDelta; 
    m_ratio.y = (setpoint.y - actPos.y) / sDelta;   

    m_t0 = millis() / 1000.0f;
    m_v0 = actSpeed;

    m_vMax = sqrt(((sDelta * 2.0f * m_aAcc) + (m_v0*m_v0)) / (1 - (m_aAcc/m_aDec)));

    if(m_vMax > m_vLimit) m_vMax = m_vLimit;

    float tAcc = ((m_vMax - m_v0   )  / m_aAcc);
    float tDec = ((0    - m_vMax )  / m_aDec);
    float sAcc = ((m_v0   + m_vMax) / 2 ) * ((m_vMax - m_v0   ) / m_aAcc);
    float sDec = ((m_vMax + 0   ) / 2 ) * ((0    - m_vMax ) / m_aDec);
    float sConst = sDelta - sAcc - sDec;
    float tConst = (sConst / m_vMax);

    m_s1 = sAcc;
    m_s2 = m_s1 + sConst;
    m_s3 = m_s2 + sDec;

    m_t1 = m_t0 + tAcc;
    m_t2 = m_t1 + tConst;
    m_t3 = m_t2 + tDec;
}

void Path::update(vec2d* actPos) {
    float sAct = 0, t = 0;
    float tAct = millis() / 1000.0f;


    if(tAct < m_t1) { //acceleration
        t = tAct - m_t0;
        sAct = (m_aAcc / 2 * pow(t,2)) + (m_v0 * t) + m_s0;
    }
    else if (tAct < m_t2) { //const speed
        t = tAct - m_t1;
        sAct = (m_vMax * t) + m_s1;
    }
    else if (tAct < m_t3) { //deceleration
        t = tAct - m_t2;
        sAct = (m_aDec / 2 * pow(t,2)) + (m_vMax * t) + m_s2;
    }
    else {
        sAct = m_s3;
    }
    if (tAct > (m_t3 + 0.25))
        m_running = false;

    actPos->x = m_start.x + (sAct * m_ratio.x);
    actPos->y = m_start.y + (sAct * m_ratio.y);
}

