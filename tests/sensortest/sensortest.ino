#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <TimerOne.h>
#include <SD.h> 
#include <SPI.h> 

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
volatile bool timerTriggered = false;
int i = 1;
uint32_t time = 0;
const int chipSelect = 10; //SD card CS pin connected to pin 53 of Arduino
File dataFile;


void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}

// interupt handler
void timer_interrupt_handler(void){
  time = millis();
  timerTriggered = true;
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
    return;
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
//    dataFile.println("TIME");
//    dataFile.print("accel1_x"); dataFile.print(","); 
//    Serial.print("accel1_x"); dataFile.print(","); 
//    dataFile.print("accel1_y"); dataFile.print(",");
//    Serial.print("accel1_y"); dataFile.print(","); 
//    dataFile.print("accel1_z"); dataFile.print(","); 
//    Serial.print("accel1_z"); dataFile.print(","); 
  } else {
    Serial.println("File not opened");
  }
  dataFile.close();

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("Time"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");


  // ---------- accelerometer setup ----------
  Serial.println("Accelerometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  // accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  accel.setRange(ADXL345_RANGE_2_G);
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");

  // set pins to output mode
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // ---------- Timer setup ----------
  Timer1.initialize(1000000); //Initialize timer with 1 second period
  Timer1.attachInterrupt(timer_interrupt_handler);
  delay(100);
}


void loop(void) 
{

  // writing to SD card
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (!dataFile) {
    Serial.println("SD card writing failed");
    while(1) {}
  }

  while(!(timerTriggered)) {}

  timerTriggered = false;

  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);

  switch(i) {
    case 1 :
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(4, HIGH);
      //delay(100);
      Serial.println(i);

      // write to sd card
//      dataFile.print(i); dataFile.print(":   ");
      dataFile.print(time); dataFile.print(",");
      dataFile.print(event.acceleration.x); dataFile.print(",");
      dataFile.print(event.acceleration.y); dataFile.print(",");
      dataFile.print(event.acceleration.z); dataFile.print(",");
      i = 2;
      break;
    case 2 :
      digitalWrite(7, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(4, HIGH);
      //delay(100);
      Serial.println(i);

      // write to sd card
//      dataFile.print(i); dataFile.print(":   ");
      dataFile.print(event.acceleration.x); dataFile.print(",");
      dataFile.print(event.acceleration.y); dataFile.print(",");
      dataFile.print(event.acceleration.z); dataFile.print(",");
      i = 3;
      break;
   case 3 :
      digitalWrite(7, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH);
      //delay(100);
      Serial.println(i);

      // write to sd card
//      dataFile.print(i); dataFile.print(":   ");
      dataFile.print(event.acceleration.x); dataFile.print(",");
      dataFile.print(event.acceleration.y); dataFile.print(",");
      dataFile.print(event.acceleration.z); // dataFile.print(",");
      dataFile.println();
      i = 1;
      break;

   case 4 :
      digitalWrite(7, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
      //delay(100);
      Serial.println(i);
      i = 1;

      // write to sd card
      dataFile.print(event.acceleration.x); dataFile.print(",");
      dataFile.print(event.acceleration.y); dataFile.print(",");
      dataFile.print(event.acceleration.z); // dataFile.print(",");
      break;
      
    default : // should never get here
      digitalWrite(7, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(4, HIGH);
      //delay(100);
      Serial.println("default");
      i = 2;
      break;
  }

  Serial.print("time: ");
  Serial.println(time);
  Serial.print("digital read 7: ");
  Serial.println(digitalRead(7));
  Serial.print("digital read 6: ");
  Serial.println(digitalRead(6));
  Serial.print("digital read 5: ");
  Serial.println(digitalRead(5));
  Serial.print("digital read 4: ");
  Serial.println(digitalRead(4));


  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  dataFile.close();
  //delay(500);
}
