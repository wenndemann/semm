#include <Arduino.h>
#include "servoDev.h"

ServoDev::ServoDev(int servoPin) {
    m_servoPin = servoPin; 
    m_dev = new Servo;

    m_min = 45;
    m_max = 95;
    m_state = true;
}

ServoDev::~ServoDev() {
    m_dev->detach();
    delete m_dev;
}

void ServoDev::setState(bool value) {
    m_state = value;
    if(!m_dev->attached()) m_dev->attach(m_servoPin);
    if(m_state) {
        m_dev->write(m_max);
    }
    else {
        m_dev->write(m_min);
    }
    delay(600);
    m_dev->detach();
}
