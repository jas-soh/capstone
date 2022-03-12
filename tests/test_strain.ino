#include <SPI.h> 
#include <WheatstoneBridge.h>

// ---------- macros ----------
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

// -----------------------------


// ----------- setup -----------
void setup()
{
  Serial.begin(9600);

  
  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("Time"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");
}

// ------------------------------


// ----------- loop -------------

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
  Serial.println(valRaw3);
  
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
    
  delay(3000); //Wait for 3 seconds before writing the next data 
}
// ----------------------------
