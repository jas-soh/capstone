/*
 * ADXL345.c - Basic library for ADXL345 sensor
 *
 * MIT License
 *
 * Copyright (c) 2017 SGN Robotica Educacional
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Wire.h>
#include <SPI.h>

#include "Simple_ADXL345.h"

/********************** Sensor registers *******************/
#define ADXL345_REG_DEVID               0x00  // Device ID.
#define ADXL345_REG_THRESH_TAP          0x1D  // Tap threshold.
#define ADXL345_REG_OFSX                0x1E  // X-axis offset.
#define ADXL345_REG_OFSY                0x1F  // Y-axis offset.
#define ADXL345_REG_OFSZ                0x20  // Z-axis offset.
#define ADXL345_REG_DUR                 0x21  // Tap duration.
#define ADXL345_REG_Latent              0x22  // Tap latency.
#define ADXL345_REG_Window              0x23  // Tap window.
#define ADXL345_REG_THRESH_ACT          0x24  // Activity threshold.
#define ADXL345_REG_THRESH_INACT        0x25  // Inactivity threshold.
#define ADXL345_REG_TIME_INACT          0x26  // Inactivity time.
#define ADXL345_REG_ACT_INACT_CTL       0x27  // Axis enable control for activity and inactivity detection.
#define ADXL345_REG_THRESH_FF           0x28  // Free-fall threshold.
#define ADXL345_REG_TIME_FF             0x29  // Free-fall time.
#define ADXL345_REG_TAP_AXES            0x2A  // Axis control for tap/double tap.
#define ADXL345_REG_ACT_TAP_STATUS      0x2B  // Source of tap/double tap.
#define ADXL345_REG_BW_RATE             0x2C  // Data rate and power mode control.
#define ADXL345_REG_POWER_CTL           0x2D  // Power-saving features control.
#define ADXL345_REG_INT_ENABLE          0x2E  // Interrupt enable control.
#define ADXL345_REG_INT_MAP             0x2F  // Interrupt mapping control.
#define ADXL345_REG_INT_SOURCE          0x30  // Source of interrupts.
#define ADXL345_REG_DATA_FORMAT         0x31  // Data format control.
#define ADXL345_REG_DATAX0              0x32  // X-Axis Data 0.
#define ADXL345_REG_DATAX1              0x33  // X-Axis Data 1.
#define ADXL345_REG_DATAY0              0x34  // Y-Axis Data 0.
#define ADXL345_REG_DATAY1              0x35  // Y-Axis Data 1.
#define ADXL345_REG_DATAZ0              0x36  // Z-Axis Data 0.
#define ADXL345_REG_DATAZ1              0x37  // Z-Axis Data 1.
#define ADXL345_REG_FIFO_CTL            0x38  // FIFO control.
#define ADXL345_REG_FIFO_STATUS         0x39  // FIFO status.


#define ADXL345_BYTE_DEVICE_ID          0xE5
#define ADXL345_BIT_PWR_REG_MEASURE    (1 << 3)

/*! ADXL345 ID */
#define ADXL345_I2C_ADDRESS             0x10


ADXL345::ADXL345(ADXL345_COMM_t commType, uint8_t csPin, bool debug, Stream *serialDebugPort)
{
    communicationType = commType;
    isDebugEnabledFlag = debug;
    debugPort = serialDebugPort;

    _csPin = csPin;

    if(communicationType == ADXL345_COMM_SPI)
    {
        sendDebugMessage("INFO", "Communication type: SPI");
        SPI.begin();
        SPI.setDataMode(SPI_MODE3);
        pinMode(csPin, OUTPUT);
        digitalWrite(csPin, HIGH);
    }
    else if(communicationType == ADXL345_COMM_I2C)
    {
        sendDebugMessage("INFO", "Communication type: I2C");
        Wire.begin();
    }
    else
    {
        sendDebugMessage("ERROR", "Invalid communication type.");
    }
}

int16_t ADXL345::readX()
{
    int16_t buffer;
    readRegs(ADXL345_REG_DATAX0, (uint8_t *) &buffer, 2);
    return buffer;
}

