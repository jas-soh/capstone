#include "Simple_ADXL345.h"

/* NOTE: Don't forget to use Serial.begin() when activating the debug mode */

/* Creating an ADXL345 object
   ADXL345 adxl(communication type, cs pin, enable debug, debug port)
   -> Communication type
      The sensor can be configured to SPI (ADXL345_COMM_SPI) or I2C (ADXL345_COMM_I2C).
   -> CS pin
      Pin to select the sensor on SPI communication. Can be any value on I2C mode. If not selected, the default value will be 0.
   -> Enable Debug
      By default, the debug is disabled. If you want to enable, you can do it by setting this parameter as true.
      Don't worry, you can also configure it later.
   -> Debug Port
      You can use another serial port on debug mode when your board has more then one Serial port.
      Don't worry, you can also configure it later.
*/

#define ADXL345_SPI_CS_PIN 10

/* Using sensor on SPI mode */
//ADXL345 adxl(ADXL345_COMM_SPI, ADXL345_SPI_CS_PIN); // Normal mode
// ADXL345 adxl(ADXL345_COMM_SPI, ADXL345_SPI_CS_PIN, true); // Debug messages enabled
// ADXL345 adxl(ADXL345_COMM_SPI, ADXL345_SPI_CS_PIN, true, &Serial2); // Enabling the debug on Serial2 port

/* Using sensor on I2C mode */
//ADXL345 adxl(ADXL345_COMM_I2C); // Normal mode
 ADXL345 adxl(ADXL345_COMM_I2C, 0, true); // Debug messages enabled. Don't care about the cs pin.
// ADXL345 adxl(ADXL345_COMM_I2C, 0, true, &Serial2); // Enabling the debug on Serial2 port

void setup()
{
  Serial.begin(9600);

  /*
     This function sets the acceleration that corresponds the
     sensor full scale.
     The ADXL345 accelerometer has a 10 bit resolution, so it's
     output values may vary from 512 to -511.
     The available values for full scale range are: 2G, 4G, 8G and 16G.
     If you don't set it, the default value will be 2G.
  */
  adxl.setFullScaleRange(2);
  // adxl.setFullScaleRange(4);
  // adxl.setFullScaleRange(8);
  // adxl.setFullScaleRange(16);

  /* You can also get the full scale range currently configured */
  int range = adxl.getFullScaleRange();
  Serial.println(range);

  /*
     When powered on, the sensor is converting acceleration data and
     storing it on it's memory.
     You can use the configuration commands before or after powering on.
  */
  adxl.powerOn();

  /*
     This function sets the output data rate, also named sampling frequency.
     The ADXL345 sensor supports the following output data rates:
     -> 6.25 Hz
     -> 12.5 Hz
     -> 25 Hz
     -> 50 Hz
     -> 100 Hz
     -> 200 Hz
     -> 400 Hz
     -> 800 Hz
     -> 1600 Hz
     -> 3200 Hz
  */
  // adxl.setDataRate(6.25);
  // adxl.setDataRate(12.5);
  // adxl.setDataRate(25);
  // adxl.setDataRate(50);
  adxl.setDataRate(100);
  // adxl.setDataRate(200);
  // adxl.setDataRate(400);
  // adxl.setDataRate(800);
  // adxl.setDataRate(1600);
  // adxl.setDataRate(3200);

  int frequency = adxl.getDataRate(); // get the current data rate
  Serial.println(frequency);

  // adxl.powerOff(); // Stops the acceleration conversion.

  /* This function returns the device ID byte. On this sensor,
     the device ID is 0xE5h. It's useful to detect if the sensor
     is working.
  */
  byte devId = adxl.getDeviceId();
  Serial.println(devId, HEX);

  adxl.enableDebug(); // Enable the debug via serial
  adxl.disableDebug(); // Disable the debug via serial

  bool debug = adxl.isDebugEnabled(); // Check if the debug is enabled
  Serial.println(debug);

  adxl.setSerialDebugPort(&Serial); // Set the serial port used for debug
}

void loop()
{
  /* If you are using an ATMEGA328 based board
     or another one that the int and int16 are equal,
     you can use both.
     For some microcontrollers, an int variable doesn't have
     16 bits, so you must use int16_t.
  */
  int16_t x, y, z;
  int ix, iy, iz;

  /* Getting the sensor readings */
  x = adxl.readX();
  y = adxl.readY();
  z = adxl.readZ();

  ix = adxl.readX();
  iy = adxl.readY();
  iz = adxl.readZ();

  Serial.println(String(x) + " | " + String(y) + " | " + String(z));
  Serial.println(String(ix) + " | " + String(iy) + " | " + String(iz));
  /* You can also try:
     (Use it only if you fell confortable with &)
  */
  adxl.readAllAxes(&x, &y, &z);
  Serial.println(String(x) + " | " + String(y) + " | " + String(z));

  delay(10);
}
