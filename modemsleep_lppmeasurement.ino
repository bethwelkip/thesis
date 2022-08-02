 /*************************************************************
Wemos D1 mini -> Modem Sleep
SCD41 -> Periodic Measurement
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLizwEYxIF"
#define BLYNK_DEVICE_NAME "MAE D1 mini 07"
#define BLYNK_AUTH_TOKEN "mNp53OSnvMEG7BC3KCA3irMwAyzqM_H5"
/////////////////////////////////////////////////////////////////////////////////

// Include Libraries
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#define BLYNK_PRINT Serial

// Dependencies
SensirionI2CScd4x scd4x;
WiFiClient wifi;
WiFiClientSecure client;
BlynkTimer timer;

// Wi-Fi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "servicenet";
char pass[] = "";

/////////////////////////////////////////////////////////////////////////////////

// functions to read and print serial numbers adapted from scd41_sample
void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}

/////////////////////////////////////////////////////////////////////////////////

// Helper function for reading measurements
void measure() {
  uint16_t error;
    char errorMessage[256];

    delay(5000);

    // Read Measurement
    uint16_t co2;
    float temperature;
    float humidity;
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping.");
    } else {
        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print("\t");
        Serial.print("Temperature:");
        Serial.print(temperature);
        Serial.print("\t");
        Serial.print("Humidity:");
        Serial.println(humidity);
    }
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, co2);
  Blynk.virtualWrite(V2, humidity);
  }

///////////////////////////////////////////////////////////////////////////////// 

void setup() {
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  Wire.begin();

  uint16_t error;
  char errorMessage[256];

    scd4x.begin(Wire);
    
    error = scd4x.startLowPowerPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        printSerialNumber(serial0, serial1, serial2);
    }

    Serial.println("Waiting for first measurement... (5 sec)");
  

}

/////////////////////////////////////////////////////////////////////////////////
void loop() {
  Blynk.run();
  measure();
  delay(25000);
}
