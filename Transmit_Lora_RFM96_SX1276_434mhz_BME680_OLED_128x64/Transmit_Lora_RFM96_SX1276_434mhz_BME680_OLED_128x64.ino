
#include <RadioLib.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C
// RFM96 / SX1276 has the following connections:
SX1278 radio = new Module(8, 3, 4, 7);


void setup() {
  
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin(434, 125.0, 9, 7, RADIOLIB_SX127X_SYNC_WORD, 20, 8, 0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  Serial.println(F("BME680 test"));
    if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

}

void loop() {
  
  bme.performReading(); 
  
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");
  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");
  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  
  Serial.print(F("[SX1278] Transmitting packet ... "));

  int temp = bme.temperature;
  int press = (bme.pressure / 100.0);
  int hum = bme.humidity;
  int gas = (bme.gas_resistance / 1000.0);
  int alt = (bme.readAltitude(SEALEVELPRESSURE_HPA));
  String bme = String(temp) + "*C "+ String(hum) + " % "+ String(press) + " hPa "+ String(alt) + " m "+ String(gas) + " KOhms";

  int state = radio.transmit(bme);

  // wait for a second before transmitting again
  delay(5000);


  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F(" success!"));

    // print measured data rate
    Serial.print(F("[SX1278] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));
  }

}
