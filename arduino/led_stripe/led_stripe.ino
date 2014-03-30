/* Copyright (C) 2013 Martina Klein, SEMM                                            */
/*-----------------------------------------------------------------------------------*/
/* Projekt:         SEMM                                                             */
/* Modul:           LED-Streifen                                                     */
/* Autor/en:        Martina Klein (MK), Stefan Wenn (SW), Eleonora Epp (EE)          */
/*                                                                                   */          
/* Dateiname:       led_streifen.ino                                                 */     
/* Version:         1.0.1                                                            */
/* Versionsstatus:  fertig                                                           */  
/*                                                                                   */
/* Platform:        Arduino Mega2560                                                 */
/*                                                                                   */
/* Beschreibung:    Gibt auf Befehl den Status, den Leucht-Modus und die Farbe an    */ 
/*                  jeweilige Spielerecke aus                                        */ 
/*                                                                                   */ 
/*-----------------------------------------------------------------------------------*/

#include <SoftPWM.h>
#include <Wire.h>
#include "i2c_defs.h"
#include "farben.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define rampeAuf 3
#define rampeAb 4

#define bluePin1 A0 
#define redPin1 A1
#define greenPin1 A2 
#define bluePin2 A3
#define redPin2 A4
#define greenPin2 A5 
#define bluePin3 A6
#define redPin3 A7
#define greenPin3 A8     
#define bluePin4 A9
#define redPin4 A10
#define greenPin4 A11  

static volatile bool data = false;
volatile char buf[16];

byte faktorRot = 100;  //LEDs sind 100% an
byte faktorGruen = 100;
byte faktorBlau = 100;

int (*out1)[6];    //Zeiger auf aktuelle Farbe/Modus (Zeiger auf zweidimensionales Feld)
int (*out2)[6];
int (*out3)[6];
int (*out4)[6];

void setup ()
{
  Serial.begin(9600); //x
  SoftPWMBegin();
  Serial.println("SEMM LED STREIFEN");
  
  //**Wire.begin(); // externen Zugriff gewähren
 
  Wire.begin(I2C_LS_ADDR);  //SLS = Streifen
  Wire.onReceive(I2C_RECEICVE_INT);
  

  SoftPWMSet(redPin1, 0);                   //LED aus
  SoftPWMSet(greenPin1, 0);
  SoftPWMSet(bluePin1, 0);

  SoftPWMSet(redPin2, 0);                   //LED aus
  SoftPWMSet(greenPin2, 0);
  SoftPWMSet(bluePin2, 0);
  
  SoftPWMSet(redPin3, 0);                   //LED aus
  SoftPWMSet(greenPin3, 0);
  SoftPWMSet(bluePin3, 0);

  SoftPWMSet(redPin4, 0);                   //LED aus
  SoftPWMSet(greenPin4, 0);
  SoftPWMSet(bluePin4, 0);

  out1 = aus; //aktuelle Farbe/Modus = aus 
  out2 = aus; //aktuelle Farbe/Modus = aus 
  out3 = aus; //aktuelle Farbe/Modus = aus 
  out4 = aus; //aktuelle Farbe/Modus = aus 
}    


void loop () 
{
  float outRot, outGruen, outBlau;

  for( int i = 0; i < 6; i++) {
    outRot = out1[RED][i];        //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out1[GREEN][i];
    outBlau = out1[BLUE][i];
    SoftPWMSetFadeTime(redPin1, out1[rampeAuf][i], out1[rampeAb][i]);
    SoftPWMSetFadeTime(greenPin1, out1[rampeAuf][i], out1[rampeAb][i]);
    SoftPWMSetFadeTime(bluePin1, out1[rampeAuf][i], out1[rampeAb][i]);
    SoftPWMSet(redPin1, (byte)outRot);                  //Ausgabewerte an PWM übergeben
    SoftPWMSet(greenPin1, (byte)outGruen);
    SoftPWMSet(bluePin1, (byte)outBlau);
      delay(62);    
    
    outRot = out2[RED][i];        //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out2[GREEN][i];
    outBlau = out2[BLUE][i];
    SoftPWMSetFadeTime(redPin2, out2[rampeAuf][i], out2[rampeAb][i]);
    SoftPWMSetFadeTime(greenPin2, out2[rampeAuf][i], out2[rampeAb][i]);
    SoftPWMSetFadeTime(bluePin2, out2[rampeAuf][i], out2[rampeAb][i]);
    SoftPWMSet(redPin2, (byte)outRot);                  //Ausgabewerte an PWM übergeben
    SoftPWMSet(greenPin2, (byte)outGruen);
    SoftPWMSet(bluePin2, (byte)outBlau);
      delay(62);    
    
    outRot = out3[RED][i];        //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out3[GREEN][i] ;
    outBlau = out3[BLUE][i] ;
    SoftPWMSetFadeTime(redPin3, out3[rampeAuf][i], out3[rampeAb][i]);
    SoftPWMSetFadeTime(greenPin3, out3[rampeAuf][i], out3[rampeAb][i]);
    SoftPWMSetFadeTime(bluePin3, out3[rampeAuf][i], out3[rampeAb][i]);
    SoftPWMSet(redPin3, (byte)outRot);                  //Ausgabewerte an PWM übergeben
    SoftPWMSet(greenPin3, (byte)outGruen);
    SoftPWMSet(bluePin3, (byte)outBlau);
      delay(62);    
    
    outRot = out4[RED][i] ;        //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out4[GREEN][i];
    outBlau = out4[BLUE][i] ;
    SoftPWMSetFadeTime(redPin4, out4[rampeAuf][i], out4[rampeAb][i]);
    SoftPWMSetFadeTime(greenPin4, out4[rampeAuf][i], out4[rampeAb][i]);
    SoftPWMSetFadeTime(bluePin4, out4[rampeAuf][i], out4[rampeAb][i]);
    SoftPWMSet(redPin4, (byte)outRot);                  //Ausgabewerte an PWM übergeben
    SoftPWMSet(greenPin4, (byte)outGruen);
    SoftPWMSet(bluePin4, (byte)outBlau);
      delay(62);    

    if(data) {
      for(int i = 0; i < 8; i++)
        buf[i] = Wire.read();
      auswerten_8(buf[0], buf[1]);
      auswerten_1(buf[6], buf[7]);
      auswerten_4(buf[2], buf[3]);
      auswerten_2(buf[4], buf[5]);
      data = false;
    }
  }
}

