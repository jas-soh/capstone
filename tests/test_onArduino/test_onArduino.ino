#include <CircularBuffer.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h> 
#include <TimerOne.h>
#include <WheatstoneBridge.h>

int ADXLAddress = 0x53; // Device address in which is also included the 8th bit for selecting the mode, read in this case.

#define X_Axis_Register_DATAX0 0x32 // Hexadecimal address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecimal address for the DATAX1 internal register.
#define Power_Register 0x2D // Power Control Register
const int chipSelect = 10; //SD card CS pin connected to pin 53 of Arduino
File dataFile;
File strainFile;
CircularBuffer<uint16_t, 200> buffer;
volatile bool enable = false;
int sample_strain = 0;
unsigned long time = 0;

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

void acc_init(void) {

//  Serial.println("Accelerometer Test"); Serial.println("");
  
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

// -------------------------------------------
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);

  Wire.begin(); // Initiate the Wire library

  delay(100);
  
  // Enable measurement
  Wire.beginTransmission(ADXLAddress);
  Wire.write(Power_Register);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);  
  Wire.endTransmission();

// -----------------------------------------------
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);

  Wire.begin(); // Initiate the Wire library

  delay(100);
  
  // Enable measurement
  Wire.beginTransmission(ADXLAddress);
  Wire.write(Power_Register);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);  
  Wire.endTransmission();

// --------------------------------------------------

  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);

  Wire.begin(); // Initiate the Wire library

  delay(100);
  
  // Enable measurement
  Wire.beginTransmission(ADXLAddress);
  Wire.write(Power_Register);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);  
  Wire.endTransmission();
  
}

void get_strain(void) {
  val1 = wsb_strain1.measureForce();
  valRaw1 = wsb_strain1.getLastForceRawADC();
  val2 = wsb_strain2.measureForce();
  valRaw2 = wsb_strain2.getLastForceRawADC();
  val3 = wsb_strain3.measureForce();
  valRaw3 = wsb_strain3.getLastForceRawADC();
  val4 = wsb_strain4.measureForce();
  valRaw4 = wsb_strain4.getLastForceRawADC();
  buffer.push(time);
  buffer.push((uint8_t)valRaw1);
  buffer.push((uint8_t)valRaw2);
  buffer.push((uint8_t)valRaw3);
  buffer.push((uint8_t)valRaw4);
}

void isr(void) {
  enable = true;
  if (sample_strain == 50) {
    get_strain();
    sample_strain = 0;
  } else {
    sample_strain++;
  }
}

void setup(void) 
{
//  // turn on built-in LED
//  pinMode(LED_BUILTIN, OUTPUT);
//  digitalWrite(LED_BUILTIN, HIGH);

  // ---------- SD card setup ----------

  if(!SD.begin(chipSelect)) {
    while(1);
  }
    
  //open file
  if (SD.exists("LOGDATA.txt")) {
    SD.remove("LOGDATA.txt"); 
  }
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (!dataFile) {
    dataFile.println("Data file not opened");
  }
  dataFile.close();


  // ---------- accelerometer setup ----------
  acc_init();
  delay(10);

  // ---------- Timer setup ----------
  Timer1.initialize(); //Initialize timer with 1 millisecond period
  Timer1.setPeriod(10000);
  Timer1.attachInterrupt(isr);
  delay(100);

  if (SD.exists("STRAIN.txt")) {
    SD.remove("STRAIN.txt"); 
  }
  strainFile = SD.open("STRAIN.txt", FILE_WRITE);
  if (!strainFile) {
    strainFile.println("Strain file not opened");
    while(1);
  }
  strainFile.close();

  delay(100);
}



// -----------------------

void loop() {
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

  while (millis() < 5000) {
    if (enable) {

    // ------------------------------
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(4, HIGH); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      time = millis();
      uint8_t X1 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      uint8_t Y1 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      uint8_t Z1 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
      Wire.endTransmission();
    
    // ------------------------------

      digitalWrite(7, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(4, HIGH); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      uint8_t X2 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      uint8_t Y2 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      uint8_t Z2 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
      Wire.endTransmission();
    
    // ------------------------------

      digitalWrite(7, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      uint8_t X3 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      uint8_t Y3 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      uint8_t Z3 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
      Wire.endTransmission();
    
    // ------------------------------

      digitalWrite(7, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW); // change
      digitalWrite(4, HIGH); 
    
      // === Read acceleromter data === //
      Wire.beginTransmission(0x53);
      Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
    
      uint8_t X4 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      uint8_t Y4 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      uint8_t Z4 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
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
      
  if (millis() > 5000) {

    // write data from buffer to strain data file
    strainFile = SD.open("STRAIN.txt",FILE_WRITE);
    
    while (buffer.size() >= 5) {
      time = buffer.first();
      buffer.shift();
      strainFile.print(time);strainFile.print(",");
      for (int i = 1; i < 4; i++) {
        int strain = buffer.first();
        strainFile.print(strain);strainFile.print(",");
        buffer.shift();
      }
      strainFile.println(buffer.first());
      buffer.shift();
    }

    strainFile.close();

//    // all files are closed, turn off built-in LED to indicate ok to remove SD card
//    digitalWrite(LED_BUILTIN, LOW);
    while(1);
  }
}

  
