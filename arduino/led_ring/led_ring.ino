/* Copyright (C) 2013 Eleonora Epp, SEMM                             */
/*-------------------------------------------------------------------*/ 
/* Projekt:          SEMM                                            */ 
/* Modul:            LED-Positionslicht                              */
/* Autor:            Eleonora Epp                                    */
/*                                                                   */
/* Dateiname:        led_ring.ino                                    */
/* Platform:         Aruino Nano V3.0                                */
/* Arduino Library:  SoftPWM.h                                       */
/*                                                                   */
/* Beschreibung:     Mit der Anleitung der I2C Befehle,              */
/*                   änderen die Leds ihre Farben und ihren Modus.   */


#include <SoftPWM.h>
#include <Wire.h>
#include "i2c_defs.h"
#include "farben.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define DIMMDOWN 300
#define DIMMUP 300

int blue = A0;
int green = A1;
int red = A2;
byte dimm = 0;

byte faktorRot = 100;
byte faktorGruen = 100;
byte faktorBlau = 100;

byte (*out)[2];    //Zeiger auf aktuelle Farbe/Modus 

void setup ()
{
  Serial.begin(9600); //x
  SoftPWMBegin();
 
  Wire.begin(I2C_LR_ADDR);
  Wire.onReceive(I2C_RECEICVE_INT);
  
  SoftPWMSet(red, 0);  //LED aus
  SoftPWMSet(green, 0);
  SoftPWMSet(blue, 0);
  
   //rampe auf = 300ms / rampe ab =  300ms
  SoftPWMSetFadeTime(red, DIMMUP, DIMMDOWN);   
  SoftPWMSetFadeTime(green, DIMMUP, DIMMDOWN);
  SoftPWMSetFadeTime(blue, DIMMUP, DIMMDOWN);
  
  out = aus; //aktuelle Farbe/Modus = aus 
}    


void loop ()
{
  
  float outRot, outGruen, outBlau;
  int i =0;
  
  if (dimm ==1)
  {
    for( int i = 1; i >= 0; i--) {
        outRot = out[RED][i] / 100 * faktorRot;//ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren        
        outGruen = out[GREEN][i] / 100 * faktorGruen;
        outBlau = out[BLUE][i] / 100 * faktorBlau;
    
        SoftPWMSet(red, (byte)outRot);//Ausgabewerte an PWM übergeben
        SoftPWMSet(green, (byte)outGruen);
        SoftPWMSet(blue, (byte)outBlau);
    
        if(i==1) delay (DIMMUP);
        else delay (DIMMDOWN);
        delay(500);//500ms Pause
      }
   }
   else
   {
    outRot = out[RED][i] / 100 * faktorRot; //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out[GREEN][i] / 100 * faktorGruen;
    outBlau = out[BLUE][i] / 100 * faktorBlau;

    SoftPWMSet(red, (byte)outRot);//Ausgabewerte an PWM übergeben
    SoftPWMSet(green, (byte)outGruen);
    SoftPWMSet(blue, (byte)outBlau);
    
    delay (100);
  }
}

void I2C_RECEICVE_INT(int howMany)//externer Zugriff für Stefan möglich
{
  
  Serial.println("I2C Event");
  char buffer[3];
  byte i = 0;
  
  while (Wire.available()) { //lese 3 Zeichen in Buffer
    buffer[i] = (char)Wire.read();
    i++;
    if (i > + 2) break;  //breche nach 3 Zeichen ab
  }
  
  while (Wire.available())   //leere input buffer
    (char)Wire.read();
    
  auswerten(buffer[0], buffer[1]);
}

void serialEvent() {
  Serial.println("serialEvent");
  char buffer[3];
  byte i = 0;
  
  while (Serial.available()) { //lese 3 Zeichen in Buffer
    buffer[i] = (char)Serial.read() - '0';
    i++;
    if (i > + 2) break;  //breche nach 3 Zeichen ab
  }
  while (Serial.available())   //leere input buffer
    (char)Serial.read();

  auswerten(buffer[0], buffer[1]);
}

void auswerten(int befehl, int farbe) {
 
  switch(befehl) //modus
  {
  case 0: //aus
    out = aus;
    Serial.println("aus");
    break;
    
  case 1: // an
    Serial.print("an ");
    switch(farbe) //farbe
    {
      case 1: // Marcel
        out = blau_an;
        Serial.println("blau");  
        break;
      case 2: // Martina 
        out = rosa_an;
        Serial.println("rosa");  
        break;
      case 3: // gruen
        out = gruen_an;
        Serial.println("gruen");  
        break;
      case 4: // Ela
        out = hellblau_an;
        Serial.println("hellblau");  
        break;
      case 5: // gelb
        out = gelb_an;
        Serial.println("gelb");  
        break;
      case 6: // rot
        out = rot_an;
        Serial.println("rot");  
        break;
      case 7: // orange
        out = orange_an;
        Serial.println("orange");  
        break;
      case 8: // Stefan 
        out = weiss_an;
        Serial.println("weiss");  
        break;
      default:
        Serial.println("Farbe ungueltig");
        break;  
    }
    break;
    
  case 2: //blinken
    Serial.print("blinken ");
    switch(farbe) //farbe
    {
      case 1: // Marcel 
        out = blau_blink;
        Serial.println("blau");  
        break;
      case 2: // Martina
        out = rosa_blink;
        Serial.println("rosa");  
        break;
      case 3: // gruen
        out = gruen_blink;
        Serial.println("gruen");  
        break;
      case 4: // Ela
        out = hellblau_blink;
        Serial.println("hellblau");  
        break;
      case 5: //gelb
        out = gelb_blink;
        Serial.println("gelb");  
        break;
      case 6: // rot
        out = rot_blink;
        Serial.println("rot");  
        break;
      case 7: // orange
        out = orange_blink;
        Serial.println("orange");  
        break;
      case 8: // Stefan 
        out = weiss_blink;
        Serial.println("weiss");  
        break;
      default:
        Serial.println("Farbe ungueltig");
        break;  
    }
    break;
  default:
    Serial.println("Befehl ungueltig");
    break;  
  }
 
}