void I2C_RECEICVE_INT(int howMany)//externer Zugriff für Stefan möglich
{
/*
  //cli(); 
  Serial.print("I2C Event ");
  Serial.println(howMany);
  //char buffer[16];
  byte i = 0;
  
  delay(10);
  
  while (Wire.available()) { //lese 3 Zeichen in Buffer
    buf[i] = (char)Wire.read();
    i++;
    if (i >=  8) break;  //breche nach 3 Zeichen ab
  }
*/  
//  while (Wire.available())   //leere input buffer
//    (char)Wire.read();
  data = true;
  //if(i >= 8) auswerten(buffer);
  //sei();
  //delay(10);
}


void serialEvent() { //x
  Serial.println("serialEvent");
  char buffer[3];
  byte i = 0;
  
  while (Serial.available()) { //lese 3 Zeichen in Buffer
    buffer[i] = (char)Serial.read() - '0'; //0-Zeichen im asci abziehen um wiwedr auf zahlen zu komen
    i++;
    if (i > 3) break;  //breche nach 3 Zeichen ab
  }
  while (Serial.available())   //leere input buffer
    (char)Serial.read();

//  auswerten(buffer[0], buffer[1], buffer[2]);
  auswerten(buffer);
}


void auswerten(volatile char* buf) {
 
//  Serial.print(befehl);
//  Serial.print(" ");
//  Serial.print(farbe);
//  Serial.print(" ");
//  Serial.println(ecke);
  
  //int (**out)[6];
  //*out = aus;
//  switch(ecke)
//  {
//    case 8:
Serial.print("1 ");
        //auswerten_8(buf[0], buf[1]);
        
//      Serial.print(" Ecke1 ");
//      out = &out1;
//      break;
//    case 1:
Serial.print("2 ");
        auswerten_1(buf[6], buf[7]);
//      out = &out2;
//      Serial.print(" Ecke2 ");
//      break;
//    case 4:
Serial.print("3 ");
        auswerten_4(buf[2], buf[3]);
//      out = &out3;
//      Serial.print(" Ecke3 ");
//      break;
//    case 2:
Serial.print("4 ");
        auswerten_2(buf[4], buf[5]);
//      out = &out4;
//      Serial.print(" Ecke4 ");
//      break;
//    default:
//      Serial.println("ecke unbekannt");
//      return;
//      break;
//  }
 Serial.println(" ");
}

void auswerten_1(char befehl, char farbe) {
  switch(befehl) //modus
  {
  case 0: out2 = aus; break;
    
  case 1: // an
    Serial.print("an ");
    switch(farbe) {
      case 1: out2 = blau_an;      break;
      case 2: out2 = pink_an;      break;
      case 3: out2 = gruen_an;     break;
      case 4: out2 = hblau_an;     break;
      case 5: out2 = gelb_an;      break;
      case 6: out2 = rot_an;       break;
      case 7: out2 = orange_an;    break;
      case 8: out2 = weiss_an;     break;
      default: return; break;  
    } break;
    
  case 2: //blinken
    switch(farbe) {
      case 1: out2 = blau_herz;    break;
      case 2: out2 = pink_herz;    break;
      case 3: out2 = gruen_herz;   break;
      case 4: out2 = hblau_herz;   break;
      case 5: out2 = gelb_herz;    break;
      case 6: out2 = rot_herz;     break;
      case 7: out2 = orange_herz;  break;
      case 8: out2 = weiss_herz;   break;
      default: return; break;  
    } break;
    
    case 3: //angst
    switch(farbe) {
      case 1: out2 = blau_angst;   break;
      case 2: out2 = pink_angst;   break;
      case 3: out2 = gruen_angst;  break;
      case 4: out2 = hblau_angst;  break;
      case 5: out2 = gelb_angst;   break;
      case 6: out2 = rot_angst;    break;
      case 7: out2 = orange_angst; break;
      case 8: out2 = weiss_angst;  break;
      default: return; break;  
    } break;

  default: return; break;  
  }  
}

