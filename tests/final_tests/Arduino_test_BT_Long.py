#include <arduino-timer.h>
#include <CircularBuffer.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h> 
#include <TimerOne.h>
#include <TimerThree.h>
#include <WheatstoneBridge.h>
#include <RTClib.h>

int ADXLAddress = 0x53; // Device address in which is also included the 8th bit for selecting the mode, read in this case.

#define X_Axis_Register_DATAX0 0x32 // Hexadecimal address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecimal address for the DATAX1 internal register.
#define Power_Register 0x2D // Power Control Register
const int sd_cs = 53; //SD card CS pin connected to pin 53 of Arduino
volatile bool enable = false;
uint8_t start_millis;
int sample_count = 0;
unsigned long time = 0;

File dataFile;
File strainFile;
RTC_DS3231 rtc;
Timer<2, millis> accel_timer;
Timer<1, millis> strain_timer;
//CircularBuffer<uint16_t, 200> buffer;

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

// ----------------- accelerometer -----------------------
int16_t X1; int16_t Y1; int16_t Z1;
int16_t X2; int16_t Y2; int16_t Z2;
int16_t X3; int16_t Y3; int16_t Z3;
int16_t X4; int16_t Y4; int16_t Z4;


// --------------------------------------------------------------

void acc_init(void) {  
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


void setup(void) 
{
  pinMode(34, OUTPUT);
  digitalWrite(34, HIGH);

  // ---------- SD card setup ----------

  if(!SD.begin(sd_cs)) {
    while(1);
  }
    
  //open file
  if (SD.exists("ACCEL.txt")) {
    SD.remove("ACCEL.txt"); 
  }
  dataFile = SD.open("ACCEL.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (!dataFile) {
    dataFile.println("Data file not opened");
    digitalWrite(34, LOW);
  }
  dataFile.close();

  if (SD.exists("STRAIN.txt")) {
    SD.remove("STRAIN.txt"); 
  }
  strainFile = SD.open("STRAIN.txt", FILE_WRITE);
  if (!strainFile) {
    strainFile.println("Strain file not opened");
    digitalWrite(34, LOW);
    while(1);
  }
  strainFile.close();


  // ---------- accelerometer setup ----------
  acc_init();
  delay(10);

  // ---------- Timer setup ----------
  accel_timer.every(5,print_accel);
  accel_timer.every(200,print_strain);
  
  delay(100);

  strainFile = SD.open("STRAIN.txt",FILE_WRITE);
  strainFile.println("time,strain1,strain2,strain3,srain4");
  dataFile = SD.open("ACCEL.txt", FILE_WRITE);
  dataFile.println("time,accel1_x,accel1_y,accel1_z,accel2_x,accel2_y,accel2_z,accel3_x,accel3_y,accel3_z,accel4_x,accel4_y,accel4_z");

//  // SETUP RTC MODULE
//  if (! rtc.begin()) {
//    digitalWrite(34, LOW);
//    while (1);
//  }
//
//  // automatically sets the RTC to the date & time on PC this sketch was compiled
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//
//  start_millis = millis();
//  DateTime now = rtc.now();

//  dataFile.print(now.hour());dataFile.print(":");dataFile.print(now.minute());dataFile.print(":");dataFile.println(now.second());

//  Timer1.initialize(); //Initialize timer with 1 millisecond period
//  Timer1.setPeriod(10000);
//  Timer1.attachInterrupt(isr);
  
  delay(100);
}

// -----------------------

void sample_accel() {
  time = millis() - start_millis;

  // ------------------------------
//  digitalWrite(23, LOW);
//  digitalWrite(24, HIGH);
//  digitalWrite(25, HIGH);
//  digitalWrite(26, HIGH); 
//  
//  // === Read acceleromter data === //
//  Wire.beginTransmission(0x53);
//  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
//  Wire.endTransmission(false);
//  Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
//
//  X1 = ( Wire.read()| Wire.read() << 8); // X-axis value
//  Y1 = ( Wire.read()| Wire.read() << 8); // Y-axis value
//  Z1 = ( Wire.read()| Wire.read() << 8); // Z-axis value
//  Wire.endTransmission();

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

  X2 = (( Wire.read()| Wire.read() << 8)); // X-axis value
  Y2 = (( Wire.read()| Wire.read() << 8)); // Y-axis value
  Z2 = (( Wire.read()| Wire.read() << 8)); // Z-axis value
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

  X3 = ( Wire.read()| Wire.read() << 8); // X-axis value
  Y3 = ( Wire.read()| Wire.read() << 8); // Y-axis value
  Z3 = ( Wire.read()| Wire.read() << 8); // Z-axis value
  Wire.endTransmission();

// ------------------------------

//  digitalWrite(23, HIGH);
//  digitalWrite(24, HIGH);
//  digitalWrite(25, LOW); // change
//  digitalWrite(26, HIGH); 
//
//  // === Read acceleromter data === //
//  Wire.beginTransmission(0x53);
//  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
//  Wire.endTransmission(false);
//  Wire.requestFrom(0x53, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
//
//  X4 = ( Wire.read()| Wire.read() << 8); // X-axis value
//  Y4 = ( Wire.read()| Wire.read() << 8); // Y-axis value
//  Z4 = ( Wire.read()| Wire.read() << 8); // Z-axis value
//  Wire.endTransmission();

// ------------------------------


}

void sample_strain() {  
  val1 = wsb_strain1.measureForce();
  valRaw1 = wsb_strain1.getLastForceRawADC();
  val2 = wsb_strain2.measureForce();
  valRaw2 = wsb_strain2.getLastForceRawADC();
  val3 = wsb_strain3.measureForce();
  valRaw3 = wsb_strain3.getLastForceRawADC();
  val4 = wsb_strain4.measureForce();
  valRaw4 = wsb_strain4.getLastForceRawADC();

}

bool print_strain(void) {
  strainFile.print(time);strainFile.print(",");
  strainFile.print(valRaw1);strainFile.print(",");
  strainFile.print(valRaw2);strainFile.print(",");
  strainFile.print(valRaw3);strainFile.print(",");
  strainFile.println(valRaw4);
  return true;
}

bool print_accel(void) {
//  enable = true;
  dataFile.print(time);dataFile.print(",");
//      dataFile.print(minute);dataFile.print(":");dataFile.print(second);dataFile.print(",");
//  dataFile.print(X1);dataFile.print(",");dataFile.print(Y1);dataFile.print(",");dataFile.print(Z1);dataFile.print(",");
  dataFile.print(X2);dataFile.print(",");dataFile.print(Y2);dataFile.print(",");dataFile.print(Z2);dataFile.print(",");
  dataFile.print(X3);dataFile.print(",");dataFile.print(Y3);dataFile.print(",");dataFile.println(Z3);
//  dataFile.print(X4);dataFile.print(",");dataFile.print(Y4);dataFile.print(",");dataFile.println(Z4);

  return true;
}

void loop() {
  sample_accel();
  sample_strain();
  accel_timer.tick();
//  strain_timer.tick();

  if (millis() > 270000) {
    accel_timer.cancel();
//    strain_timer.cancel();
    
    dataFile.close();
    strainFile.close();
  
    digitalWrite(34, LOW);
    while(1);
  }
}
