/* -------------------------------------------------------------------------
   Virtual Nixie Tube Clock is built on RGB Led Matrix 64x64.
   Library: PxMatrix.
   Led matrix: P2.5 RGB 64x64
   ------------------------------------------------------------------------- */

#define double_buffer
#include <PxMatrix.h>

// WIFI SETUP

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
const char* ssid     = "FTPTelecom";              // SSID of local network
const char* password = "12345689";                // Password on network
#define NTP_OFFSET      25200                     // In seconds - Time offset
#define NTP_INTERVAL    60 * 1000                 // In miliseconds
#define NTP_ADDRESS     "1.asia.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

#define DISPLAY_DRAW_TIME 10

unsigned long samplingtime  = 0;

unsigned long samplingtimes0 = 0;
unsigned long samplingtimes1 = 0;
unsigned long samplingtimem0 = 0;
unsigned long samplingtimem1 = 0;
unsigned long samplingtimeh0 = 0;
unsigned long samplingtimeh1 = 0;

int s0, s1, m0, m1, h0, h1;
int prves0, prves1, prvem0, prvem1, prveh0, prveh1;

bool dotstate=1;

#ifdef ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#endif

// Pins for LED MATRIX

PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

uint16_t myRED      = display.color565(255, 0, 0);
uint16_t myGREEN    = display.color565(0, 255, 0);
uint16_t myBLUE     = display.color565(0, 0, 255);
uint16_t myMAGENTA  = display.color565(255, 0, 255);
uint16_t myYELLOW   = display.color565(255, 255, 0);
uint16_t myCYAN     = display.color565(0, 255, 255);
uint16_t myBLACK    = display.color565(0, 0, 0);
uint16_t myWHITE    = display.color565(255, 255, 255);

uint16_t myCOLORS[7]={myRED, myGREEN, myCYAN, myMAGENTA, myYELLOW, myBLUE, myWHITE};

#ifdef ESP8266
// ISR for display refresh
void display_updater()
{
  display.display(DISPLAY_DRAW_TIME);
}
#endif

#ifdef ESP32
void IRAM_ATTR display_updater() {
  display.display(DISPLAY_DRAW_TIME);
}
#endif

/* The Nixie Numbers Code */

#include "NixieNumbers.h"
NixieNumbers NixieNumbers(&display);

/* Small Nixie Number*/

const uint16_t *Numbers_S[10] = {

Nixie_20x31_0,
Nixie_20x31_1,
Nixie_20x31_2,
Nixie_20x31_3,
Nixie_20x31_4,
Nixie_20x31_5,
Nixie_20x31_6,
Nixie_20x31_7,
Nixie_20x31_8,
Nixie_20x31_9,

};

/* Big Nixie Number*/

const uint16_t *Numbers_B[10] = {
Nixie_32x61_0,
Nixie_32x61_1,
Nixie_32x61_2,
Nixie_32x61_3,
Nixie_32x61_4,
Nixie_32x61_5,
Nixie_32x61_6,
Nixie_32x61_7,
Nixie_32x61_8,
Nixie_32x61_9,
};


void setup()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  Serial.print(".");
  }
  timeClient.begin();

  //Serial.begin(115200);

  display.begin(32);
  display.setMuxDelay(1,1,1,1,1);   // Delay multiplexing - Important for some panel with slow multiplexer
  display.setFastUpdate(true);
  display.clearDisplay();

  #ifdef ESP8266
    display_ticker.attach(0.002, display_updater);
  #endif

  #ifdef ESP32
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  #endif
}

void loop()
{
timeClient.update();

int rawTime = timeClient.getEpochTime();
  
int hours = (rawTime % 86400L) / 3600;

int minutes = (rawTime % 3600) / 60;

int seconds = rawTime % 60;

  h1 = ((hours/10));
  h0 = (hours%10);
    
  m1 = ((minutes/10));
  m0 = (minutes%10);
  
  s1 = ((seconds/10));
  s0 = (seconds%10);
  
  if (seconds>=0 && seconds<=30) // Show seconds from 0 ~ 30 in Small Nixie Number.
    {   
      if (seconds==0)
      {
        NixieNumbers.clearNumber(0, 0, NUMBER_WIDTH_B, NUMBER_HEIGHT_B);    // Clear Big Nixie Number
        NixieNumbers.clearNumber(32, 0, NUMBER_WIDTH_B, NUMBER_HEIGHT_B);   // Clear Big Nixie Number
      }
        Nixie_S0();
        Nixie_S1();
        Nixie_M0();
        Nixie_M1();
        Nixie_H0();
        Nixie_H1();
    }
    else   // Show seconds from 31 ~ 59 in Big Nixie Number.
    {
      if (seconds==31)
      {     
        NixieNumbers.clearNumber(0, 0, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);    // Clear Small Nixie Number
        NixieNumbers.clearNumber(0, 33, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);   // Clear Small Nixie Number
        NixieNumbers.clearNumber(22, 0, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);   // Clear Small Nixie Number
        NixieNumbers.clearNumber(22, 33, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);  // Clear Small Nixie Number
        NixieNumbers.clearNumber(44, 0, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);   // Clear Small Nixie Number
        NixieNumbers.clearNumber(44, 33, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);  // Clear Small Nixie Number
      } 
        Nixie_S0_B();
        Nixie_S1_B();
    }

  /* Show the contents of buffer to led matrix screen */
  display.showBuffer();
}

