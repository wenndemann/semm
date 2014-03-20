/* Copyright (C) 2013 Stefan Wenn, SEMM                                       */
/*----------------------------------------------------------------------------*/
/* Projekt: SEMM                                                              */
/* Modul: XY-Positioniereinheit (XYS)                                         */
/* Autor/en: Stefan wenn (SW)                                                 */
/*                                                                            */
/* Dateiname: rotaryrncoder.h                                                 */
/* Version: 0.9                                                               */
/* Versionsstatus: Experimentell                                              */
/*                                                                            */ 
/* Platform: Arduino Uno                                                      */
/*                                                                            */
/* Beschreibung: Dient dem Auswerten und Auslensen eines Inkrementalgebers    */
/*----------------------------------------------------------------------------*/

#ifndef __ENCODER_H
#define __ENCODER_H

#include <Arduino.h>

class RotaryEncoder {
  public:
    RotaryEncoder(int aPin, int bPin);
    long getPos() { return m_pos; }
    float getSpeed() { return m_speed; }
    int setSpeedUpdateTimeMs() { return m_speedUpdateTimeMs; }
    void setPos(long pos) { m_pos = pos; }
    void setSpeedUpdateTimeMs(int time) { m_speedUpdateTimeMs = time; }
    void updatePos();
  
  private:
    int m_aPin, m_bPin;
    long m_pos;
    byte m_last;
    unsigned long m_lastTime;
    float m_speed;
    int m_speedUpdateTimeMs;
};

#endif
