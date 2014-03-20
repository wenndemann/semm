/* Copyright (C) 2013 Marcel Gutmann, SEMM                                           */
/*-----------------------------------------------------------------------------------*/
/* Projekt:         SEMM                                                             */
/* Modul:           RFID-Sensor (RFID)                                               */
/* Autor/en:        Marcel Gutmann (MG)                                              */
/*                                                                                   */          
/* Dateiname:       rfid_reader.ino                                                  */     
/* Version:         0.9.21b                                                          */
/* Versionsstatus:  Experimentell                                                    */  
/*                                                                                   */
/* Platform:        Arduino Nano / Atmega328                                         */
/*                                                                                   */
/* Beschreibung:    Liest auf Anfrage die 12x angeschlossenen RFID-Reader.           */ 
/*                  Ermittelte TAGs werden zurück an den Anfragenden                 */ 
/*                  gesendet. Die Kommunikation erfolgt über I2C.                    */ 
/*-----------------------------------------------------------------------------------*/


//80ms startcodeerkennung, erhöht von ursprünglich 40ms
//lesezeit gesamt variiert jetzt von 880ms bis 1210ms
//del command eingeführt
//startet mit lesen erst nach del befehl und löscht alle gespeicherten werte
//werte werden nur gesetzt, nicht mehr automatisch gelöscht
//nur zwei byte der Tags übertragen

#define VERSION "RFID V0.9.21b Exp"

#define NO_PIN_STATE        // to indicate that you don't need the pinState
#define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin
#define DISABLE_PCINT_MULTI_SERVICE // to limit the handler to servicing a single interrupt per invocation.

#define TOLERANZ_SHORT 100
#define TOLERANZ_LONG 100

#include "i2c_defs.h"
#include <Wire.h>
#include <PinChangeInt.h> //http://code.google.com/p/arduino-pinchangeint/
#include "Rfid.h"

//#define debug

Rfid rfid;

volatile byte data[10] = {0};
volatile boolean ready = false;
volatile boolean x = true;
volatile boolean s = false;
volatile boolean code_begin = false;
volatile byte block_length = 0; 
volatile byte block_no = 0;
volatile boolean parity = true;
volatile long ref_time_span_short = 0;
volatile byte last_command = 0;
volatile boolean do_run = false;

void setup()
{
  //Alle Reader aus
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  
  //Alle ReaderPorts auf Ausgang
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);

  #ifdef debug
    Serial.begin(9600);
    Serial.println("DEBUG: " VERSION);
  #endif
  
  Wire.begin(I2C_RFID_ADDR);                // I2C-Adresse 
  Wire.onReceive(receiveEvent); 
  Wire.onRequest(requestEvent);
}

void loop()
{
  while (do_run) oneRun();
//    PCintPort::attachInterrupt(2, onRFID, CHANGE);
//   
//    digitalWrite(3, LOW);
//    saveTAG(11);
//    digitalWrite(3, HIGH);
//    PCintPort::detachInterrupt(2);
}

void oneRun()
{
    PCintPort::attachInterrupt(2, onRFID, CHANGE);
    digitalWrite(3, LOW);
    saveTAG(0);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    saveTAG(1);
    digitalWrite(4, HIGH);
// schlechtes Leserverhalten wegen Platinenschaden. da nur 11 stk. gebraucht werden, wird diese momentan nicht verwendet
//    digitalWrite(5, LOW);
//    saveTAG(2);
//    digitalWrite(5, HIGH);
    PCintPort::detachInterrupt(2);

    PCintPort::attachInterrupt(11, onRFID, CHANGE);
    digitalWrite(6, LOW);
    saveTAG(2);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    saveTAG(3);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    saveTAG(4);
    digitalWrite(8, HIGH);
    PCintPort::detachInterrupt(11);

    PCintPort::attachInterrupt(12, onRFID, CHANGE);
    digitalWrite(9, LOW);
    saveTAG(5);
    digitalWrite(9, HIGH);
    digitalWrite(10, LOW);
    saveTAG(6);
    digitalWrite(10, HIGH);
    digitalWrite(13, LOW);
    saveTAG(7);
    digitalWrite(13, HIGH);
    PCintPort::detachInterrupt(12);

    PCintPort::attachInterrupt(A2, onRFID, CHANGE);
    digitalWrite(A3, LOW);
    saveTAG(8);
    digitalWrite(A3, HIGH);
    digitalWrite(A0, LOW);
    saveTAG(9);
    digitalWrite(A0, HIGH);
    digitalWrite(A1, LOW);
    saveTAG(10);
    digitalWrite(A1, HIGH);
    PCintPort::detachInterrupt(A2);
}

void saveTAG(byte Kanal)
{
  byte buffer[5];
  //gib ausreichend zeit, um startbits zu erkennen
  ////Falls Startbits erkannt, dann verlängere die Wartezeit
  reInit();
  delay(80);
  if (code_begin) delay(30);

  if (ready)
  {
    rfid.setTAG(Kanal, ((data[0] << 4) | data[1]), 
      ((data[2] << 4) | data[3]), 
      ((data[4] << 4) | data[5]), 
      ((data[6] << 4) | data[7]), 
      ((data[8] << 4) | data[9]));  
    
    #ifdef debug
      rfid.getTAGData(Kanal, buffer);
      Serial.print(Kanal);
      Serial.print(": ");
      for (int i = 0; i < 5; i++)
      {
        Serial.print(buffer[i]);
        if (!(i + 1 % 5))
          Serial.println();
        else
          Serial.print("-");
      }
      Serial.print(" >> ");
      Serial.println(ref_time_span_short);
    #endif

    reInit();
  }
}

