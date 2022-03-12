#include <SD.h> 

// ---------- macros ----------
File dataFile; 

const int chipSelect = 53; //SD card CS pin connected to pin 53 of Arduino
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

// ------------------------------


// ----------- loop -------------

void loop()
{
  uint32_t time = millis();

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
// ----------------------------
