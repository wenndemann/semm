/* Copyright (C) 2013 Stefan Wenn, SEMM                                       */
/*----------------------------------------------------------------------------*/
/* Projekt: SEMM                                                              */
/* Modul: XY-Positioniereinheit (XYS)                                         */
/* Autor/en: Stefan wenn (SW)                                                 */
/*                                                                            */
/* Dateiname: motor.h                                          */
/* Version: 0.9                                                               */
/* Versionsstatus: Experimentell                                              */
/*                                                                            */ 
/* Platform: Arduino Uno                                                      */
/*                                                                            */
/* Beschreibung: dient der Ansteuerung eines Motors                           */
/*----------------------------------------------------------------------------*/

#ifndef __MOTOR_H
#define __MOTOR_H

class Motor {
 public:
   Motor(int enAPin, int enBPin, int pwmPin, int limAPin, int limBPin) ;
   bool getEnable() { return m_enable; }
   int getOutput() { return m_output; }
   void setEnable(bool enable) { m_enable = enable; }
   void setOutput(int value);
   bool isRef();
 
 private:
    int m_enAPin, m_enBPin, m_pwmPin, m_limAPin, m_limBPin;
    int m_output;  
    bool m_enable;
};

#endif