void onRFID() //ausgelöst durch pinchange interrupt
{
  unsigned long cur_time_span;
  static unsigned long oldTime = 0;
  static unsigned long old_time_span = 0;
  static unsigned long ref_time_span_long;
  
  if (!ready) // wenn datensatz nicht freigegeben, dann kein gültiger/vollständiger vorhanden, also bereit zum aufnehmen
  {
    cur_time_span = micros() - oldTime;
    oldTime = micros();      
 
    if (!code_begin)
    {
      //ShortTimeDuration (Clock) ermitteln/synchronisieren
      if (cur_time_span < 800 && cur_time_span > 200 && cur_time_span > (old_time_span - TOLERANZ_SHORT) && cur_time_span < (old_time_span + TOLERANZ_SHORT)) 
      {
        //ShortTimeDuration (Clock) mitteln
        ref_time_span_short += cur_time_span;
        //StartPattern erkennen und dann flag setzen
        block_length++;

        //Eigentlich 16 impulse zur erkennung der 9 Startbits nötig, der erste wird aufgrund der Programmierung und Syncronisierung aber nicht mitgezäht
        //Das erste Bit der 9 entfällt ebenfalls aufgrund der programmierung
        if (block_length == 15)  
        {        
          code_begin = true;
          //Mittelwert bilden
          ref_time_span_short /= block_length;
          ref_time_span_long = ref_time_span_short * 2;  
          block_length = 0;
        }
      }
      else
      {
        reInit();
      }
    }
    else //erkennung der daten
    {
      if (cur_time_span > (ref_time_span_short - TOLERANZ_SHORT) && cur_time_span < (ref_time_span_short + TOLERANZ_SHORT)) // Fall 1cur_time_span
      {
        if (s)
        {
          setData();
        }
        s = !s;
      }
      else if (cur_time_span > (ref_time_span_long - TOLERANZ_LONG) && cur_time_span < (ref_time_span_long + TOLERANZ_LONG)) // Fall 2
      {
        if (!s)
        {
          x = !x; // Wechseln von x
          setData();
        }
        else // Fehlerfall, wenn Status besagt in Kurzschleife, ein kurzer Impuls erwartet wird, aber ein langer impuls erfolgt. Signalstörung
        {
          reInit();
        }
      }
      else // Fehlerfall, wenn Fall eins bis zwei nicht eingetroffen. Signalstörung
      {
        reInit();
      }
    } //if (code_begin) [...] else: 
  } //if (!ready):
  old_time_span = cur_time_span; 
}

void reInit()
{
  // Bei Fehlerfällen oder wenn der Code vollständig empfangen wurde, dann neu Initialisieren
  block_length = 0;
  block_no = 0;
  ref_time_span_short = 0;
  x = true;
  s = false;
  code_begin = false;
  parity = true;
  ready = false;

  for (int i = 0; i < 10; i++)
  {
    data[i] = 0;
  }
}

void setData()
{      
  block_length++;
  //Bit-Shift um Platz für neues Bit zu schaffen
  data[block_no] <<= 0x01;
  if (x) // Wenn X = false dann nichts weiter, da bereits Bit-Shift erfolgt und damit eh 0-Wert eingetragen
  {
    data[block_no] |= 0x01;
    
    parity = !parity; //Da auch beim letzten paritätsbit die Variable gewechselt wird, ist die Variable immer true, wenn der Wert gültig ist
    // DDDDP  START:
    // 10010 = TRUE: FALSE > TRUE >> Wert gütig, Variable Parity gesetzt
    // 00000 = TRUE: >> Wert gültig, Variable Parity gesetzt
    // 01111 = TRUE: FALSE > TRUE > FALSE > TRUE >> Wert gültig, Variable Parity gesetzt
    // 01101 = TRUE: FASE > TRUE > FALSE >> Wert ungültig, Variable parity nicht gesetzt
  }
  
  if (block_length == 5)
  {
    //Parity prüfen
    if (parity)
    {    
      block_length = 0;
      
      //Wert ohne Parity speichern
      data[block_no] >>= 1;
      block_no++;
      //Der 11. Block ist ein ParityBlock und wird hier nicht ausgewertet
      //Auswertung komplex und verbraucht Zeit. Da Interrupt, wird dieser teil gespart
      if (block_no == 10)
      {
        ready = true;
      }
    }
    else
    {
      reInit();
    }    
  }
}

void receiveEvent(int howMany)
{   
  while(Wire.available()) 
  {
    byte received = Wire.read();

    switch (received)
    {
      case I2C_RFID_IREAD:
        rfid.delTAGs();
        do_run = true;
      case I2C_RFID_TAGS: //Ausgabe TAGS (4 x 11 Bytes)
      case I2C_RFID_VERSION: //Ausgabe der Versionsnummer
        last_command = received;
        break;
      default:
        last_command = 0;
    }
  }
}

void requestEvent()
{  
  byte buffer[22];

  if (last_command == I2C_RFID_VERSION)
  {
    Wire.write(VERSION);
  }
  
  if (last_command == I2C_RFID_TAGS)
  {
    for (byte i = 0; i < 11; i++)
    {
      rfid.getTAGData(i, &buffer[i*2]);
    }
    Wire.write(buffer, 22);  
    do_run = false; 
  }
  last_command = 0;
}
