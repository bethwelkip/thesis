#include <ESP8266WiFi.h>;

void setup()
{

    // start serial monitor
    Serial.begin(115200);
    Serial.println("Starting up .............");
    int count = 0;
    // while loop to connect to wifi
    WiFi.begin("Servicenet", "password");
    while (Wifi.status() != WL_CONNECTED && count < 1000)
    {
        delay(1000);
        Serial.print("... ");
        count++;
    }

    if (WiFi.status == WL_CONNECTED)
    {
        Serial.print("Connected, IP address: ");
        Serial.println(WiFi.localIP());
    }
    // set up as client for servicenet

    // set up as soft station for the others

    // connect to all the others
}

void loop()
{

    // connect to all the others

    // receive the data and push online
}