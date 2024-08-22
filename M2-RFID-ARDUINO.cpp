#include <RFID.h>
#include <SPI.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 8
#define RED_LED 2
#define GREEN_LED 3
#define BUZZER 7
#define IR_PIN 5

Servo doorServo;
RFID rfid(SS_PIN, RST_PIN);

int validID[5] = {10, 28, 87, 46, 111};
bool cardValid = false;
bool doorLocked = false;

void closeDoor() {
  if (doorLocked) {
    doorServo.write(0);
    Serial.println("Door closed.");
    delay(100);
    doorServo.write(180);
  }
}

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(IR_PIN, OUTPUT);
  doorServo.attach(SERVO_PIN);
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}

void loop() {
  cardValid = true;
  doorServo.write(0);

  Serial.println("Hold the Card.");
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      Serial.print("ID: ");
      delay(100);
    }
    
    for (int i = 0; i < 5; i++) {
      Serial.print(rfid.serNum[i]);
      delay(100);
    }
    
    for (int i = 0; i < 5; i++) {
      if (validID[i] != rfid.serNum[i]) {
        cardValid = false;
        tone(BUZZER, 1000);
        delay(1000);
        noTone(BUZZER);
        Serial.println("Wrong ID.");
        delay(1000);
        return;
      }
    }

    Serial.println();
    delay(1000);
    Serial.println("Welcome!");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    tone(BUZZER, 1000);
    delay(1000);
    noTone(BUZZER);
    delay(100);
    doorServo.write(180);
    delay(2000);
    Serial.println("Door opened.");
    delay(10000);

    closeDoor();
    
    if (digitalRead(IR_PIN) <= 500) {
      doorServo.write(0);
      Serial.println("Door closed.");
    }
  }
}
