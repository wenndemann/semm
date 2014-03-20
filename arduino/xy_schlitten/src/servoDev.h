#ifndef __SERVO_H
#define __SERVO_H

#include <Servo.h>

class ServoDev {
 public:
   ServoDev(int servoPin);
   ~ServoDev();
   bool getState() { return m_state; }
   int getMin() { return m_min; }
   int getMax() { return m_max; }

   void setState(bool value);
   void setMin(int value) { if(value >= 0) m_min = value; }
   void setMax(int value) { if(value <= 255) m_max = value; }
 
 private:
    int m_servoPin;
    int m_min, m_max;
    bool m_state;
    Servo* m_dev;
};

#endif

