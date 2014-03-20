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
/* Beschreibung: Steuert die zwei Getriebemotoren auf Befhele kommend von der */
/* I²C Schnittstelle                                                          */
/*----------------------------------------------------------------------------*/

//local includes
//#include "comm.h"
#include "data.h"
#include "i2c_defs.h"
#include "local_defs.h"
#include "Motor.h"
#include "Path.h"
#include "Pid.h"
#include "RotaryEncoder.h"

//global includes
#include <PinChangeInt.h>
#include <Streaming.h>
#include <TimerOne.h>
#include <Wire.h>
#include <avr/wdt.h>

void timer1Interrupt();
void rotaryEncoderInterrupt();
int smooth(int data, float filterVal, float smoothedVal);
void serialEvent();
void wireReceive(int length);
void wireRequest();
void sendToWire();
void sendToSerial();
void getFromWire();
void getFromSerial();

int dataPointer;
Data Data::d;
uint8_t comm = 0;
uint8_t reset = 0;

void setup() {
    //serial port
    Serial.begin(115200);
    Serial.println("Setup");

    //i2c
    Wire.begin(I2C_XYS_ADDR);
    Wire.onReceive(wireReceive);
    Wire.onRequest(wireRequest);

    //interrupts
    PCintPort::attachInterrupt(M0_RE_A_PIN, rotaryEncoderInterrupt, CHANGE);
    PCintPort::attachInterrupt(M0_RE_B_PIN, rotaryEncoderInterrupt, CHANGE);
    PCintPort::attachInterrupt(M1_RE_A_PIN, rotaryEncoderInterrupt, CHANGE);
    PCintPort::attachInterrupt(M1_RE_B_PIN, rotaryEncoderInterrupt, CHANGE);

    //init setpointSpeed for referencing
    Data::d.setpointSpeed[0] = -200.0f;
    Data::d.setpointSpeed[1] = -200.0f;

    //init servo
    Data::d.servo->setState(false);
    
    //init watchdog
    wdt_enable(WDTO_2S);  
}

//main loop
void loop() {
    static int mode = 0;
    if(!reset) wdt_reset();
    for(int i = 0; i < 2; i++) {
        if(Data::d.pidSpeed[i]->update(Data::d.smoothedSpeed[i], 
									   Data::d.setpointSpeed[i], 
									   &Data::d.output[i])) {
            if(Data::d.path->getRunning()) 
				Data::d.motor[i]->setOutput((int)Data::d.output[i]);
            else Data::d.motor[i]->setOutput(0);
				Data::d.smoothedSpeed[i] =  smooth(Data::d.rotaryEncoder[i]->getSpeed(), 
												   0.99, 
												   Data::d.smoothedSpeed[i]); 
        }
    }
    Serial << Data::d.rotaryEncoder[0]->getPos() << " " 
		   << Data::d.rotaryEncoder[1]->getPos() << endl;
    switch(mode) {
        case 0:
            if(!Data::d.motor[0]->isRef()) Data::d.setpointSpeed[0] = 0.0f;
            if(!Data::d.motor[1]->isRef()) Data::d.setpointSpeed[1] = 0.0f;
            if(Data::d.setpointSpeed[0] == 0.0f && Data::d.setpointSpeed[1] == 0.0f) {
                mode = 1;
                Data::d.rotaryEncoder[0]->setPos(-50.0f);
                Data::d.rotaryEncoder[1]->setPos(-270.0f);
                Data::d.path->newSetpoint(vec2d(0.0f, 0.0f), 
										  vec2d(Data::d.rotaryEncoder[0]->getPos(), 
										  Data::d.rotaryEncoder[1]->getPos()), 
										  0.0f);
            }
            break;
        case 1:
            Data::d.pidPos[0]->update(Data::d.rotaryEncoder[0]->getPos(), 
									  Data::d.setpointPath->x, 
									  &Data::d.setpointSpeed[0]);
            Data::d.pidPos[1]->update(Data::d.rotaryEncoder[1]->getPos(), 
									  Data::d.setpointPath->y, 
									  &Data::d.setpointSpeed[1]);
            Data::d.path->update(Data::d.setpointPath);
            break;
    }
    switch(comm) {
        case 1:
            Serial << "sendToWire" << endl;
            sendToWire();
            break;
        case 2:
            Serial << "sendToSerial" << endl;
            sendToSerial();
            break;
        case 3:
            Serial << "getFromWire" << endl;
            getFromWire();
            break;
        case 4:
            Serial << "getFromSerial" << endl;
            getFromSerial();
    }
}

