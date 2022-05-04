#include <TimerOne.h>
#include <SD.h> 
#include <SPI.h> 

// ---------- macros ----------
File dataFile;
const int chipSelect = 53; //SD card CS pin connected to pin 53 of Arduino 
volatile bool timerTriggered = false;
uint32_t time = 0;

// ---------- setup -----------
void setup() {
   // put your setup code here, to run once:
   Serial.begin(9600);
   Timer1.initialize(1000000); //Initialize timer with 1 second period
   Timer1.attachInterrupt(printFunc);
  
   Serial.begin(9600);

  // setup for the SD card
  Serial.print("Initializing SD card...");
  
  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while(1)
    return;
  }
  Serial.println("initialization done.");
    
  //open file
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);
  
  // if the file opened ok, write to it:
  if (dataFile) {
    Serial.println("File opened ok");
    // print the headings for our data
    dataFile.println("Time");
  }
  dataFile.close();
  
  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("Time"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");
}


// ---------- main ----------
void loop() {
//  // put your main code here, to run repeatedly:
  if(timerTriggered){
    timerTriggered = false; // clear bit
    Serial.println("Timer was triggered");
//

  //open file
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(time);
    dataFile.println(); //End of Row move to next row

    dataFile.close(); //Close the file
  }
  else
    Serial.println("SD card writing failed");

  Serial.print("Time: "); //always write "DATA" to Indicate the following as Data
  Serial.print(time);
  Serial.println(); //End of Row move to next row
  }
}
void printFunc(void){
  time = millis();
  timerTriggered = true;
}
