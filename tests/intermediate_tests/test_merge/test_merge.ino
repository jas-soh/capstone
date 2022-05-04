#include <SD.h>

void setup() {
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void merge_files() {
  dataFile = SD.open("LOGDATA.txt");
  strainFile = SD.open("STRAIN.txt");
  finalFile = SD.open("FINAL_DATA.txt", FILE_WRITE);

  String accelString = "";
  String strainString = "";

  
  if (dataFile.available()) {
     accelString = dataFile.readStringUntil('\n');
  }

  if (strainFile.available()) {
     strainString = strainFile.readStringUntil('\n');
  }

  finalFile.print(accelString); finalFile.print(","); finalFile.println(strainString);



  dataFile.close();
  strainFile.close();
  finalFile.close();

 
}
