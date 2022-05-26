#include <CircularBuffer.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h> 
#include <TimerOne.h>
#include <TimerThree.h>
#include <WheatstoneBridge.h>
#include <RTClib.h>

unsigned long testing_time = 270000; // change this to change the length of time (in milliseconds) to record data; 75000 is 1 minute 15 seconds; 7 minutes 420000
int ADXLAddress = 0x53; // Device address in which is also included the 8th bit for selecting the mode, read in this case.

#define X_Axis_Register_DATAX0 0x32 // Hexadecimal address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecimal address for the DATAX1 internal register.
#define Power_Register 0x2D // Power Control Register
const int sd_cs = 53; //SD card CS pin connected to pin 53 of Arduino
volatile bool enable = false;
unsigned long time = 0;

File strainFile;
RTC_DS3231 rtc;

// ------------------ strain ---------------------------
WheatstoneBridge wsb_strain1(A0, 365, 675, 0, 1000);
WheatstoneBridge wsb_strain2(A1, 365, 675, 0, 1000);
WheatstoneBridge wsb_strain3(A2, 365, 675, 0, 1000);
WheatstoneBridge wsb_strain4(A3, 365, 675, 0, 1000);

int val1;
int valRaw1;
int val2;
int valRaw2;
int val3;
int valRaw3;
int val4;
int valRaw4;

// --------------------------------------------------------------


void isr(void) {
  enable = true;
}


void setup(void) 
{
  pinMode(34, OUTPUT);
  digitalWrite(34, HIGH);
  

  // ---------- SD card setup ----------
  if(!SD.begin(sd_cs)) {
    while(1);
  }
  
  if (SD.exists("STRAIN.txt")) {
    SD.remove("STRAIN.txt"); 
  }
  strainFile = SD.open("STRAIN.txt", FILE_WRITE);
  if (!strainFile) {
//    strainFile.println("Strain file not opened");
    digitalWrite(34, LOW);
    while(1);
  }
  strainFile.close();

  strainFile = SD.open("STRAIN.txt",FILE_WRITE);
  strainFile.println("time,strain1,strain2,strain3,strain4");

  // ---------- Timer setup ----------
  Timer1.initialize(); //Initialize timer with 1 millisecond period
  Timer1.setPeriod(200000);
  Timer1.attachInterrupt(isr);
  
  delay(10);

  // SETUP RTC MODULE
//  if (! rtc.begin()) {
//    Serial.println("Couldn't find RTC");
//    Serial.flush();
//    digitalWrite(34, LOW);
//    while (1);
//  }

  // automatically sets the RTC to the date & time on PC this sketch was compiled
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  delay(100);
}


// -----------------------

void loop() {
  uint16_t start_millis = millis();
//  DateTime now = rtc.now();
//  strainFile.print(now.hour());strainFile.print(":");strainFile.print(now.minute());strainFile.print(":");strainFile.println(now.second());

  while (millis() < testing_time) { // 7 minutes
    if (enable) {

      time = millis() - start_millis;

      val1 = wsb_strain1.measureForce();
      valRaw1 = wsb_strain1.getLastForceRawADC();
      val2 = wsb_strain2.measureForce();
      valRaw2 = wsb_strain2.getLastForceRawADC();
      val3 = wsb_strain3.measureForce();
      valRaw3 = wsb_strain3.getLastForceRawADC();
      val4 = wsb_strain4.measureForce();
      valRaw4 = wsb_strain4.getLastForceRawADC();
      strainFile.print(time);strainFile.print(",");
      strainFile.print((uint8_t)valRaw1);strainFile.print(",");
      strainFile.print((uint8_t)valRaw2);strainFile.print(",");
      strainFile.print((uint8_t)valRaw3);strainFile.print(",");
      strainFile.println((uint8_t)valRaw4);

      enable = false;
    }
  
  }

  Timer1.detachInterrupt();     
  
  strainFile.close();
  digitalWrite(34, LOW);
  while(1);


}

  