int16_t ADXL345::readY()
{
    int16_t buffer;
    readRegs(ADXL345_REG_DATAY0, (uint8_t *) &buffer, 2);
    return buffer;
}

int16_t ADXL345::readZ()
{
    int16_t buffer;
    readRegs(ADXL345_REG_DATAZ0, (uint8_t *) &buffer, 2);
    return buffer;
}

void ADXL345::readAllAxes(int16_t *x, int16_t *y, int16_t *z)
{
    int16_t buffer[3];
    readRegs(ADXL345_REG_DATAX0, (uint8_t *) buffer, 6);
    *x = buffer[0];
    *y = buffer[1];
    *z = buffer[2];
}

void ADXL345::setDataRate(float dataRate)
{
    uint8_t buffer;
    sendDebugMessage("INFO", "Setting Output Data Rate to " + String(dataRate));

    if(dataRate == 6.25) buffer = 0b0110;
    else if(dataRate == 12.5) buffer = 0b0111;
    else if(dataRate == 25) buffer = 0b1000;
    else if(dataRate == 50) buffer = 0b1001;
    else if(dataRate == 100) buffer = 0b1010;
    else if(dataRate == 200) buffer = 0b1011;
    else if(dataRate == 400) buffer = 0b1100;
    else if(dataRate == 800) buffer = 0b1101;
    else if(dataRate == 1600) buffer = 0b1110;
    else if(dataRate == 3200) buffer = 0b1111;
    else
    {
        buffer = 0b1111;
        sendDebugMessage("WARNING", "Invalid input. Setting output data rate to 3200Hz (default)");
    }

    buffer |= (readReg(ADXL345_REG_BW_RATE) & !0x0F);
    sendDebugMessage("INFO", "Writing to register " + String(ADXL345_REG_BW_RATE) + " the data " + String(buffer, HEX));
    writeReg(ADXL345_REG_BW_RATE, buffer);
}

float ADXL345::getDataRate()
{
    float dataRate;
    uint8_t buff;

    buff = readReg(ADXL345_REG_BW_RATE) & 0x0F;

    switch(buff)
    {
        case 0b0110:
            dataRate = 6.25;
            break;

        case 0b0111:
            dataRate = 12.5;
            break;

        case 0b1000:
            dataRate =  25;
            break;

        case 0b1001:
            dataRate =  50;
            break;

        case 0b1010:
            dataRate =  100;
            break;

        case 0b1011:
            dataRate =  200;
            break;

        case 0b1100:
            dataRate =  400;
            break;

        case 0b1101:
            dataRate =  800;
            break;

        case 0b1110:
            dataRate =  1600;
            break;

        case 0b1111:
            dataRate =  3200;
            break;
    }

    return dataRate;
}

void ADXL345::setFullScaleRange(uint8_t fullScaleRange)
{
    uint8_t buffer;
    sendDebugMessage("INFO", "Setting Full Scale Range to " + String(fullScaleRange) + "G");

    switch(fullScaleRange)
    {
        case 2:
            buffer = 0b00;
            break;

        case 4:
            buffer = 0b01;
            break;

        case 8:
            buffer = 0b10;
            break;

        case 16:
            buffer = 0b11;
            break;

        default:
            sendDebugMessage("WARNING", "Invalid input. Setting output range to 2G (default)");
            buffer = 0b00;
            break;
    }
    buffer |= readReg(ADXL345_REG_DATA_FORMAT) & !0x03;
    sendDebugMessage("INFO", "Writing to register " + String(ADXL345_REG_DATA_FORMAT) + " the data " + String(buffer, HEX));
    writeReg(ADXL345_REG_DATA_FORMAT, buffer);
}

uint8_t ADXL345::getFullScaleRange()
{
    uint8_t range;
    uint8_t buff;
    buff = readReg(ADXL345_REG_DATA_FORMAT) & 0x03;

    switch(buff)
        {
            case 0b00:
                range = 2;
                break;

            case 0b01:
                range = 4;
                break;

            case 0b10:
                range = 8;
                break;

            case 0b11:
                range = 16;
                break;
        }
    return range;
}

