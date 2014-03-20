#include <Arduino.h>
#include "Motor.h"

Motor::Motor(int enAPin, int enBPin, int pwmPin, int limAPin, int limBPin) {
  m_enAPin = enAPin;
  m_enBPin = enBPin;
  m_pwmPin = pwmPin;
  m_limAPin = limAPin;
  m_limBPin = limBPin;

  m_enable = true;

  pinMode(m_enAPin, OUTPUT);
  pinMode(m_enBPin, OUTPUT);
  pinMode(m_pwmPin, OUTPUT);
  pinMode(m_limAPin, INPUT);
  pinMode(m_limBPin, INPUT);

  digitalWrite(m_limAPin, HIGH);
  digitalWrite(m_limBPin, HIGH);
}

void Motor::setOutput(int value) {
 
  if(m_enable) {

    m_output = value;

    analogWrite(m_pwmPin, abs(m_output));

    if(m_output > 0 && digitalRead(m_limAPin)) 
    {
      digitalWrite(m_enAPin, HIGH);
      digitalWrite(m_enBPin, LOW);

    }
    else if(m_output < 0 && digitalRead(m_limBPin)) 
    {
      digitalWrite(m_enBPin, HIGH);
      digitalWrite(m_enAPin, LOW);
    }
    else
    {
      digitalWrite(m_enBPin, LOW);
      digitalWrite(m_enAPin, LOW);
    }
  }
  else 
  {
    digitalWrite(m_enBPin, LOW);
    digitalWrite(m_enAPin, LOW);
  }
}

bool Motor::isRef() {
    return digitalRead(m_limBPin);
}
