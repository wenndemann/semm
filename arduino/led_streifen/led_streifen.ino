// Befehl 

#include <SoftPWM.h>
#include <Wire.h>
#include "i2c_defs.h"

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

int herz[6] = {255,255,0,0,0,0}; // Impulslänge Herzschlag
int angst[6] = {255,0,0,255,0,0}; // Impulslänge für den schnellen Angst-Herzschlag
int dim0[6] = {0,0,0,0,0,0};
int dim2h[6] = {2,2,0,0,0,0};
int dim2a[6] = {2,0,0,2,0,0};
int dim10h[6] = {10,10,5,5,1,1};
int dim10a[6] = {10,5,1,10,5,1};
int dim200h[6] = {200,200,0,0,0,0};
int dim200a[6] = {200,0,0,200,0,0};

int befehl = 0; //x
int i;



void setup ()  // zu Beginn, wenn noch kein Befehl getätigt wurde, sind folgende LEDs an: //
{
  Serial.begin(9600); //x
  SoftPWMBegin();
  //**Wire.begin();
  Wire.begin(I2C_LS_ADDR);      // join i2c bus with address I2C_LS_ADDR  weiß grau schwarz rot braun
  Wire.onReceive(receiveEvent); // register eventCEIVE_INT); // register event
  Serial.begin(9600);           // start serial for output

  
}    
//~~~~~~~~~~~~muss die unter "loop" ?? ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(Wire.available()) // loop through all but the last  ??~~~~~~~~~~~~~~~~~~
  {
    befehl = Wire.read(); // receive byte as a character~~~~~~~~~~~~~~~hier müsste doch "befehl" rein ja
    Serial.println(befehl);         // print the character ~~~~~~~~~~~~~~~~hier auch~~~~~~~~~~~~
  }
}


