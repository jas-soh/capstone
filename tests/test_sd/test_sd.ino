////#include <Wire.h>
////#include <Adafruit_Sensor.h>
////#include <Adafruit_ADXL345_U.h>
////#include <TimerOne.h>
////#include <SD.h> 
////#include <SPI.h> 
////
////// ---------- macros ----------
////File dataFile; 
////Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
////volatile bool timerTriggered = false;
////uint32_t time = 0;
////const int chipSelect = 10; //SD card CS pin connected to pin 53 of Arduino
////uint8_t buffer[10][10] = {{}};
////int i = 0;
////int j = 0;
////int a = 0;
////int size = 10;
////// -----------------------------
////
////
////// print data
////void print_data(uint8_t data[]) {
////  Serial.println(data[0]);
////  Serial.println(data[1]);
////  Serial.println(data[2]);
////  Serial.println(data[3]);
////  Serial.println(data[4]);
////  Serial.println(data[5]);
////  Serial.println(data[6]);
////  Serial.println(data[7]);
////  Serial.println(data[8]);
////  Serial.println(data[9]);
////}
////
////
////// interupt handler
////void timer_interrupt_handler(void){
////  time = millis();
////  sensors_event_t event; 
////  accel.getEvent(&event);
////
//////  buffer[j][i] = time ;
////  buffer[j][i] = event.acceleration.x;
////  i += 1;
////
////  if (i == size) {
////    //timerTriggered = true;
////    i = 0;
////    print_data(buffer[j]);
////    j++;
////    if (j == size) {
////      j = 0;
////    }
////  }
////
////}
////
////// ----------- setup -----------
////void setup(void) 
////{
////  #ifndef ESP8266
////    while (!Serial); // for Leonardo/Micro/Zero
////  #endif
////
////  // ---------- SD card setup ----------
////  Serial.begin(115200);
////
////  // setup for the SD card
////  Serial.print("Initializing SD card...");
////
////  if(!SD.begin(chipSelect)) {
////    Serial.println("initialization failed!");
////    return;
////  }
////  Serial.println("initialization done.");
////    
////  //open file
////  if (SD.exists("LOGDATA.txt")) {
////    SD.remove("LOGDATA.txt"); 
////  }
////  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);
////
////  // if the file opened ok, write to it:
////  if (dataFile) {
////    Serial.println("File opened ok");
////    // print the headings for our data
//////    dataFile.println("TIME");
//////    dataFile.print("accel1_x"); dataFile.print(","); 
//////    Serial.print("accel1_x"); dataFile.print(","); 
//////    dataFile.print("accel1_y"); dataFile.print(",");
//////    Serial.print("accel1_y"); dataFile.print(","); 
//////    dataFile.print("accel1_z"); dataFile.print(","); 
//////    Serial.print("accel1_z"); dataFile.print(","); 
////  } else {
////    Serial.println("File not opened");
////  }
////  dataFile.close();
////
////  Serial.println("CLEARDATA"); //clears up any data left from previous projects
////  Serial.println("Time"); //always write LABEL, to indicate it as first line
////  Serial.println("RESETTIMER");
////
////
////  // ---------- accelerometer setup ----------
////  Serial.println("Accelerometer Test"); Serial.println("");
////  
////  /* Initialise the sensor */
////  if(!accel.begin())
////  {
////    /* There was a problem detecting the ADXL345 ... check your connections */
////    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
////    while(1);
////  }
////
////  /* Set the range to whatever is appropriate for your project */
////  // accel.setRange(ADXL345_RANGE_16_G);
////  // accel.setRange(ADXL345_RANGE_8_G);
////  // accel.setRange(ADXL345_RANGE_4_G);
////  accel.setRange(ADXL345_RANGE_2_G);
////  
////  /* Display some basic information on this sensor */
//////  displaySensorDetails();
////  
////  /* Display additional settings (outside the scope of sensor_t) */
//////  displayDataRate();
//////  displayRange();
//////  Serial.println("");
////
////  // set pins to output mode
////  pinMode(4, OUTPUT);
////  pinMode(5, OUTPUT);
////  pinMode(6, OUTPUT);
////  pinMode(7, OUTPUT);
////
////  // ---------- Timer setup ----------
////  //Timer1.initialize(1000000); //Initialize timer with 1 second 
////  //Timer1.initialize(1000); //Initialize timer with 1 millisecond period
////  Timer1.initialize(); //Initialize timer with 1 millisecond period
////  Timer1.setPeriod(1000);
////  Timer1.attachInterrupt(timer_interrupt_handler);
////  delay(100);
////}
////
////
////// ------------------------------
////
////
////// ----------- loop -------------
////
////void loop()
////{
//////  sensors_event_t event; 
//////  accel.getEvent(&event);
////
//////  if (timerTriggered)
//////  {
//////    for (int m = 0; m < size; m++) {
//////      for (int n = 0; n < size; n++) {
//////        Serial.println(buffer[m][n]);
//////      }
//////    }
//////    timerTriggered = false;
////
//////
//////    Serial.print(" X: "); Serial.print(event.acceleration.x); Serial.print("  ");
//////    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
//////    Serial.print("Z: "); Serial.println(event.acceleration.z);
//////  }
////}
//
//
//


