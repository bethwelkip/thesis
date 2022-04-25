

/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings

#define BLYNK_TEMPLATE_ID "TMPL4Pgc2RfL"
#define BLYNK_DEVICE_NAME "MAE D1 mini 11"
#define BLYNK_AUTH_TOKEN "px65AxFrcajcQS0DBQu8oZ4pNU3AzRB-"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>

SensirionI2CScd4x scd4x;


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "servicenet";
char pass[] = "";

BlynkTimer timer;



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

void myTimerEvent()
{
   uint16_t error;
   char errorMessage[256];

    delay(5000);

    // Declare variables
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
    } 
    
    
    else {
        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print("\t");
        Serial.print("Temperature:");
        Serial.print(temperature);
        Serial.print("\t");
        Serial.print("Humidity:");
        Serial.println(humidity);
        
    }
    // delay(5000);

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, co2);
  Blynk.virtualWrite(V3, humidity);

  Serial.println("Now going to sleep");
  
  
  ESP.deepSleep(10e6);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Sets up the CO2 sensor
  Wire.begin();

  
  
  uint16_t error;
  char errorMessage[256];

    scd4x.begin(Wire);

    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }



    // Start Measurement
   
    error = scd4x.startLowPowerPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    Serial.println("Waiting for first measurement... (5 sec)");
  



}

void loop()
{
  
    myTimerEvent();
}
