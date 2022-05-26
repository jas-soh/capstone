#include <SoftwareSerial.h>

SoftwareSerial BT_Serial(8,9); // RX, TX

bool bytesWereRead = false;
char c = ' ';
boolean NL = true;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BT_Serial.begin(9600);

}

void loop()
{
  if (BT_Serial.available()) {
    bytesWereRead = true;
    int bytesRead = Serial.write(BT_Serial.read());
  }

  if (Serial.available()) {
    c = Serial.read();
    if (c!=10 & c!=13){
      BT_Serial.write(c);
    }
    if (NL) {Serial.print("\r\n"); NL = false;}
    Serial.write(c);
    if (c==10){NL = true;}
  }
}