#include <Wire.h>

void setup() {
Serial.begin(9600);
Wire.begin();

pinMode(4, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);

Wire.beginTransmission(0x53);
Wire.write(0x2C); 
Wire.write(0x08); 
Wire.endTransmission();

Wire.beginTransmission(0x53);
Wire.write(0x31); 
Wire.write(0x08); 
Wire.endTransmission();

Wire.beginTransmission(0x53);
Wire.write(0x2D); 
Wire.write(0x08); 
Wire.endTransmission();

}

void loop() {
Wire.beginTransmission(0x53);
Wire.write(0x32); 
Wire.endTransmission();
Wire.requestFrom(0x53, 1);
byte x0 = Wire.read();

Wire.beginTransmission(0x53);
Wire.write(0x33); 
Wire.endTransmission();
Wire.requestFrom(0x53, 1);
byte x1 = Wire.read();
x1 = x1 & 0x03;

uint16_t x = (x1 << 8) + x0;
int16_t xf = x;
if(xf > 511)
{
xf = xf - 1024;
}
float xa = xf * 0.004;
Serial.print("X = "); 
Serial.print(xa);
Serial.print(" g"); 
Serial.println(); 


Wire.beginTransmission(0x53);
Wire.write(0x34); 
Wire.endTransmission();
Wire.requestFrom(0x53, 1);
byte y0 = Wire.read();

Wire.beginTransmission(0x53);
Wire.write(0x35); 
Wire.endTransmission();
Wire.requestFrom(0x53, 1);
byte y1 = Wire.read();
y1 = y1 & 0x03;

uint16_t y = (y1 << 8) + y0;
int16_t yf = y;
if(yf > 511)
{
yf = yf - 1024;
}
float ya = yf * 0.004;
Serial.print("Y = "); 
Serial.print(ya);
Serial.print(" g"); 
Serial.println(); 

Wire.beginTransmission(0x53);
Wire.write(0x36); 
Wire.endTransmission();
Wire.requestFrom(0x53, 1);
byte z0 = Wire.read();

Wire.beginTransmission(0x53);
Wire.write(0x37); 
Wire.endTransmission();
Wire.requestFrom(0x53, 1);
byte z1 = Wire.read();
z1 = z1 & 0x03;

uint16_t z = (z1 << 8) + z0;
int16_t zf = z;
if(zf > 511)
{
zf = zf - 1024;
}
float za = zf * 0.004;
Serial.print("Z = "); 
Serial.print(za);
Serial.print(" g"); 
Serial.println(); 
Serial.println(); 
delay(500);
}
