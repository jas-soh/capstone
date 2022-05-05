#include <Wire.h>
#include <SD.h>
#include <SPI.h> 
#include <TimerOne.h>
#include <WheatstoneBridge.h>
#include <RTClib.h>


#define X_Axis_Register_DATAX0 0x32 // Hexadecimal address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecimal address for the DATAX1 internal register.
#define Power_Register 0x2D // Power Control Register

unsigned long testing_time = 75000; // change this to change the length of time (in milliseconds) to record data; 75000 is 1 minute 15 seconds

int ADXLAddress = 0x53; // Device address in which is also included the 8th bit for selecting the mode, read in this case.
const int chipSelect = 53; //SD card CS pin connected to pin 53 of Arduino
File dataFile;
volatile bool enable = false;
unsigned long time = 0;
unsigned long start_time = 0;
RTC_DS3231 rtc;

// --------------------------------------------------------------

void acc_init(void) {

//  Serial.println("Accelerometer Test"); Serial.println("");
  
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);

// -------------------------------------------
  digitalWrite(23, LOW);
  digitalWrite(24, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);

  Wire.begin(); // Initiate the Wire library

  delay(100);
  
  // Enable measurement
  Wire.beginTransmission(ADXLAddress);
  Wire.write(Power_Register);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);  
  Wire.endTransmission();

// -----------------------------------------------
  digitalWrite(23, HIGH);
  digitalWrite(24, LOW);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);

  Wire.begin(); // Initiate the Wire library

  delay(100);
  
  // Enable measurement
  Wire.beginTransmission(ADXLAddress);
  Wire.write(Power_Register);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);  
  Wire.endTransmission();

// --------------------------------------------------

  digitalWrite(23, HIGH);
  digitalWrite(24, HIGH);
  digitalWrite(25, LOW);
  digitalWrite(26, HIGH);

  Wire.begin(); // Initiate the Wire library

  delay(100);
  
  // Enable measurement
  Wire.beginTransmission(ADXLAddress);
  Wire.write(Power_Register);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);  
  Wire.endTransmission();
  
}

void isr(void) {
  enable = true;
}

void setup(void) 
{
  // turn on LED to indicate on
  pinMode(34, OUTPUT);
  digitalWrite(34, HIGH);

  // ---------- SD card setup ----------

  if(!SD.begin(chipSelect)) {
    while(1);
  }
    
  //open file
  if (SD.exists("ACCEL.txt")) {
    SD.remove("ACCEL.txt"); 
  }
  dataFile = SD.open("ACCEL.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (!dataFile) {
//    dataFile.println("Data file not opened");
    digitalWrite(34, LOW);
    while(1);
  }
  dataFile.println("time,accel1_x,accel1_y,accel1_z,accel2_x,accel2_y,accel2_z,accel3_x,accel3_y,accel3_z,accel4_x,accel4_y,accel4_z");
  dataFile.close();


  // ---------- accelerometer setup ----------
  acc_init();
  delay(10);

  // ---------- Timer setup ----------
  Timer1.initialize(); //Initialize timer with 1 millisecond period
  Timer1.setPeriod(10000);
  Timer1.attachInterrupt(isr);

  // SETUP RTC MODULE
//  if (! rtc.begin()) {
//    Serial.println("Couldn't find RTC");
//    Serial.flush();
//    digitalWrite(34, LOW);
//    while (1);
//  }

  // automatically sets the RTC to the date & time on PC this sketch was compiled
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//  rtc.setTime()
  
  delay(100);

}

// -----------------------

void loop() {
  dataFile = SD.open("ACCEL.txt", FILE_WRITE);
  start_time = millis();
//  DateTime now = rtc.now();
//  dataFile.print(now.hour());dataFile.print(":");dataFile.print(now.minute());dataFile.print(":");dataFile.println(now.second());

  while (millis() < testing_time) { // 1 minute 15 seconds 75000 // 7 minutes 420000
    if (enable) {

    // ------------------------------
      digitalWrite(23, LOW);
      digitalWrite(24, HIGH);
      digitalWrite(25, HIGH);
      digitalWrite(26, HIGH); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      time = millis()- start_time;
      int16_t X1 = ( Wire.read()| Wire.read() << 8); // X-axis value
      int16_t Y1 = ( Wire.read()| Wire.read() << 8); // Y-axis value
      int16_t Z1 = ( Wire.read()| Wire.read() << 8); // Z-axis value
      Wire.endTransmission();
    
    // ------------------------------

      digitalWrite(23, HIGH);
      digitalWrite(24, LOW);
      digitalWrite(25, HIGH);
      digitalWrite(26, HIGH); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      int16_t X2 = ( Wire.read()| Wire.read() << 8); // X-axis value
      int16_t Y2 = ( Wire.read()| Wire.read() << 8); // Y-axis value
      int16_t Z2 = ( Wire.read()| Wire.read() << 8); // Z-axis value
      Wire.endTransmission();
    
    // ------------------------------

      digitalWrite(23, HIGH);
      digitalWrite(24, HIGH);
      digitalWrite(25, LOW);
      digitalWrite(26, HIGH); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      int16_t X3 = ( Wire.read()| Wire.read() << 8); // X-axis value
      int16_t Y3 = ( Wire.read()| Wire.read() << 8); // Y-axis value
      int16_t Z3 = ( Wire.read()| Wire.read() << 8); // Z-axis value
      Wire.endTransmission();
    
    // ------------------------------

      digitalWrite(23, HIGH);
      digitalWrite(24, HIGH);
      digitalWrite(25, HIGH); 
      digitalWrite(26, LOW); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      int16_t X4 = ( Wire.read()| Wire.read() << 8); // X-axis value
      int16_t Y4 = ( Wire.read()| Wire.read() << 8); // Y-axis value
      int16_t Z4 = ( Wire.read()| Wire.read() << 8); // Z-axis value
      Wire.endTransmission();
    
    // ------------------------------
    
      dataFile.print(time);dataFile.print(",");
      dataFile.print(X1);dataFile.print(",");dataFile.print(Y1);dataFile.print(",");dataFile.print(Z1);dataFile.print(",");
      dataFile.print(X2);dataFile.print(",");dataFile.print(Y2);dataFile.print(",");dataFile.print(Z2);dataFile.print(",");
      dataFile.print(X3);dataFile.print(",");dataFile.print(Y3);dataFile.print(",");dataFile.print(Z3);dataFile.print(",");
      dataFile.print(X4);dataFile.print(",");dataFile.print(Y4);dataFile.print(",");dataFile.println(Z4);

      enable = false;
    }
  
  }
  dataFile.close();

  digitalWrite(34, LOW);

  while(1);
}


  
