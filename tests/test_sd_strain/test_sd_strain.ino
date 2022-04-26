#include <SD.h> 
#include <SPI.h> 
#include <WheatstoneBridge.h>

// ---------- macros ----------
File dataFile; 

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

const int chipSelect = 10; //SD card CS pin connected to pin 53 of Arduino

void setup() {
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
    dataFile.println("Time,valRaw1,valRaw2,valRaw3,valRaw4");
  }
  dataFile.close();

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("Time"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");
}

void loop()
{

  // Read strain 1
  val1 = wsb_strain1.measureForce();
  valRaw1 = wsb_strain1.getLastForceRawADC();
  Serial.print("strain1: ");
  Serial.print(val1);
  Serial.print(", strain1 raw: ");
  Serial.println(valRaw1);
  
  // Read strain 2
  val2 = wsb_strain2.measureForce();
  valRaw2 = wsb_strain2.getLastForceRawADC();
  Serial.print("strain2: ");
  Serial.print(val3);
  Serial.print(", strain2 raw: ");
  Serial.println(valRaw2);
  
  // Read strain 3
  val3 = wsb_strain3.measureForce();
  valRaw3 = wsb_strain3.getLastForceRawADC();
  Serial.print("strain3: ");
  Serial.print(val3);
  Serial.print(", strain3 raw: ");
  Serial.println(valRaw3);
  
  // Read strain 4
  val4 = wsb_strain4.measureForce();
  valRaw4 = wsb_strain4.getLastForceRawADC();
  Serial.print("strain4: ");
  Serial.print(val4);
  Serial.print(", strain4 raw: ");
  Serial.println(valRaw4);

  //Serial.println(); //End of Row move to next row

  uint32_t time = millis();

  dataFile = SD.open("LOGDATA.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(time); 
    dataFile.print(","); 
    dataFile.print(valRaw1); 
    dataFile.print(","); 
    dataFile.print(valRaw2); 
    dataFile.print(","); 
    dataFile.print(valRaw3); 
    dataFile.print(","); 
    dataFile.print(valRaw4); 

    dataFile.println();
    dataFile.close(); //Close the file
  }
  else
    Serial.println("SD card writing failed");


    
  delay(3000); //Wait for 3 seconds before writing the next data 
}
