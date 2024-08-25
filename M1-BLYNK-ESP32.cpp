#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "<SSID>"; 
const char* password = "<PASSWORD>";
const char* blynkAuth = "<BLYNK_AUTH_TOKEN>";

const int pirPins[] = { 23, 22, 21 }; // PIR pins
const int reedPins[] = { 19, 18 }; // reed switch pins

const int numPIRs = sizeof(pirPins)/sizeof(pirPins[0]); // no of PIRs
const int numReeds = sizeof(reedPins)/sizeof(reedPins[0]); // no of reed switches

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Blynk.begin(blynkAuth, ssid, password);
    
  for (int i = 0; i < numPIRs; i++){
    pinMode(pirPins[i], INPUT);
  }
  for (int i = 0; i < numReeds; i++){
    pinMode(reedPins[i], INPUT);
  }
}

void loop(){

  Blynk.run();
    
  for (int i= 0; i< numPIRs; i++){
    if (digitalRead(pirPins[i]) == HIGH){
      String message = "Motion detected at PIR sensor " + String(i);
      Blynk.notify(message);
      delay(5000);
    }
  }

  for (int i = 0; i <numReeds;i++){
    if (digitalRead(reedPins[i]) == HIGH){
      String message = "Door/Window " + String(i) + " opened";
      Blynk.notify(message);
      delay(5000);
    }
  }

  delay(100);
}