void ADXL345::powerOn()
{
    uint8_t buffer;
    sendDebugMessage("INFO", "Turning sensor on. Starting acceleration sampling");
    buffer = readReg(ADXL345_REG_POWER_CTL) | ADXL345_BIT_PWR_REG_MEASURE;
    writeReg(ADXL345_REG_POWER_CTL, buffer);
}

void ADXL345::powerOff()
{
    uint8_t buffer;
    sendDebugMessage("INFO", "Turning sensor off. Halting acceleration sampling");
    buffer = readReg(ADXL345_REG_POWER_CTL) & !ADXL345_BIT_PWR_REG_MEASURE;
    writeReg(ADXL345_REG_POWER_CTL, buffer);
}


uint8_t ADXL345::getDeviceId()
{
    sendDebugMessage("INFO", "Getting the device ID.");
    uint8_t buffer = readReg(ADXL345_REG_DEVID);
    if(buffer == ADXL345_BYTE_DEVICE_ID)
    {
        sendDebugMessage("INFO", "Expected device ID");
    }
    else
    {
        sendDebugMessage("ERROR", "Can't get the expected device ID. Expected: " + String(ADXL345_BYTE_DEVICE_ID, HEX) + " Read: " + String(buffer));
    }
    return buffer;
}

void ADXL345::enableDebug()
{
    isDebugEnabledFlag = true;
}

void ADXL345::disableDebug()
{
    isDebugEnabledFlag = false;
}

bool ADXL345::isDebugEnabled()
{
    return isDebugEnabledFlag;
}

void ADXL345::sendDebugMessage(String tag, String data)
{
    if(isDebugEnabledFlag && debugPort != NULL)
    {
        debugPort->print("[ADXL345][" + tag + "]: ");
        debugPort->println(data);
    }
}

void ADXL345::setSerialDebugPort(Stream *port)
{
    debugPort = port;
}

void ADXL345::writeReg(uint8_t regAddr, uint8_t data)
{
    if(communicationType == ADXL345_COMM_I2C)
    {
        Wire.beginTransmission(ADXL345_I2C_ADDRESS);
        Wire.write(regAddr);
        Wire.write(data);
        Wire.endTransmission();
    }
    else if(communicationType == ADXL345_COMM_SPI)
    {
        digitalWrite(_csPin, LOW);
        SPI.transfer(regAddr);
        SPI.transfer(data);
        digitalWrite(_csPin, HIGH);
    }
}

uint8_t ADXL345::readReg(uint8_t regAddr)
{
    uint8_t buffer;

    if(communicationType == ADXL345_COMM_I2C)
    {
        Wire.beginTransmission(ADXL345_I2C_ADDRESS);
        Wire.write(regAddr);
        Wire.endTransmission(false);
        Wire.requestFrom(ADXL345_I2C_ADDRESS, 1, true);

        buffer = Wire.read();
    }
    else if(communicationType == ADXL345_COMM_SPI)
    {
        uint8_t spiAddress = 0x80 | regAddr;

        digitalWrite(_csPin, LOW);
        SPI.transfer(spiAddress);
        buffer = SPI.transfer(0x00);
        digitalWrite(spiAddress, HIGH);
    }

    return buffer;
}

void ADXL345::readRegs(uint8_t regAddr, uint8_t *pBuffer, uint8_t size)
{
    if(pBuffer == NULL) return;

    if(communicationType == ADXL345_COMM_I2C)
    {
        Wire.beginTransmission(ADXL345_I2C_ADDRESS);
        Wire.write(regAddr);
        Wire.endTransmission();

        Wire.requestFrom(ADXL345_I2C_ADDRESS, (int) size, true);

        while(Wire.available())
        {
            *pBuffer++ = (uint8_t) Wire.read();
        }
    }
    else if(communicationType == ADXL345_COMM_SPI)
    {
        uint8_t spiAddress = 0x80 | regAddr;

        if(size > 1)
        {
            spiAddress = spiAddress | 0x40;
        }

        digitalWrite(_csPin, LOW);
        SPI.transfer(spiAddress);
        for(uint8_t i = 0; i < size; i++)
        {
            *pBuffer++ = SPI.transfer(0x00);
        }
        digitalWrite(_csPin, HIGH);
    }
}
