#include <SD.h> 
#include <SPI.h>

// ---------- macros ----------
File dataFile; 

const int chipSelect = 10; //SD card CS pin connected to pin 53 of Arduino
// -----------------------------


// ----------- setup -----------
void setup()
{
  Serial.begin(9600);

  // setup for the SD card
  Serial.print("Initializing SD card...");

  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
    
  //open file
  SD.remove("LOGDATA.txt");  
  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (dataFile) {
    Serial.println("File opened ok");
    // print the headings for our data
    dataFile.println("Time");
    dataFile.print("accel1_x"); dataFile.print(","); 
    Serial.print("accel1_x"); dataFile.print(","); 
    dataFile.print("accel1_y"); dataFile.print(",");
    Serial.print("accel1_y"); dataFile.print(","); 
    dataFile.print("accel1_z"); dataFile.print(","); 
    Serial.print("accel1_z"); dataFile.print(","); 
  } else {
    Serial.println("File not opened");
  }
  dataFile.close();

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("Time"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");
}

// ------------------------------


// ----------- loop -------------

void loop()
{
  uint32_t time = millis();


  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(time); 

    dataFile.println();
    dataFile.close(); //Close the file
  }
  else
    Serial.println("SD card writing failed");

  Serial.print("Time: "); //always write "DATA" to Indicate the following as Data
  Serial.print(time);
  Serial.println(); //End of Row move to next row
    
 delay(3000); //Wait for 3 seconds before writing the next data 
}
