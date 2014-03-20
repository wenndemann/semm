/* Copyright (C) 2013 Marcel Gutmann, SEMM                                           */
/*-----------------------------------------------------------------------------------*/
/* Projekt:         SEMM                                                             */
/* Modul:           Display-Bedieneinheiten (DBEN)                                   */
/* Autor/en:        Marcel Gutmann (MG) / Martina Klein (MK)                         */
/*                                                                                   */
/* Dateiname:       programm.ino                                                     */
/* Version:         0.9.62                                                           */
/* Versionsstatus:  Experimentell                                                    */
/*                                                                                   */
/* Platform:        Arduino Uno                                                      */
/*                                                                                   */
/* Beschreibung:    Auf Anweisung mittels I2C werden Bildschirmmenüs angezeigt.      */
/*                  Ausgewählte GUI-Buttons werden auf Anfrage zurückgesandt.        */
/*-----------------------------------------------------------------------------------*/
//http://playground.arduino.cc/Main/WireLibraryDetailedReference

#define VERSION "DBEN V0.9.62 Exp"

#include <Encoder.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include "i2c_defs.h"
#include "displayDraw.h"

#define PUSH_TIME_LONG 2
#define PUSH_TIME_SHORT 1
#define ENCODER_TURN_NO 0
#define ENCODER_TURN_LEFT 1
#define ENCODER_TURN_RIGHT 2

volatile byte last_command = 0;
volatile byte menuCommand = I2C_DBEN_PIC_SEMM;
volatile boolean pushed_down = false; 
Encoder encoder(4, 3);

void setup(void) {
  initDisplay();  

  //Serial.begin(9600);

  Wire.begin(_i2c_adresse);              
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);

  attachInterrupt(0, setDown, FALLING);
  digitalWrite(2, HIGH); //PullUp fÃ¼r Drehencoder Press

}

void loop() {
  int pushTimeCount = 0;
  int pushLength = 0;
  static byte nRead = 0;

  //Auswertung der DrÃ¼ckdauer
  //pushed_down Ã¼ber Interrupt
  while (pushed_down)
  {
    if (pushTimeCount < 25)
    {
      pushLength = PUSH_TIME_SHORT;
    }
    else
    {
      //Lang gedrÃ¼ckt
      pushLength = PUSH_TIME_LONG;
      break;
    }
    delay(100);
    pushTimeCount++;
  }

  if (pushLength == PUSH_TIME_LONG) menuCommand = I2C_DBEN_PIC_CANCEL;

  drawPicture(menuCommand);

  if (pushLength == PUSH_TIME_SHORT) drawPushedButton();

  if (encoderRichtung() != ENCODER_TURN_NO)
  {
    drawButtonSelection();
    encoder.write(ENCODER_TURN_NO); //vorheriges drehen verwerfen
  }
}

void setDown()
{
  pushed_down = true;
  attachInterrupt(0, setUp, RISING);
}

void setUp()
{
  pushed_down = false;
  attachInterrupt(0, setDown, FALLING);
}

int encoderRichtung()
{
  int pos, retVal = ENCODER_TURN_NO;

  //Ein klick drehen incrementiert um 2
  pos = encoder.read();
  if (pos == 2)
  {
    retVal = ENCODER_TURN_LEFT;
  }

  if (pos == -2)
  {
    retVal = ENCODER_TURN_RIGHT;
  }

  return retVal;
}


String inputString = "";
void serialEvent() 
{
  while (Serial.available()) 
  {
    // get the new byte:
    // add it to the inputString:
    char inChar = (char)Serial.read(); 
 
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n' || inChar == '-') 
    {
      char temp[inputString.length() + 1];
      inputString.toCharArray(temp, sizeof(temp));
     
      byte received = strtol(temp, NULL, 16);
     
      switch (received)
      {
        case I2C_DBEN_PICTURE: //Anzeige Bild
        case I2C_DBEN_VERSION: //Ausgabe der Versionsnummer  
        case I2C_DBEN_BUTTON: //Buttonabfrage
          last_command = received;
          break;
        case I2C_DBEN_SHOW_CONFIG:
          drawInfo(VERSION); 
        default:
          if (last_command == I2C_DBEN_PICTURE)
          {
            menuCommand = received;
          }
          last_command = 0;
      }
 
      Serial.print("Befehl: ");
      Serial.println(received);
      inputString = "";
      
      switch (last_command)
      {
        case I2C_DBEN_VERSION:
          Serial.println(VERSION);
          break;
        case I2C_DBEN_BUTTON:
          Serial.println(getPushedButton());
          break;
      }
    }   
    else
      inputString += inChar;
  }
}
 
void receiveEvent(int howMany)
{   
  while(Wire.available()) 
  {
    byte received = Wire.read();

    switch (received)
    {
    case I2C_DBEN_PICTURE: //Anzeige Bild
    case I2C_DBEN_VERSION: //Ausgabe der Versionsnummer
    case I2C_DBEN_BUTTON: //Buttonabfrage
      last_command = received;
      break;
    case I2C_DBEN_SHOW_CONFIG:
      drawInfo(VERSION); 
    default:
      if (last_command == I2C_DBEN_PICTURE)
      {
        menuCommand = received;
      }
      last_command = 0;
    }
  }
}

void requestEvent()
{  
  switch (last_command)
  {
  case I2C_DBEN_VERSION:
    Wire.write(VERSION);
    break;
  case I2C_DBEN_BUTTON:
    Wire.write(getPushedButton());
    break;
  }
}







