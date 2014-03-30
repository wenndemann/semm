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
#include <QueueList.h>
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


byte faktorRot = 100;  //LEDs sind 100% an
byte faktorGruen = 100;
byte faktorBlau = 100;

int (*out1)[6];    //Zeiger auf aktuelle Farbe/Modus (Zeiger auf zweidimensionales Feld)
int (*out2)[6];
int (*out3)[6];
int (*out4)[6];

struct Data {
  Data() { }
  Data(char b, char f, char e) : befehl(b), farbe(f), ecke(e) { }
  char befehl, farbe, ecke; 
};

QueueList<Data> data_queue;

static volatile bool mutex = false;

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
  
/*
  out1 = aus; //aktuelle Farbe/Modus = aus 
  out2 = aus; //aktuelle Farbe/Modus = aus 
  out3 = aus; //aktuelle Farbe/Modus = aus 
  out4 = aus; //aktuelle Farbe/Modus = aus 
*/
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
    
    outRot = out2[RED][i];        //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out2[GREEN][i];
    outBlau = out2[BLUE][i];
    SoftPWMSetFadeTime(redPin2, out2[rampeAuf][i], out2[rampeAb][i]);
    SoftPWMSetFadeTime(greenPin2, out2[rampeAuf][i], out2[rampeAb][i]);
    SoftPWMSetFadeTime(bluePin2, out2[rampeAuf][i], out2[rampeAb][i]);
    SoftPWMSet(redPin2, (byte)outRot);                  //Ausgabewerte an PWM übergeben
    SoftPWMSet(greenPin2, (byte)outGruen);
    SoftPWMSet(bluePin2, (byte)outBlau);
    
    outRot = out3[RED][i];        //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out3[GREEN][i] ;
    outBlau = out3[BLUE][i] ;
    SoftPWMSetFadeTime(redPin3, out3[rampeAuf][i], out3[rampeAb][i]);
    SoftPWMSetFadeTime(greenPin3, out3[rampeAuf][i], out3[rampeAb][i]);
    SoftPWMSetFadeTime(bluePin3, out3[rampeAuf][i], out3[rampeAb][i]);
    SoftPWMSet(redPin3, (byte)outRot);                  //Ausgabewerte an PWM übergeben
    SoftPWMSet(greenPin3, (byte)outGruen);
    SoftPWMSet(bluePin3, (byte)outBlau);
    
    outRot = out4[RED][i] ;        //ausgabe Werte aus Array laden und mit "Dimmfaktor" multiplizieren
    outGruen = out4[GREEN][i];
    outBlau = out4[BLUE][i] ;
    SoftPWMSetFadeTime(redPin4, out4[rampeAuf][i], out4[rampeAb][i]);
    SoftPWMSetFadeTime(greenPin4, out4[rampeAuf][i], out4[rampeAb][i]);
    SoftPWMSetFadeTime(bluePin4, out4[rampeAuf][i], out4[rampeAb][i]);
    SoftPWMSet(redPin4, (byte)outRot);                  //Ausgabewerte an PWM übergeben
    SoftPWMSet(greenPin4, (byte)outGruen);
    SoftPWMSet(bluePin4, (byte)outBlau);

    while(mutex) Serial.println("wait main");
    mutex = true;
    while(!data_queue.isEmpty()) {
      mutex = true;
      Data d = data_queue.pop();
      mutex = false;
      auswerten(d.befehl, d.farbe, d.ecke); 
    }
    mutex = false;
    delay(250);                                    //500ms Pause
  }
}

void I2C_RECEICVE_INT(int howMany)//externer Zugriff für Stefan möglich
{
  unsigned char b,f,e;
  
  delay(1);
  
  byte i = 0;
  if(howMany >= 3) {
    //while (Wire.available()) { //lese 3 Zeichen in Buffer
      b = (char)Wire.read();
      i++;
      f = (char)Wire.read();
      i++;
      e = (char)Wire.read();
      i++;
      //if (i >  3) break;  //breche nach 3 Zeichen ab
    //}
  }
  
  if(Wire.available()) {
    while (Wire.available())   //leere input buffer
      (char)Wire.read();
  }
  
  Serial.print("recieve ");
  Serial.println(i);

  
  //while(mutex) Serial.println("wait interrupt");
  //mutex = true;  
  if(i == 3) {
    auswerten(b,f,e);
  //  data_queue.push(Data(buffer[0], buffer[1], buffer[2] ) ); 
  }
  //mutex = false;
//  delay(1);
}


