#include <Arduino.h>
#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int aPin, int bPin) {
  m_aPin = aPin;
  m_bPin = bPin;
  m_pos = 0;
  m_speed = 0;
  m_lastTime = micros();
  
  pinMode(m_aPin, INPUT);
  pinMode(m_bPin, INPUT);
  digitalWrite(m_aPin, HIGH);  
  digitalWrite(m_bPin, HIGH);  
}

void RotaryEncoder::updatePos() {
  unsigned long now = micros();
  unsigned long timeChange(now - m_lastTime);
  byte neu, diff;
  long posOld = m_pos;
  float deltaT = timeChange / 1000000.0f; 
  
  neu = 0;
  if(digitalRead(m_aPin)) neu = 3;
  if(digitalRead(m_bPin)) neu ^= 1;	        // convert gray to binary
  diff = m_last - neu;			        // difference last - new
  if( diff & 1 ) {			        // bit 0 = value (1)
    m_last = neu;				        // store new as next last
    m_pos += (diff & 2) - 1;	                // bit 1 = direction (+/-)
  }
  if (m_pos - posOld) {
    m_speed = (m_pos - posOld) / deltaT;
    m_lastTime = now;
  }
  else if(deltaT > 0.02f) {
    m_speed = 0;
  }
  
}