void loop () 
{
  /*  aus dem ersten Video http://www.youtube.com/watch?v=GJX0BRUagCg  
  // wenn man Master ist
  //Start Talking
  Wire.beginTransmission()//Adresse vom Rasperry ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Ask for Register zero
  Wire.send(0);
  //Complete Transmission
  Wire.endTransmission();
  //Request 1 byte
  Wire.requestFrom(); //Adresse vom Rasperry und ",1"
   //wait for response
   while(Wire.available() == 0);
   // get the information // Ich denke, hier kommt der "Befehl" an
   befehl = Wire.receive();
   //print the results // Ausgabe von dem was angekommen ist:
   Serial.print(befehl);
   Serial.print("Befehl,");
   */ // Wenn ich der Master wäre ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
   delay(500); //nach 500us wird noch mal "abgefragt"  
  
  for(i=0;i<6;i++) 
  {  
    switch(befehl)
    {
      case 1:    // alle aus ***************************************************************
       SoftPWMSet(redPin1, 0);    // Ecke 1  Stefan
        SoftPWMSet(greenPin1, 0);
        SoftPWMSet(bluePin1, 0);
        
        SoftPWMSet(redPin2, 0);      // Ecke 2 Ela
        SoftPWMSet(greenPin2, 0);
        SoftPWMSet(bluePin2, 0);
        
        SoftPWMSet(redPin3, 0);    // Ecke 3 Marcel
        SoftPWMSet(greenPin3, 0);
        SoftPWMSet(bluePin3, 0);
      
        SoftPWMSet(redPin4, 0);    // Ecke 4 Martina
        SoftPWMSet(greenPin4, 0);
        SoftPWMSet(bluePin4, 0);
        break;
     //***********************************************************************************
      case 2:   //alle in der eigenen Farbe AN ******************************************
        SoftPWMSet(redPin1, 255);    // Ecke 1 weiß  Stefan
        SoftPWMSet(greenPin1, 255);
        SoftPWMSet(bluePin1, 255);
            SoftPWMSet(redPin2, 2);      // Ecke 2 hellblau Ela 
            SoftPWMSet(greenPin2, 200);
            SoftPWMSet(bluePin2, 255);
        SoftPWMSet(redPin3, 2);    // Ecke 3 dunkelblau Marcel
        SoftPWMSet(greenPin3, 2);
        SoftPWMSet(bluePin3, 255);
            SoftPWMSet(redPin4, 255);    // Ecke 4 pink Martina
            SoftPWMSet(greenPin4, 0);
            SoftPWMSet(bluePin4, 10);
        break;
     //***********************************************************************************   
     case 3:     // nur Ecke 1 an *****************************************************
        SoftPWMSet(redPin1, 255);    // Ecke 1  Stefan
        SoftPWMSet(greenPin1, 255);
        SoftPWMSet(bluePin1, 255);
        break;
     case 4:     // nur Ecke 2 an **********************************************************  
        SoftPWMSet(redPin2, 2);      // Ecke 2 Ela
        SoftPWMSet(greenPin2, 200);
        SoftPWMSet(bluePin2, 255);
        break;
     case 5:     // nur Ecke 3 an **********************************************************  
        SoftPWMSet(redPin3, 2);    // Ecke 3 Marcel
        SoftPWMSet(greenPin3, 2);
        SoftPWMSet(bluePin3, 255);
        break;
      case 6:     // nur Ecke 4 an **********************************************************  
         SoftPWMSet(redPin4, 255);    // Ecke 4 Martina
         SoftPWMSet(greenPin4, 0);
         SoftPWMSet(bluePin4, 10);
         break;
      //***********************************************************************************
      case 7:    // alle Herzschlag *******************************************************
        SoftPWMSetFadeTime(redPin1, 500, 1000);   // Ecke 1  Stefan~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin1, 500, 1000);
        SoftPWMSetFadeTime(bluePin1, 500, 1000);
        SoftPWMSet(redPin1, herz[i]);
        SoftPWMSet(greenPin1, herz[i]);   
        SoftPWMSet(bluePin1, herz[i]);   

        SoftPWMSetFadeTime(redPin2, 500, 1000);    // Ecke 2  Ela ~~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin2, 500, 1000);
        SoftPWMSetFadeTime(bluePin2, 500, 1000);
        SoftPWMSet(redPin2,dim2h[i]);
        SoftPWMSet(greenPin2, dim200h[i]);   
        SoftPWMSet(bluePin2, herz[i]);   
        
        SoftPWMSetFadeTime(redPin3, 500, 1000); // Ecke 3 Marcel ~~~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin3, 500, 1000);
        SoftPWMSetFadeTime(bluePin3, 500, 1000);
        SoftPWMSet(redPin3, dim2h[i]);
        SoftPWMSet(greenPin3, dim2h[i]);   
        SoftPWMSet(bluePin3, herz[i]);   
       
        SoftPWMSetFadeTime(redPin4, 500, 1000); // Ecke 4 Martina~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin4, 500, 1000);
        SoftPWMSetFadeTime(bluePin4, 500, 4000);
        SoftPWMSet(redPin4, herz[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10h[i]);   
        
        break;
        //***********************************************************************************
        case 8:
        SoftPWMSetFadeTime(redPin1, 500, 1000);  // Ecke 1  Stefan~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin1, 500, 1000);
        SoftPWMSetFadeTime(bluePin1, 500, 1000);
        SoftPWMSet(redPin1, herz[i]);
        SoftPWMSet(greenPin1, herz[i]);   
        SoftPWMSet(bluePin1, herz[i]);   
        break;
        //***********************************************************************************
        case 9: 
        SoftPWMSetFadeTime(redPin2, 500, 1000); // Ecke 2  Ela ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin2, 500, 1000);
        SoftPWMSetFadeTime(bluePin2, 500, 1000);
        SoftPWMSet(redPin2, dim2h[i]);
        SoftPWMSet(greenPin2, dim200h[i]);   
        SoftPWMSet(bluePin2, herz[i]);
        break;   
        //***********************************************************************************
        case 10:
        SoftPWMSetFadeTime(redPin3, 500, 1000);  // Ecke 3 Marcel ~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin3, 500, 1000);
        SoftPWMSetFadeTime(bluePin3, 500, 1000);
        SoftPWMSet(redPin3, dim2h[i]);
        SoftPWMSet(greenPin3, dim2h[i]);   
        SoftPWMSet(bluePin3, herz[i]);   
        break;
        //***********************************************************************************
        case 11:
        SoftPWMSetFadeTime(redPin4, 500, 1000); // Ecke 4 Martina~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SoftPWMSetFadeTime(greenPin4, 500, 1000);
        SoftPWMSetFadeTime(bluePin4, 500, 1000);
        SoftPWMSet(redPin4, herz[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10h[i]);   
        break;
        //***********************************************************************************       
        //Ecke 1 blinkt im Herzschlag   Ecke 2 blinkt im Angstmodus
        case 12: 
        SoftPWMSetFadeTime(redPin1, 500, 1000);   // Ecke 1  Herz
        SoftPWMSetFadeTime(greenPin1, 500, 1000);
        SoftPWMSetFadeTime(bluePin1, 500, 1000);
        SoftPWMSet(redPin1, herz[i]);
        SoftPWMSet(greenPin1, herz[i]);   
        SoftPWMSet(bluePin1, herz[i]);   
        
        SoftPWMSetFadeTime(redPin2, 250, 500); // Ecke 2  Angst
        SoftPWMSetFadeTime(greenPin2, 250, 500);
        SoftPWMSetFadeTime(bluePin2, 250, 500);
        SoftPWMSet(redPin2, dim2a[i]);
        SoftPWMSet(greenPin2, dim200a[i]);   
        SoftPWMSet(bluePin2, angst[i]); 
        break;
        //************************************************************************************
        case 13:
        SoftPWMSetFadeTime(redPin2, 500, 1000); // Ecke 2 Herz
        SoftPWMSetFadeTime(greenPin2, 500, 1000);
        SoftPWMSetFadeTime(bluePin2, 500, 1000);
        SoftPWMSet(redPin2, dim2h[i]);
        SoftPWMSet(greenPin2, dim200h[i]);   
        SoftPWMSet(bluePin2, herz[i]);

        SoftPWMSetFadeTime(redPin1, 250, 500);   // Ecke 1 Angst
        SoftPWMSetFadeTime(greenPin1, 250, 500);
        SoftPWMSetFadeTime(bluePin1, 250, 500);
        SoftPWMSet(redPin1, angst[i]);
        SoftPWMSet(greenPin1, angst[i]);   
        SoftPWMSet(bluePin1, angst[i]);   
        break;
        //***********************************************************************************
        case 14:
        SoftPWMSetFadeTime(redPin1, 500, 1000);   // Ecke 1 Herz
        SoftPWMSetFadeTime(greenPin1, 500, 1000);
        SoftPWMSetFadeTime(bluePin1, 500, 1000);
        SoftPWMSet(redPin1, herz[i]);
        SoftPWMSet(greenPin1, herz[i]);   
        SoftPWMSet(bluePin1, herz[i]);   
  
        SoftPWMSetFadeTime(redPin3, 250, 500);  // Ecke 3 Angst
        SoftPWMSetFadeTime(greenPin3, 250, 500);
        SoftPWMSetFadeTime(bluePin3, 250, 500);
        SoftPWMSet(redPin3, dim2a[i]);
        SoftPWMSet(greenPin3, dim2a[i]);   
        SoftPWMSet(bluePin3, angst[i]);   
        break;
        //***********************************************************************************
        case 15:
        SoftPWMSetFadeTime(redPin3, 500, 1000);  // Ecke 3 Herz
        SoftPWMSetFadeTime(greenPin3, 500, 1000);
        SoftPWMSetFadeTime(bluePin3, 500, 1000);
        SoftPWMSet(redPin3, dim2h[i]);
        SoftPWMSet(greenPin3, dim2h[i]);   
        SoftPWMSet(bluePin3, herz[i]);   
        
        SoftPWMSetFadeTime(redPin1, 250, 500);   // Ecke 1 Angst
        SoftPWMSetFadeTime(greenPin1, 250, 500);
        SoftPWMSetFadeTime(bluePin1, 250, 500);
        SoftPWMSet(redPin1, angst[i]);
        SoftPWMSet(greenPin1, angst[i]);   
        SoftPWMSet(bluePin1, angst[i]); 
        break;  
        //***********************************************************************************
        case 16:
        SoftPWMSetFadeTime(redPin1, 500, 1000);   // Ecke 1 Herz
        SoftPWMSetFadeTime(greenPin1, 500, 1000);
        SoftPWMSetFadeTime(bluePin1, 500, 1000);
        SoftPWMSet(redPin1, herz[i]);
        SoftPWMSet(greenPin1, herz[i]);   
        SoftPWMSet(bluePin1, herz[i]);   

        SoftPWMSetFadeTime(redPin4, 250, 500); // Ecke 4 Angst
        SoftPWMSetFadeTime(greenPin4, 250, 500);
        SoftPWMSetFadeTime(bluePin4, 250, 500);
        SoftPWMSet(redPin4, angst[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10a[i]); 
        break;  
        //***********************************************************************************
        case 17:
        SoftPWMSetFadeTime(redPin4, 500, 1000); // Ecke 4 Herz
        SoftPWMSetFadeTime(greenPin4, 500, 1000);
        SoftPWMSetFadeTime(bluePin4, 500, 1000);
        SoftPWMSet(redPin4, herz[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10h[i]);   

        SoftPWMSetFadeTime(redPin1, 250, 500);   // Ecke 1 Angst
        SoftPWMSetFadeTime(greenPin1, 250, 500);
        SoftPWMSetFadeTime(bluePin1, 250, 500);
        SoftPWMSet(redPin1, angst[i]);
        SoftPWMSet(greenPin1, angst[i]);   
        SoftPWMSet(bluePin1, angst[i]);
        break;     
        //***********************************************************************************
        case 18:
        SoftPWMSetFadeTime(redPin2, 500, 1000); // Ecke 2 Herz
        SoftPWMSetFadeTime(greenPin2, 500, 1000);
        SoftPWMSetFadeTime(bluePin2, 500, 1000);
        SoftPWMSet(redPin2, dim2h[i]);
        SoftPWMSet(greenPin2, dim200h[i]);   
        SoftPWMSet(bluePin2, herz[i]);
        
        SoftPWMSetFadeTime(redPin3, 250, 500);  // Ecke 3 Angst
        SoftPWMSetFadeTime(greenPin3, 250, 500);
        SoftPWMSetFadeTime(bluePin3, 250, 500);
        SoftPWMSet(redPin3, dim2a[i]);
        SoftPWMSet(greenPin3, dim2a[i]);   
        SoftPWMSet(bluePin3, angst[i]);   
        break;  
        //***********************************************************************************
        case 19:
        SoftPWMSetFadeTime(redPin3, 500, 1000);  // Ecke 3 Herz
        SoftPWMSetFadeTime(greenPin3, 500, 1000);
        SoftPWMSetFadeTime(bluePin3, 500, 1000);
        SoftPWMSet(redPin3, dim2h[i]);
        SoftPWMSet(greenPin3, dim2h[i]);   
        SoftPWMSet(bluePin3, herz[i]);   
        
        SoftPWMSetFadeTime(redPin2, 250,500); // Ecke 2 Angst
        SoftPWMSetFadeTime(greenPin2, 250,500);
        SoftPWMSetFadeTime(bluePin2, 250,500);
        SoftPWMSet(redPin2, dim2a[i]);
        SoftPWMSet(greenPin2, dim200a[i]);   
        SoftPWMSet(bluePin2, angst[i]);
        break;  
        //***********************************************************************************
        case 20:
        SoftPWMSetFadeTime(redPin2, 500, 1000); // Ecke 2 Herz
        SoftPWMSetFadeTime(greenPin2, 500, 1000);
        SoftPWMSetFadeTime(bluePin2, 500, 1000);
        SoftPWMSet(redPin2, dim2h[i]);
        SoftPWMSet(greenPin2, dim200h[i]);   
        SoftPWMSet(bluePin2, herz[i]);
        
        SoftPWMSetFadeTime(redPin4, 250, 500); // Ecke 4 Angst
        SoftPWMSetFadeTime(greenPin4, 250, 500);
        SoftPWMSetFadeTime(bluePin4, 250, 500);
        SoftPWMSet(redPin4, angst[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10a[i]); 
        break;    
        //***********************************************************************************
        case 21:
        SoftPWMSetFadeTime(redPin4, 500, 1000); // Ecke 4 Herz
        SoftPWMSetFadeTime(greenPin4, 500, 1000);
        SoftPWMSetFadeTime(bluePin4, 500, 1000);
        SoftPWMSet(redPin4, herz[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10h[i]);   
        
        SoftPWMSetFadeTime(redPin2, 250,500); // Ecke 2 Angst
        SoftPWMSetFadeTime(greenPin2, 250,500);
        SoftPWMSetFadeTime(bluePin2, 250,500);
        SoftPWMSet(redPin2, dim2a[i]);
        SoftPWMSet(greenPin2, dim200a[i]);   
        SoftPWMSet(bluePin2, angst[i]);
        break;  
        //***********************************************************************************
        case 22:
        SoftPWMSetFadeTime(redPin3, 500, 1000);  // Ecke 3 Herz
        SoftPWMSetFadeTime(greenPin3, 500, 1000);
        SoftPWMSetFadeTime(bluePin3, 500, 1000);
        SoftPWMSet(redPin3, dim2h[i]);
        SoftPWMSet(greenPin3, dim2h[i]);   
        SoftPWMSet(bluePin3, herz[i]);
       
        SoftPWMSetFadeTime(redPin4, 250, 500); // Ecke 4 Angst
        SoftPWMSetFadeTime(greenPin4, 250, 500);
        SoftPWMSetFadeTime(bluePin4, 250, 500);
        SoftPWMSet(redPin4, angst[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10a[i]);
        break;  
        //***********************************************************************************
        case 23:
        SoftPWMSetFadeTime(redPin4, 500, 1000); // Ecke 4 Herz
        SoftPWMSetFadeTime(greenPin4, 500, 1000);
        SoftPWMSetFadeTime(bluePin4, 500, 1000);
        SoftPWMSet(redPin4, herz[i]);
        SoftPWMSet(greenPin4, dim0[i]);   
        SoftPWMSet(bluePin4, dim10h[i]);   
        
        SoftPWMSetFadeTime(redPin3, 250, 500);  // Ecke 3 Angst
        SoftPWMSetFadeTime(greenPin3, 250, 500);
        SoftPWMSetFadeTime(bluePin3, 250, 500);
        SoftPWMSet(redPin3, dim2a[i]);
        SoftPWMSet(greenPin3, dim2a[i]);   
        SoftPWMSet(bluePin3, angst[i]);   
        break;  
        
        // *********************************************************************************
        // diese cases sollen im 1 Sekunde - delay ablaufen. NICHT 250us        
        //***********************************************************************************
        case 24:   // SEMM-Farben im Wechsel  //weiß, pink, hellblau, dunkelblau  
        SoftPWMSet(redPin1, 255);  // Ecke 1 weiß
        SoftPWMSet(greenPin1, 255);   
        SoftPWMSet(bluePin1, 255);
          SoftPWMSet(redPin2, 1);   // Ecke 2  hellblau
          SoftPWMSet(greenPin2, 200);   
          SoftPWMSet(bluePin2, 255);
            SoftPWMSet(redPin3, 2);   // Ecke 3  dunkelblau
            SoftPWMSet(greenPin3, 2);   
            SoftPWMSet(bluePin3, 255);   
              SoftPWMSet(redPin4, 255); // Ecke 4  pink
              SoftPWMSet(greenPin4, 0);
              SoftPWMSet(bluePin4, 10);
        delay(1000);
        SoftPWMSet(redPin2, 255);  // Ecke 2 weiß
        SoftPWMSet(greenPin2, 255);   
        SoftPWMSet(bluePin2, 255);
          SoftPWMSet(redPin3, 1);   //  Ecke 1 hellblau
          SoftPWMSet(greenPin3, 200);   
          SoftPWMSet(bluePin3, 255);
            SoftPWMSet(redPin4, 2);   // Ecke 2 dunkelblau
            SoftPWMSet(greenPin4, 2);   
            SoftPWMSet(bluePin4, 255);   
              SoftPWMSet(redPin1, 255); // Ecke 1 pink
              SoftPWMSet(greenPin1, 0);
              SoftPWMSet(bluePin1, 10);
        delay(1000);
        SoftPWMSet(redPin3, 255);  // Ecke 3 weiß
        SoftPWMSet(greenPin3, 255);   
        SoftPWMSet(bluePin3, 255);
          SoftPWMSet(redPin4, 1);   // Ecke 4  hellblau
          SoftPWMSet(greenPin4, 200);   
          SoftPWMSet(bluePin4, 255);
            SoftPWMSet(redPin1, 2);   //  Ecke 1 dunkelblau
            SoftPWMSet(greenPin1, 2);   
            SoftPWMSet(bluePin1, 255);   
              SoftPWMSet(redPin2, 255); // Ecke 2 pink
              SoftPWMSet(greenPin2, 0);
              SoftPWMSet(bluePin2, 10);
        delay(1000);
        SoftPWMSet(redPin4, 255);  // Ecke 4  weiß
        SoftPWMSet(greenPin4, 255);   
        SoftPWMSet(bluePin4, 255);
          SoftPWMSet(redPin1, 1);   // Ecke 1 hellblau
          SoftPWMSet(greenPin1, 200);   
          SoftPWMSet(bluePin1, 255);
            SoftPWMSet(redPin2, 2);   // Ecke 2 dunkelblau
            SoftPWMSet(greenPin2, 2);   
            SoftPWMSet(bluePin2, 255);   
              SoftPWMSet(redPin3, 255); // Ecke 3 pink
              SoftPWMSet(greenPin3, 0);
              SoftPWMSet(bluePin3, 10);
         delay(1000);
        break;  
        //***********************************************************************************
        case 25: // Rottöne im Wechsel: rot, orange, gelb, pink
        SoftPWMSet(redPin1, 255);  // Ecke 1 rot
        SoftPWMSet(greenPin1, 0);   
        SoftPWMSet(bluePin1, 0);
          SoftPWMSet(redPin2, 255);   // Ecke 2  orange
          SoftPWMSet(greenPin2, 40);   
          SoftPWMSet(bluePin2, 0);
            SoftPWMSet(redPin3, 255);   // Ecke 3  gelb
            SoftPWMSet(greenPin3, 100);   
            SoftPWMSet(bluePin3, 0);   
              SoftPWMSet(redPin4, 255); // Ecke 4  pink
              SoftPWMSet(greenPin4, 0);
              SoftPWMSet(bluePin4, 10);
        delay(1000);
        SoftPWMSet(redPin2, 255);  // Ecke 2 rot
        SoftPWMSet(greenPin2, 0);   
        SoftPWMSet(bluePin2, 0);
          SoftPWMSet(redPin3, 255);   // Ecke 3  orange
          SoftPWMSet(greenPin3, 40);   
          SoftPWMSet(bluePin3, 0);
            SoftPWMSet(redPin4, 255);   // Ecke 4  gelb
            SoftPWMSet(greenPin4, 100);   
            SoftPWMSet(bluePin4, 0);   
              SoftPWMSet(redPin1, 255); // Ecke 1  pink
              SoftPWMSet(greenPin1, 0);
              SoftPWMSet(bluePin1, 10);
        delay(1000);
        SoftPWMSet(redPin3, 255);  // Ecke 3 rot
        SoftPWMSet(greenPin3, 0);   
        SoftPWMSet(bluePin3, 0);
          SoftPWMSet(redPin4, 255);   // Ecke 4  orange
          SoftPWMSet(greenPin4, 40);   
          SoftPWMSet(bluePin4, 0);
            SoftPWMSet(redPin1, 255);   // Ecke 1  gelb
            SoftPWMSet(greenPin1, 100);   
            SoftPWMSet(bluePin1, 0);   
              SoftPWMSet(redPin2, 255); // Ecke 2  pink
              SoftPWMSet(greenPin2, 0);
              SoftPWMSet(bluePin2, 10);
        delay(1000);
        SoftPWMSet(redPin4, 255);  // Ecke 4 rot
        SoftPWMSet(greenPin4, 0);   
        SoftPWMSet(bluePin4, 0);
          SoftPWMSet(redPin1, 255);   // Ecke 1  orange
          SoftPWMSet(greenPin1, 40);   
          SoftPWMSet(bluePin1, 0);
            SoftPWMSet(redPin2, 255);   // Ecke 2 gelb
            SoftPWMSet(greenPin2, 100);   
            SoftPWMSet(bluePin2, 0);   
              SoftPWMSet(redPin3, 255); // Ecke 3  pink
              SoftPWMSet(greenPin3, 0);
              SoftPWMSet(bluePin3, 10);
         delay(1000);
        break;  
        //***********************************************************************************
        case 26: // blau-grün im Wechsel: blau, grün, hellblau, türkis
        SoftPWMSet(redPin1, 0);  // Ecke 1 blau
        SoftPWMSet(greenPin1, 0);   
        SoftPWMSet(bluePin1, 255);
          SoftPWMSet(redPin2, 0);   // Ecke 2  grün
          SoftPWMSet(greenPin2, 255);   
          SoftPWMSet(bluePin2, 0);
            SoftPWMSet(redPin3, 1);   // Ecke 3  hellblau
            SoftPWMSet(greenPin3, 200);   
            SoftPWMSet(bluePin3, 255);   
              SoftPWMSet(redPin4, 5); // Ecke 4  türkis
              SoftPWMSet(greenPin4, 255);
              SoftPWMSet(bluePin4, 70);
        delay(1000);
        SoftPWMSet(redPin2, 0);  // Ecke 2 blau
        SoftPWMSet(greenPin2, 0);   
        SoftPWMSet(bluePin2, 255);
          SoftPWMSet(redPin3, 0);   // Ecke 3  grün
          SoftPWMSet(greenPin3, 255);   
          SoftPWMSet(bluePin3, 0);
            SoftPWMSet(redPin4, 1);   // Ecke 4  hellblau
            SoftPWMSet(greenPin4, 200);   
            SoftPWMSet(bluePin4, 255);   
              SoftPWMSet(redPin1, 5); // Ecke 1  türkis
              SoftPWMSet(greenPin1, 255);
              SoftPWMSet(bluePin1, 70);
        delay(1000);
        SoftPWMSet(redPin3, 0);  // Ecke 3 blau
        SoftPWMSet(greenPin3, 0);   
        SoftPWMSet(bluePin3, 255);
          SoftPWMSet(redPin4, 0);   // Ecke 3  grün
          SoftPWMSet(greenPin4, 255);   
          SoftPWMSet(bluePin4, 0);
            SoftPWMSet(redPin1, 1);   // Ecke 1  hellblau
            SoftPWMSet(greenPin1, 200);   
            SoftPWMSet(bluePin1, 255);   
              SoftPWMSet(redPin2, 5); // Ecke 2  türkis
              SoftPWMSet(greenPin2, 255);
              SoftPWMSet(bluePin2, 70);
        delay(1000);
        SoftPWMSet(redPin4, 0);  // Ecke 4 blau
        SoftPWMSet(greenPin4, 0);   
        SoftPWMSet(bluePin4, 255);
          SoftPWMSet(redPin1, 0);   // Ecke 1  grün
          SoftPWMSet(greenPin1, 255);   
          SoftPWMSet(bluePin1, 0);
            SoftPWMSet(redPin2, 1);   // Ecke 2  hellblau
            SoftPWMSet(greenPin2, 200);   
            SoftPWMSet(bluePin2, 255);   
              SoftPWMSet(redPin3, 5); // Ecke 3  türkis
              SoftPWMSet(greenPin3, 255);
              SoftPWMSet(bluePin3, 70);
        delay(1000);
        break;  
        //***********************************************************************************
        
        
    }
    delay (250);
  }  
}
 
  
void serialEvent() { //x
  String inputString = "";
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      char temp[inputString.length() + 1];
      inputString.toCharArray(temp, sizeof(temp));

      befehl = atoi(temp);
      Serial.print("Befehl: ");
      Serial.println(befehl);
      inputString = "";
    } 
    else
      inputString += inChar;
  }
}

