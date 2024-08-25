#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define RST_PIN 9
#define SS_PIN 10
#define RED_LED_PIN 2
#define GREEN_LED_PIN 3
#define BUZZER_PIN 4
#define RELAY_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int rfidAttempts = 0;
int fingerprintAttempts = 0;
bool ownerNotified = false;
String otp = "";
bool accessGranted = false;

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  finger.begin(57600);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Security System");
  delay(2000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID/Biometric");

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
    String rfidTag = "";
    for (byte i = 0; i < mfrc522.uid.size; i++){
      rfidTag += String(mfrc522.uid.uidByte[i], HEX);
    }
    rfidTag.toUpperCase();
    mfrc522.PICC_HaltA();

    if (isValidRFID(rfidTag)){
      correctAccess();
    } else {
      wrongAccessAttempt();
      rfidAttempts++;
      if (rfidAttempts >= 3){
        notifyOwner();
      }
    }
  }

  int fingerprintID = getFingerprintID();
  if (fingerprintID != -1){
    if (isValidFingerprint(fingerprintID)){
      correctAccess();
    } else{
      wrongAccessAttempt();
      fingerprintAttempts++;
      if (fingerprintAttempts >= 3){
        notifyOwner();
      }
    }
  }

  if (ownerNotified && !accessGranted{
    if (checkOwnerApproval()){
      lcd.clear();
      lcd.print("Enter OTP:");
      String enteredOTP = getUserInput();
      if (enteredOTP == otp){
        correctAccess();
      } else{
        wrongAccessAttempt();
      }
    }
  }

  delay(500);
}

bool isValidRFID(String rfidTag){
  // Compare rfidTag with stored authorized tags
  // Return true if valid, false otherwise
  return rfidTag == "SOME_VALID_TAG";  // Example tag
}

bool isValidFingerprint(int id){
  // Compare id with stored authorized IDs
  // Return true if valid, false otherwise
  return id == 1; // Example ID
}

int getFingerprintID(){
  if (finger.verifyPassword()){
    int result = finger.getImage();
    if (result == FINGERPRINT_OK){
      result = finger.image2Tz();
      if (result == FINGERPRINT_OK){
        result = finger.fingerFastSearch();
        if (result == FINGERPRINT_OK){
          return finger.fingerID;
        }
      }
    }
  }
  return -1;
}

void correctAccess({
  digitalWrite(GREEN_LED_PIN, HIGH);
  lcd.clear();
  lcd.print("Access Granted");
  delay(3000);
  digitalWrite(GREEN_LED_PIN, LOW);
  accessGranted = true;
}

void wrongAccessAttempt(){
  digitalWrite(RED_LED_PIN, HIGH);
  lcd.clear();
  lcd.print("Access Denied");
  tone(BUZZER_PIN, 1000, 500);
  delay(2000);
  digitalWrite(RED_LED_PIN, LOW);
}

void notifyOwner() {
  if (!ownerNotified){
    ownerNotified = true;
    // Send notification to owner (e.g., via WiFi module)
    otp = generateOTP();
    Serial.print("OTP sent to owner: ");
    Serial.println(otp);
  }
}

String generateOTP(){
  String otp = "";
  for (int i = 0; i < 4; i++){
    otp += String(random(0, 10));
  }
  return otp;
}

String getUserInput(){
  String input = "";
  char key;
  while (input.length() < 4) {
    key = keypad.getKey();
    if (key){
      input += key;
      lcd.print(key);
    }
  }
  return input;
}

bool checkOwnerApproval(){
  // Check if owner has approved access (e.g., through a mobile app)
  // Return true if approved, false otherwise
  return true;  // Example: Assume owner has approved
}
