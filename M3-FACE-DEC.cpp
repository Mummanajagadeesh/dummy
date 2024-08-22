#include <WiFi.h>
#include <Camera.h>
#include <OpenCV.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
bool faceDetected = false;

void setup() {
    WiFi.begin(ssid, password);
    Camera.begin();
}

void loop() {
    faceDetected = Camera.captureAndDetectFace();
    if (faceDetected) {
        Serial.println("Face detected!");
        // Placeholder for alert or action
    }
    delay(1000);
    // Placeholder for future features
}
