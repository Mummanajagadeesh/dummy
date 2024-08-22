#include <RFID.h>
#include <Biometric.h>

RFID rfid = RFID();
Biometric bio = Biometric();

void setup() {
    Serial.begin(9600);
    rfid.init();
    bio.init();
}

void loop() {
    if (rfid.isCardPresent()) {
        if (bio.verifyFinger()) {
            Serial.println("Access granted");
        } else {
            Serial.println("Access denied");
        }
    }
    // Placeholder for future features
}