void auswerten_2(char befehl, char farbe) {
  switch(befehl) //modus
  {
  case 0: out4 = aus; break;
    
  case 1: // an
    Serial.print("an ");
    switch(farbe) {
      case 1: out4 = blau_an;      break;
      case 2: out4 = pink_an;      break;
      case 3: out4 = gruen_an;     break;
      case 4: out4 = hblau_an;     break;
      case 5: out4 = gelb_an;      break;
      case 6: out4 = rot_an;       break;
      case 7: out4 = orange_an;    break;
      case 8: out4 = weiss_an;     break;
      default: return; break;  
    } break;
    
  case 2: //blinken
    switch(farbe) {
      case 1: out4 = blau_herz;    break;
      case 2: out4 = pink_herz;    break;
      case 3: out4 = gruen_herz;   break;
      case 4: out4 = hblau_herz;   break;
      case 5: out4 = gelb_herz;    break;
      case 6: out4 = rot_herz;     break;
      case 7: out4 = orange_herz;  break;
      case 8: out4 = weiss_herz;   break;
      default: return; break;  
    } break;
    
    case 3: //angst
    switch(farbe) {
      case 1: out4 = blau_angst;   break;
      case 2: out4 = pink_angst;   break;
      case 3: out4 = gruen_angst;  break;
      case 4: out4 = hblau_angst;  break;
      case 5: out4 = gelb_angst;   break;
      case 6: out4 = rot_angst;    break;
      case 7: out4 = orange_angst; break;
      case 8: out4 = weiss_angst;  break;
      default: return; break;  
    } break;

  default: return; break;  
  }  
}

void auswerten_4(char befehl, char farbe) {
  switch(befehl) //modus
  {
  case 0: out3 = aus; break;
    
  case 1: // an
    Serial.print("an ");
    switch(farbe) {
      case 1: out3 = blau_an;      break;
      case 2: out3 = pink_an;      break;
      case 3: out3 = gruen_an;     break;
      case 4: out3 = hblau_an;     break;
      case 5: out3 = gelb_an;      break;
      case 6: out3 = rot_an;       break;
      case 7: out3 = orange_an;    break;
      case 8: out3 = weiss_an;     break;
      default: return; break;  
    } break;
    
  case 2: //blinken
    switch(farbe) {
      case 1: out3 = blau_herz;    break;
      case 2: out3 = pink_herz;    break;
      case 3: out3 = gruen_herz;   break;
      case 4: out3 = hblau_herz;   break;
      case 5: out3 = gelb_herz;    break;
      case 6: out3 = rot_herz;     break;
      case 7: out3 = orange_herz;  break;
      case 8: out3 = weiss_herz;   break;
      default: return; break;  
    } break;
    
    case 3: //angst
    switch(farbe) {
      case 1: out3 = blau_angst;   break;
      case 2: out3 = pink_angst;   break;
      case 3: out3 = gruen_angst;  break;
      case 4: out3 = hblau_angst;  break;
      case 5: out3 = gelb_angst;   break;
      case 6: out3 = rot_angst;    break;
      case 7: out3 = orange_angst; break;
      case 8: out3 = weiss_angst;  break;
      default: return; break;  
    } break;

  default: return; break;  
  }  
}

void auswerten_8(char befehl, char farbe) {
  switch(befehl) //modus
  {
  case 0: out1 = aus; break;
    
  case 1: // an
    Serial.print("an ");
    switch(farbe) {
      case 1: out1 = blau_an;      break;
      case 2: out1 = pink_an;      break;
      case 3: out1 = gruen_an;     break;
      case 4: out1 = hblau_an;     break;
      case 5: out1 = gelb_an;      break;
      case 6: out1 = rot_an;       break;
      case 7: out1 = orange_an;    break;
      case 8: out1 = weiss_an;     break;
      default: return; break;  
    } break;
    
  case 2: //blinken
    switch(farbe) {
      case 1: out1 = blau_herz;    break;
      case 2: out1 = pink_herz;    break;
      case 3: out1 = gruen_herz;   break;
      case 4: out1 = hblau_herz;   break;
      case 5: out1 = gelb_herz;    break;
      case 6: out1 = rot_herz;     break;
      case 7: out1 = orange_herz;  break;
      case 8: out1 = weiss_herz;   break;
      default: return; break;  
    } break;
    
    case 3: //angst
    switch(farbe) {
      case 1: out1 = blau_angst;   break;
      case 2: out1 = pink_angst;   break;
      case 3: out1 = gruen_angst;  break;
      case 4: out1 = hblau_angst;  break;
      case 5: out1 = gelb_angst;   break;
      case 6: out1 = rot_angst;    break;
      case 7: out1 = orange_angst; break;
      case 8: out1 = weiss_angst;  break;
      default: return; break;  
    } break;

  default: return; break;  
  }  
}
