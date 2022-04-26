#include <CircularBuffer.h>
#include <Wire.h>
#include <SD.h>
#include <TimerOne.h>

int ADXLAddress = 0x53; // Device address in which is also included the 8th bit for selecting the mode, read in this case.

#define X_Axis_Register_DATAX0 0x32 // Hexadecima address for the DATAX0 internal register.
#define X_Axis_Register_DATAX1 0x33 // Hexadecima address for the DATAX1 internal register.
#define Power_Register 0x2D // Power Control Register
const int chipSelect = 10; //SD card CS pin connected to pin 53 of Arduino
File dataFile;
CircularBuffer<int16_t, 210> buffer;
volatile bool enable = false;
bool isAccel_1 = true;
int accel_num = 1;

void acc_init(void) {

  Serial.println("Accelerometer Test"); Serial.println("");
  
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

void isr(void) {
  enable = true;
}

void setup(void) 
{
  #ifndef ESP8266
    while (!Serial); // for Leonardo/Micro/Zero
  #endif

  // ---------- SD card setup ----------
  Serial.begin(9600);

  // setup for the SD card
  Serial.print("Initializing SD card...");

  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");
    
  //open file
  if (SD.exists("LOGDATA.txt")) {
    SD.remove("LOGDATA.txt"); 
  }
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (dataFile) {
    Serial.println("File opened ok");
    // print the headings for our data
  } else {
    Serial.println("File not opened");
  }
  dataFile.close();

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("Time"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");


  // ---------- accelerometer setup ----------
  acc_init();
  delay(100);

  // ---------- Timer setup ----------
  //Timer1.initialize(1000000); //Initialize timer with 1 second 
  //Timer1.initialize(1000); //Initialize timer with 1 millisecond period
  Timer1.initialize(); //Initialize timer with 1 millisecond period
  Timer1.setPeriod(10000);
  Timer1.attachInterrupt(isr);
  delay(100);

  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

//  digitalWrite(7, LOW);
//  digitalWrite(6, HIGH);
//  digitalWrite(5, HIGH);
//  digitalWrite(4, HIGH); 
}



// -----------------------

void loop() {
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

  while (millis() < 10000) {
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
    
      int time = millis();
      int8_t X1 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      int8_t Y1 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      int8_t Z1 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
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
    
//      int time = millis();
      int8_t X2 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      int8_t Y2 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      int16_t Z2 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
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
    
//      int time = millis();
      int8_t X3 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      int8_t Y3 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      int8_t Z3 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
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
    
//      int time = millis();
      int8_t X4 = ( Wire.read()| Wire.read() << 8) * 10; // X-axis value
      int8_t Y4 = ( Wire.read()| Wire.read() << 8) * 10; // Y-axis value
      int8_t Z4 = ( Wire.read()| Wire.read() << 8) * 10; // Z-axis value
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
      
  if (millis() > 10000) {
//    dataFile.close();
    Serial.println("finished");
    dataFile = SD.open("LOGDATA.txt");
    while (dataFile.available()) {
       Serial.write(dataFile.read());
    }
    // close the file:
    dataFile.close();
    while(1);
  }
}
  
