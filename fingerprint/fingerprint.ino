#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Set up the LCD display
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7, v0=13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// buzzer pin
int buzzer = 6;

// lEDs
const int redLED = 5, greenLED = 4;

void setup() {
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  analogWrite(v0,5);

  lcd.begin(16, 2);  // Initialize a 16x2 LCD display

  // setup for LED
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // set the data rate for the sensor serial port
  finger.begin(57600);

  // Check if the fingerprint sensor is responding
  if (finger.getTemplateCount() != FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  // Verify the password for the fingerprint sensor
  if (finger.verifyPassword()) {
    Serial.println("Verified password for sensor");
  } else {
    Serial.println("Error verifying password for sensor");
    while (1) {
      delay(1);
    }
  }
}

void loop() {
  // Enroll a new fingerprint

  delay(600);  // Add a delay of 600 milliseconds
  lcd.clear();
  lcd.print("Place your thumb.");
  

  int p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER) {
    return;
  }
  else if (p != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error taking fingerprint");
    delay(500);
  } else {
    Serial.println("Fingerprint taken");
    delay(300);  // Add a delay of 800 milliseconds

    // Convert the image to a template
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
      Serial.println("Error converting fingerprint image to template");
      delay(500);  // Add a delay of 500 milliseconds
      return;
    }

    //image successfully converted, showing checking message
    lcd.clear();
    lcd.print("Checking...");
    delay(500);  // Add a delay of 500 milliseconds

    // Check if the fingerprint already exists in the database
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
      tone(buzzer, 450);
      Serial.println("Already voted.");
      digitalWrite(redLED, HIGH);
      lcd.clear();
      lcd.print("Already voted.");
      delay(500);
      noTone(buzzer);
      digitalWrite(redLED, LOW);
      delay(200);
      tone(buzzer, 450);
      digitalWrite(redLED, HIGH);
      delay(500);
      noTone(buzzer);
      digitalWrite(redLED, LOW);
      delay(200);
      tone(buzzer, 450);
      digitalWrite(redLED, HIGH);
      delay(1000);
      noTone(buzzer);
      delay(500);
      digitalWrite(redLED, LOW);
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      lcd.clear();
      lcd.print("Communication error.");
      Serial.println("Communication error");
      return;
    } else if (p == FINGERPRINT_NOTFOUND) {
      Serial.println("New voter.");
      lcd.clear();
      lcd.print("You can vote.");
      digitalWrite(greenLED, HIGH);
      tone(buzzer, 450);
      delay(1000);  // Add a delay of 1000 milliseconds
      noTone(buzzer);
      
      // Store the fingerprint in the database
      p = finger.storeModel(finger.fingerID);
      if (p != FINGERPRINT_OK) {
        Serial.println("Error storing fingerprint in the database");
        delay(500);  // Add a delay of 500 milliseconds
        return;
      }

      
      Serial.println("Fingerprint stored.");
      delay(1000);
      digitalWrite(greenLED, LOW);
    }
  }
}
