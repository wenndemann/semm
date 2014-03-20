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
/* Beschreibung: Berechnet Start- und Stopprampen für eine Stecke vom         */
/* Punkt zu einem anderen                                                     */
/*----------------------------------------------------------------------------*/

#ifndef __PATH_H
#define __PATH_H

#include <Streaming.h>

typedef struct vec2d{
  vec2d() { x=0.0f; y=0.0f; };
  vec2d(float _x, float _y) { x = _x; y = _y; }
  vec2d(const vec2d& vec) { this->x = vec.x; this->y = vec.y; }
  float getX() { return this->x; }
  float getY() { return this->y; }
  float x,y;
} vec2d;

class Path {
public:
  Path();
  float getVMax() { return m_vMax; }
  float getAAcc() { return m_aAcc; }
  float getADec() { return m_aDec; }
  bool getRunning() { return m_running; }

  void setVMax(float val) { m_vMax = val; }
  void setAAcc(float val) { m_aAcc = val; }
  void setADec(float val) { m_aDec = val; }
  void newSetpoint(const vec2d& setpoint, const vec2d& actPos, float actSpeed) ;
  void update(vec2d* actPos);

private:
  float m_t0, m_t1, m_t2, m_t3;
  float m_s0, m_s1, m_s2, m_s3;
  float m_v0;
  
  float m_aAcc;
  float m_aDec;
  float m_vLimit;
  float m_vMax;
  vec2d m_ratio;
  vec2d m_start;
  bool m_running;
};


#endif