void rotaryEncoderInterrupt() {
    for( int i = 0; i < 2; i++)
        Data::d.rotaryEncoder[i]->updatePos();
}

void timer1Interrupt() {
    Data::d.sendData = true;
}

int smooth(int data, float filterVal, float smoothedVal){
    if (filterVal > 1){      // check to make sure param's are within range
        filterVal = 1;
    }
    else if (filterVal <= 0){
        filterVal = 0;
    }
    smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

    return (int)smoothedVal;
}
void serialEvent() {
    static char b[2];

    delay(1);
    if(Serial.available()) {
        Serial.readBytes(b, 2);
        char inOut = b[0];
        dataPointer = b[1];
        if(inOut == 'r') {
            comm = 2;
        }
        else if(inOut == 'w') {
            comm = 4;
        }
    }
    while(Serial.available()) Serial.read();
}

void wireReceive(int length) {
    //delay(1);
    if(Wire.available()) {
        dataPointer = Wire.read();
        if(Wire.available()) comm = 3;
    }
}


void wireRequest() {
    //comm = 1;
    sendToWire();
}

void sendToWire() { // 1
    static char buf[32];
    int length = 0;
    switch(dataPointer) {
        case I2C_XYS_SETPOINTS:
            buf[0] = I2C_XYS_SETPOINTS;
            buf[1] = ' ';
            dtostrf(Data::d.setpointPath->getX(), 7, 2, buf+2);
            buf[9] = ' ';
            dtostrf(Data::d.setpointPath->getY(), 7, 2, buf+10);
            buf[17] = '\0';
            length = 18;
            break;

        case I2C_XYS_RUNNING:
            buf[0] = I2C_XYS_RUNNING;
            buf[1] = ' ';
            buf[2] = '0' + Data::d.path->getRunning();
            buf[3] = '\0';
            length = 4;
            break;
        default:
            comm = 0;
            return;
    }
    Wire << buf;
    comm = 0;
}


void sendToSerial() { // 2
    switch(dataPointer) {
        case I2C_XYS_SETPOINTS:
            Serial << I2C_XYS_SETPOINTS << " " << Data::d.setpointPath->getX() 
				   << " " <<  Data::d.setpointPath->getY();
            break;

        case I2C_XYS_RUNNING:
            Serial << I2C_XYS_RUNNING << " " << Data::d.path->getRunning();
            break;
            
        default:
            comm = 0;
            return;
    }
    comm = 0;
}

void getFromWire() { // 3
    switch(dataPointer) {
        case I2C_XYS_SETPOINTS: 
            {
                float fX, fY;
                fX = Wire.parseFloat();
                fY = Wire.parseFloat();
                Data::d.path->newSetpoint(vec2d(fX, fY), 
										  vec2d(Data::d.rotaryEncoder[0]->getPos(), 
										  Data::d.rotaryEncoder[1]->getPos()), 
										  0.0f);
            }
            break;

        case I2C_XYS_SERVO:
            {
                int i;
                i = Wire.parseInt();
                Data::d.servo->setState(i);
            }
            break;

        case I2C_XYS_RESET:
            {
                reset = Wire.parseInt();
                Serial << "reset=" << reset << endl;
            }
            break;
    }
    while(Wire.available()) Wire.read();
    comm = 0;
}
void getFromSerial() { // 4
    switch(dataPointer) {
        case I2C_XYS_SETPOINTS: 
            {
                float fX, fY;
                fX = Serial.parseFloat();
                fY = Serial.parseFloat();
                Data::d.path->newSetpoint(vec2d(fX, fY), 
				                          vec2d(Data::d.rotaryEncoder[0]->getPos(), 
										  Data::d.rotaryEncoder[1]->getPos()), 
										  0.0f);
            }
            break;

        case I2C_XYS_SERVO:
            {
                int i;
                i = Serial.parseInt();
                Data::d.servo->setState(i);
            }
            break;

        case I2C_XYS_RESET:
            {
                reset = Wire.parseInt();
            }
            break;
    }
    comm = 0;
}
