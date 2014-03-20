/* Copyright (C) 2013 Marcel Gutmann, SEMM                                           */
/*-----------------------------------------------------------------------------------*/
/* Projekt:         SEMM                                                             */
/* Modul:           RFID-Sensor (RFID)                                               */
/* Autor/en:        Marcel Gutmann (MG)                                              */
/*                                                                                   */          
/* Dateiname:       Rfid.h                                                           */     
/* Version:         0.9.4b                                                            */
/* Versionsstatus:  Experimentell                                                    */ 
/*                                                                                   */
/* Platform:        Arduino Nano / Atmega328                                         */
/*                                                                                   */
/* Beschreibung:    Klassen-Header zum Speichern und verwalten der gelesenen TAGs    */
/*-----------------------------------------------------------------------------------*/

#ifndef _RFID_H_
#define _RFID_H_

//Theoretisch sind 12 TAGs möglich. Der 12. Reader funktioniert aber nicht richtig
#define NUMBER_OF_TAGS 11

#include <Arduino.h>

class Rfid{
public:
  Rfid();
  void setTAG(byte tagNumber, byte dataA, byte dataB, byte dataC, byte dataD, byte dataE);
  void getTAGData(byte tagNumber, byte *buffer);
  void delTAGs();
private:
  struct 
    {
      byte d1;
      byte d2;
      byte d3;
      byte d4;
      byte d5;
    } tag[NUMBER_OF_TAGS];
};

Rfid::Rfid(){
  delTAGs();
}

void Rfid::delTAGs()
{
  for (int i = 0; i < NUMBER_OF_TAGS; i++)
  {
    tag[i].d1 = 0;
    tag[i].d2 = 0;
    tag[i].d3 = 0;
    tag[i].d4 = 0;
    tag[i].d5 = 0;
  }
}

void Rfid::setTAG(byte tagNumber, byte dataA, byte dataB, byte dataC, byte dataD, byte dataE)
{
  tag[tagNumber].d1 = dataA;
  tag[tagNumber].d2 = dataB;
  tag[tagNumber].d3 = dataC;
  tag[tagNumber].d4 = dataD;
  tag[tagNumber].d5 = dataE;
}

void Rfid::getTAGData(byte tagNumber, byte *buffer)
{
  *(buffer + 0) = tag[tagNumber].d4;
  *(buffer + 1) = tag[tagNumber].d5;
}

#endif // _RFID_H_
