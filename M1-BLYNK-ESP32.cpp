#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "<id>"
#define BLYNK_TEMPLATE_NAME "<name>"
#define BLYNK_AUTH_TOKEN "<token>"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DFRobot_mmWave_Radar.h>


char ssid[] = "<ssid>";
char pass[] = "<pass>";

HardwareSerial mySerial(1);  
DFRobot_mmWave_Radar sensor(&mySerial);

#define alarmPin 7

int val = 0;

BLYNK_WRITE(V2) {
  if (param.asInt() == 0) { 
    digitalWrite(alarmPin, LOW); 
    Serial.println("Alarm OFF."); 
  } else {
    digitalWrite(alarmPin, HIGH);  
    Serial.println("Alarm ON.");  
  }
}

void setup() {
  Serial.begin(115200); 
  mySerial.begin(115200, SERIAL_8N1, 20, 21); 
  pinMode(alarmPin, OUTPUT);    
  digitalWrite(alarmPin, LOW);   

  sensor.factoryReset();   
  sensor.DetRangeCfg(0, 1); 

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  
}

void loop() {
  Blynk.run();  
  val = sensor.readPresenceDetection();  
  digitalWrite(alarmPin, val);  
  Serial.println(val); 


  // Placeholder for additional features and potential addons

  
                    
}
