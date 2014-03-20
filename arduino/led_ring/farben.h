#include <Arduino.h>

byte aus[3][2] =   {{ 0,   0}    //red
                  ,{  0,   0}    //green
                  ,{  0,   0}};  //blue
//----------------------------------------------------- 
//Farbe Weiß
byte weiss_an[3][2] = {{ 255, 255}   //red
                     ,{  255, 255}   //green
                     ,{  255, 255}};   //blue

byte weiss_blink[3][2] = {{ 128, 255}   
                        ,{  128, 255}    
                        ,{  128, 255}};  
//-----------------------------------------------------
//Farbe Rosa
byte rosa_an[3][2] =  {{ 235, 235}
                     ,{  33, 33}
                     ,{  180, 180}};
                      
byte rosa_blink[3][2] = {{ 128, 235}
                        ,{ 20, 33}
                        ,{ 128, 235}};
//-----------------------------------------------------  
//Farbe Blau
byte blau_an[3][2] = {{ 0,   0}
                    ,{  0,   0}
                    ,{  255, 255}};   

byte blau_blink[3][2] = {{ 0,   0}
                       ,{  0,   0}
                       ,{  128,   255}};   
//-----------------------------------------------------
//Farbe Hellblau
byte hellblau_an[3][2] = {{ 22,   22}
                        ,{  160,   160}
                        ,{  255,   255}};       
                         
byte hellblau_blink[3][2] = {{15,   22}
                           ,{ 110,   206}
                           ,{ 128,   245}};   

//-----------------------------------------------------
//-----------------------------------------------------
//Nicht Spielfiguren Farben
//Farbe Grün
byte gruen_an[3][2] = {{ 0,  0}
                     ,{  150, 150}
                     ,{  0,  0}};   

byte gruen_blink[3][2] = {{ 0,   0}
                        ,{  128,  255}
                        ,{  0,   0}}; 
//-----------------------------------------------------                        
//Farbe Gelb
byte gelb_an[3][2] = {{ 255,  255}
                     ,{  255, 255}
                     ,{  0,  0}};   

byte gelb_blink[3][2] = {{ 128,   255}
                        ,{  128,  255}
                        ,{  0,   0}};                          
//-----------------------------------------------------              
//Farbe Rot
byte rot_an[3][2] = {{ 255,  255}
                     ,{  0,  0}
                     ,{  0,  0}};   

byte rot_blink[3][2] = {{ 128,  255}
                        ,{  0,  0}
                        ,{  0,  0}};                          
//-----------------------------------------------------              
//Farbe Orange
byte orange_an[3][2] = {{ 205,  205}
                      ,{  112,  112}
                      ,{  20,  20}};   

byte orange_blink[3][2] = {{ 128,  205}
                         ,{  108,  112}
                         ,{  10,  20}};                          
//----------------------------------------------------- 