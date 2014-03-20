#include <Arduino.h>
#include "Pid.h"

Pid::Pid(float kP, float kI, float kD) {
  m_kP = kP ;
  m_kI = kI;
  m_kD = kD;
  
  m_eSum = 0;
  m_eOld = 0;
 
  m_outMin = 0; 
  m_outMax = PID_STD_OUT_MAX;
  m_lastTime = 0;
  m_sampleTime = PID_STD_SAMPLE_TIME_MS;
}

int Pid::update(float actVal, float setpoint, float* out) {
  unsigned long now = millis();
  if((now - m_lastTime) > m_sampleTime) {
    float output = 0;
     
    //actVal = smoothData(actVal);

    m_e = setpoint - actVal;
    m_eSum += m_e * m_kI;
    
    if(m_kI == 0) m_eSum = 0;
    m_eSum *= 0.95;
    
    if(m_eSum >  m_outMax) m_eSum =  m_outMax;
    if(m_eSum < -m_outMax) m_eSum = -m_outMax;
    
    output = (m_kP * m_e) + (m_eSum) + (m_kD * (m_e - m_eOld));
    
    if(output <  m_outMin && output > 0) output = 0;
    if(output > -m_outMin && output < 0) output = 0;
    if(output >  m_outMax) output =  m_outMax;
    if(output < -m_outMax) output = -m_outMax;
    
   
    m_eOld = m_e;
    m_lastTime = now;
   
    *out = output;
    
    return 1;
  }
  else return 0;
}

void Pid::reset() {
  m_eSum = 0;
  m_eOld = 0;  
}


