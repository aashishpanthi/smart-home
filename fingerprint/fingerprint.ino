#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Set up the LCD display
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// buzzer pin
int buzzer = 8;

void setup() {
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  lcd.begin(16, 2);  // Initialize a 16x2 LCD display

  // set the data rate for the sensor serial port
  finger.begin(57600);

  // Check if the fingerprint sensor is responding
  if (finger.getTemplateCount() != FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  // Verify the password for the fingerprint sensor
  if (finger.verifyPassword()) {
    Serial.println("Verified password for sensor");
  } else {
    Serial.println("Error verifying password for sensor");
    while (1) { delay(1); }
  }
}

void loop() {
  // Enroll a new fingerprint
  lcd.println("Place a finger on the sensor");
  delay(100);  // Add a delay of 500 milliseconds
  
  int p = finger.getImage();
  if(p == FINGERPRINT_NOFINGER){
    return;
  }
  else if (p != FINGERPRINT_OK) {
    Serial.println("Error taking fingerprint image");
    delay(500);
    } else {
    Serial.println("Image taken");
    delay(1000);  // Add a delay of 500 milliseconds

    // Convert the image to a template
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
      Serial.println("Error converting fingerprint image to template");
      delay(500);  // Add a delay of 500 milliseconds
      return;
    }

    //image successfully converted, showing checking message
    Serial.println("Checking...");
    delay(500);  // Add a delay of 500 milliseconds

    // Check if the fingerprint already exists in the database
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
      tone(buzzer, 450);
      Serial.println("You have already voted.");
      delay(1500);
      noTone(buzzer);
      delay(2500);
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return;
    } else if (p == FINGERPRINT_NOTFOUND) {
      Serial.println("You can vote.");
      delay(1000);  // Add a delay of 500 milliseconds

      // Store the fingerprint in the database
      p = finger.storeModel(finger.fingerID);
      if (p != FINGERPRINT_OK) {
        Serial.println("Error storing fingerprint in the database");
        delay(500);  // Add a delay of 500 milliseconds
        return;
      }

      Serial.println("Fingerprint stored successfully.");
      delay(5000);
    }
  }
}