void serialEvent() { //x
  char buffer[3];
  byte i = 0;
  
  while (Serial.available()) { //lese 3 Zeichen in Buffer
    buffer[i] = (char)Serial.read() - '0'; //0-Zeichen im asci abziehen um wiwedr auf zahlen zu komen
    i++;
    if (i > 3) break;  //breche nach 3 Zeichen ab
  }
  while (Serial.available())   //leere input buffer
    (char)Serial.read();

  auswerten(buffer[0], buffer[1], buffer[2]);
}


void auswerten(unsigned char befehl,unsigned char farbe,unsigned char ecke) {
  int (**out)[6];
  
  switch(ecke)
  {
    case 1:
    Serial.print(" Ecke1 ");
      out = &out1;
      break;
    case 8:
      out = &out2;
    Serial.print(" Ecke2 ");
      break;
    case 4:
      out = &out3;
    Serial.print(" Ecke3 ");
      break;
    case 2:
      out = &out4;
    Serial.print(" Ecke4 ");
      break;
    default:
//      Serial.println("ecke unbekannt");
      return;
      break;
  }
  
  
  switch(befehl) //modus
  {
  case 0: //aus
    *out = aus;
  while(mutex);
    Serial.println("aus");
    break;
    
  case 1: // an
    Serial.print("an ");
    switch(farbe) //farbe
    {
      case 1: // Marcel
        *out = blau_an;
        Serial.println("blau");  
        break;
      case 2: // Martina 
        *out = pink_an;
      Serial.println("pink");  
        break;
      case 3: // gruen
        *out = gruen_an;
      Serial.println("gruen");  
        break;
      case 4: // Ela
        *out = hblau_an;
      Serial.println("hellblau");  
        break;
      case 5: // gelb
        *out = gelb_an;
      Serial.println("gelb");  
        break;
      case 6: // rot
        *out = rot_an;
      Serial.println("rot");  
        break;
      case 7: // orange
        *out = orange_an;
      Serial.println("orange");  
        break;
      case 8: // Stefan 
        *out = weiss_an;
      Serial.println("weiss");  
        break;
      default:
      Serial.println("Farbe ungueltig");
        return;
        break;  
    }
    break;
    
  case 2: //blinken
    Serial.print("herz");
    switch(farbe) //farbe
    {
      case 1: // Marcel 
        *out = blau_herz;
      Serial.println("blau");  
        break;
      case 2: // Martina
        *out = pink_herz;
      Serial.println("pink");  
        break;
      case 3: // gruen
        *out = gruen_herz;
      Serial.println("gruen");  
        break;
      case 4: // Ela
        *out = hblau_herz;
      Serial.println("hellblau");  
        break;
      case 5: //gelb
        *out = gelb_herz;
      Serial.println("gelb");  
        break;
      case 6: // rot
        *out = rot_herz;
      Serial.println("rot");  
        break;
      case 7: // orange
        *out = orange_herz;
      Serial.println("orange");  
        break;
      case 8: // Stefan 
        *out = weiss_herz;
     Serial.println("weiss");  
        break;
      default:
      Serial.println("Farbe ungueltig");
       return;
        break;  
    }
    break;
    
    
    case 3: //angst
  Serial.print("angst ");
    switch(farbe) //farbe
    {
      case 1: // Marcel 
        *out = blau_angst;
      Serial.println("blau");  
        break;
      case 2: // Martina
        *out = pink_angst;
      Serial.println("pink");  
        break;
      case 3: // gruen
        *out = gruen_angst;
      Serial.println("gruen");  
        break;
      case 4: // Ela
        *out = hblau_angst;
      Serial.println("hellblau");  
        break;
      case 5: //gelb
        *out = gelb_angst;
      Serial.println("gelb");  
        break;
      case 6: // rot
        *out = rot_angst;
      Serial.println("rot");  
        break;
      case 7: // orange
        *out = orange_angst;
      Serial.println("orange");  
        break;
      case 8: // Stefan 
        *out = weiss_angst;
      Serial.println("weiss");  
        break;
      default:
      Serial.println("Farbe ungueltig");
        return;
        break;  
    }
    break;

  default:
  Serial.println("Befehl ungueltig");
    return;
    break;  
  }
}

