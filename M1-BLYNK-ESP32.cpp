#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

char auth[] = "<token>";
char ssid[] = "<ssid>";
char pass[] = "<password>";

int pirPin = D1;  
int reedPin = D2;  
bool pirState = false;  
bool reedState = false;  

void setup() {
    Blynk.begin(auth, ssid, pass);
    pinMode(pirPin, INPUT);
    pinMode(reedPin, INPUT);
}

void loop() {
    pirState = digitalRead(pirPin);
    reedState = digitalRead(reedPin);
    if (pirState || reedState) {
        Blynk.notify("Alert! Motion detected or door/window opened");
    }
    Blynk.run();
    // Placeholder for future features
}
