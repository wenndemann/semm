/* Copyright (C) 2013 Stefan Wenn, SEMM                                       */
/*----------------------------------------------------------------------------*/
/* Projekt: SEMM                                                              */
/* Modul: XY-Positioniereinheit (XYS)                                         */
/* Autor/en: Stefan wenn (SW)                                                 */
/*                                                                            */
/* Dateiname: xy_schlitten.ino                                                */
/* Version: 0.9                                                               */
/* Versionsstatus: Experimentell                                              */
/*                                                                            */ 
/* Platform: Arduino Uno                                                      */
/*                                                                            */
/* Beschreibung: Stellt die Funktion eines PID Reglers dar.                   */
/*----------------------------------------------------------------------------*/

#ifndef __PID_H
#define __PID_H

#define PID_STD_OUT_MAX 255;
#define PID_STD_SAMPLE_TIME_MS 1;

class Pid {
 public:
   Pid(float kP, float kI, float kD);
   int update(float actVal, float setpoint, float* out);
   float getKp() { return m_kP; }
   float getKi() { return m_kI; }
   float getKd() { return m_kD; }
   float getOutMin() { return m_outMin; }
   float getOutMax() { return m_outMax; }
   
   void setKp(float val) { m_kP = val; }
   void setKi(float val) { m_kI = val; m_eSum = 0;}
   void setKd(float val) { m_kD = val; }
   void setOutMin(float val) { m_outMin = val; }
   void setOutMax(float val) { m_outMax = val; }
   
   void setSampleTime(int sampleTime) { m_sampleTime = sampleTime; }
   
   void reset();
 
 private:
   float m_kP, m_kI, m_kD;
   float m_e, m_eSum, m_eOld;
   float m_outMin, m_outMax;
   unsigned long m_lastTime;
   int m_sampleTime;
   float m_smooth;
  
};


#endif