void Nixie_S1()
{ 
  if ((unsigned long) (micros() - samplingtimes1) > 101)
  {                             
    if (s1 != prves1)  
      {                  
        //display.clearDisplay();
        NixieNumbers.clearNumber(44, 0, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        NixieNumbers.drawNumber(44, 0, Numbers_S[s1], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        prves1 = s1;         
      }
    else
      {
        NixieNumbers.drawNumber(44, 0, Numbers_S[prves1], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
      }      
   
      samplingtimes1 = micros(); 
    }
}        

void Nixie_S0()
{ 
  if ((unsigned long) (micros() - samplingtimes0) > 99)
  {      
      if (s0 != prves0)  
      {  
        //display.clearDisplay();
        NixieNumbers.clearNumber(44, 33, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        NixieNumbers.drawNumber(44, 33, Numbers_S[s0], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);       
        prves0 = s0;
      }          
    else
      {
        NixieNumbers.drawNumber(44, 33, Numbers_S[prves0], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
      }
      samplingtimes0 = micros(); 
    }
}

void Nixie_M1()
{ 
  if ((unsigned long) (micros() - samplingtimem1) > 105)
  {     
    if (m1 != prvem1)  
      {      
        //display.clearDisplay();
        NixieNumbers.clearNumber(22, 0, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        NixieNumbers.drawNumber(22, 0, Numbers_S[m1], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);              
        prvem1 = m1;
      }
    else
      {
        NixieNumbers.drawNumber(22, 0, Numbers_S[prvem1], NUMBER_WIDTH_S, NUMBER_HEIGHT_S); 
      }      
      samplingtimem1 = micros(); 
    }
}   

void Nixie_M0()
{ 
  if ((unsigned long) (micros() - samplingtimem0) > 103)
  {
    if (m0!=prvem0)  
      {         
        //display.clearDisplay();
        NixieNumbers.clearNumber(22, 33, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        NixieNumbers.drawNumber(22, 33, Numbers_S[m0], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        prvem0 = m0;
      }
    else
      {
        NixieNumbers.drawNumber(22, 33, Numbers_S[prvem0], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
      }      
      samplingtimem0 = micros(); 
    }
} 

void Nixie_H1()
{ 
  if ((unsigned long) (micros() - samplingtimeh1) > 109)
  {
    if (h1 != prveh1)  
      {          
        //display.clearDisplay();
        NixieNumbers.clearNumber(0, 0, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        NixieNumbers.drawNumber(0, 0, Numbers_S[h1], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        prveh1 = h1;
      }
    else
      {
        NixieNumbers.drawNumber(0, 0, Numbers_S[prveh1], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
      }
      samplingtimeh1 = micros(); 
    }
}       

void Nixie_H0()
{ 
  if ((unsigned long) (micros() - samplingtimeh0) > 107)
  {      
    if (h0 != prveh0)  
      {   
        //display.clearDisplay();
        NixieNumbers.clearNumber(0, 33, NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        NixieNumbers.drawNumber(0, 33, Numbers_S[h0], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
        prveh0 = h0;
      }
      else
      {
        NixieNumbers.drawNumber(0, 33, Numbers_S[prveh0], NUMBER_WIDTH_S, NUMBER_HEIGHT_S);
      }
      samplingtimeh0 = micros(); 
    }
} 

void Nixie_S1_B()
{ 
  if ((unsigned long) (micros() - samplingtimes1) > 101)
  {                             
    if (s1 != prves1)  
      {                  
        //display.clearDisplay();
        NixieNumbers.clearNumber(0, 0, NUMBER_WIDTH_B, NUMBER_HEIGHT_B);
        NixieNumbers.drawNumber(0, 0, Numbers_B[s1], NUMBER_WIDTH_B, NUMBER_HEIGHT_B);
        prves1 = s1;         
      }
    else
      {
        NixieNumbers.drawNumber(0, 0, Numbers_B[prves1], NUMBER_WIDTH_B, NUMBER_HEIGHT_B);
      }      
   
      samplingtimes1 = micros(); 
    }
}        

void Nixie_S0_B()
{ 
  if ((unsigned long) (micros() - samplingtimes0) > 99)
  {      
      if (s0 != prves0)  
      {  
        //display.clearDisplay();
        NixieNumbers.clearNumber(32, 0, NUMBER_WIDTH_B, NUMBER_HEIGHT_B);
        NixieNumbers.drawNumber(32, 0, Numbers_B[s0], NUMBER_WIDTH_B, NUMBER_HEIGHT_B);        
        prves0 = s0;
      }          
    else
      {
        NixieNumbers.drawNumber(32, 0, Numbers_B[prves0], NUMBER_WIDTH_B, NUMBER_HEIGHT_B);
      }
      samplingtimes0 = micros(); 
    }
}
