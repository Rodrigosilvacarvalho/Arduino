#include <ESP8266WiFi.h>
#include "mPower.h"

const char* ssid     = "...";
const char* password = "...";
const char* host = "...";


mPower device = mPower(host);

void setup() {
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}


void loop() {

    Serial.printf("getting status\n");
    if(device.getStatus()){
        for(int i=0; i<6; i++)
              Serial.printf("port %i: %s %fW %fPF \n",i,device.enabled[i]==true?"on ":"off",device.power[i],device.powerFactor[i]);
        Serial.printf("available ports: %d\n", device.portsAvailable);

        device.enabled[2] = !device.enabled[2];
        device.setPorts();
        
    } else {
        Serial.printf("not available\n");
    }

    delay(10000);
}

