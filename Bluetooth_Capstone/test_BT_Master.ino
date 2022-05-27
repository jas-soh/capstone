#include <arduino-timer.h>
#include <CircularBuffer.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h> 
#include <TimerOne.h>
#define TX 10
#define RX 11
SoftwareSerial HM10(RX, TX); // (RX, TX)

int ADXLAddress = 0x53; // Device address in which is also included the 8th bit for selecting the mode, read in this case.

#define X_Axis_Register_DATAX0 0x32 // Hexadecimal address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecimal address for the DATAX1 internal register.
#define Power_Register 0x2D // Power Control Register
#define BAUDRATE 9600
const int sd_cs = 53; //SD card CS pin connected to pin 53 of Arduino
volatile bool enable = false;
uint8_t start_millis;
int sample_count = 0;
unsigned long time = 0;

Timer<2, millis> accel_timer;
//CircularBuffer<uint16_t, 200> buffer;

// ----------------------- Packet to read bluetooth acceleration values --------------------------------
struct Packet {
  float x;
  float y;
  float z;
} pkt; // Instantiate a Packet struct


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
  digitalWrite(9, HIGH);
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
    // Start Serial Monitor for feedback
  Serial.begin(BAUDRATE);

  // HM-10 virtual UART
  HM10.begin(BAUDRATE);  

  pinMode(34, OUTPUT);
  digitalWrite(34, HIGH);


  // ---------- accelerometer setup ----------
  acc_init();
  delay(10);

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

void loop() {
  sample_accel();
  accel_timer.tick();
  bluetooth_transmit();  
  print_packet();
//  strain_timer.tick();

  if (millis() > 270000) {
    accel_timer.cancel();
//    strain_timer.cancel();

  
    digitalWrite(34, LOW);
    while(1);
  }
}

// Function responsible for transmitting data over bluetooth
void bluetooth_transmit() {
  // Update data to be transmitted
  pkt.x = X2;
  pkt.y = Y2;
  pkt.z = Z2;
  
  // Write packet data to the bluetooth - and transmit
  HM10.write((byte *) & pkt,sizeof(Packet));

  // Print packet (debug)
  print_packet();
}

// Function to print packet data (debug)
void print_packet() {  
  Serial.print("TX: (x,y,z)=(");
  Serial.print(pkt.x); Serial.print(",");
  Serial.print(pkt.y); Serial.print(",");
  Serial.print(pkt.z); Serial.print(",");  
  Serial.println(")");
}
