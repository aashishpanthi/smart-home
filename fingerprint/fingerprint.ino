#include <Adafruit_Fingerprint.h>

// Replace these with the correct serial port and baud rate for your hardware:
SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("Adafruit Fingerprint sensor enrollment");

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
    Serial.println("Verified password for fingerprint sensor");
  } else {
    Serial.println("Error verifying password for fingerprint sensor");
    while (1) { delay(1); }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // Enroll a new fingerprint
  Serial.println("Place a finger on the sensor");
  delay(500);  // Add a delay of 500 milliseconds
  int p = finger.getImage();
  if(p == FINGERPRINT_NOFINGER){
    return;
  }
  else if (p != FINGERPRINT_OK) {
    Serial.println("Error taking fingerprint image");
    delay(500);
    } else {
    Serial.println("Image taken");
    delay(500);  // Add a delay of 500 milliseconds

    // Convert the image to a template
    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
      Serial.println("Error converting fingerprint image to template");
      delay(500);  // Add a delay of 500 milliseconds
      return;
    }

    Serial.println("Image converted");
    delay(500);  // Add a delay of 500 milliseconds

    // Check if the fingerprint already exists in the database
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
      Serial.println("Fingerprint already exists in the database");
      delay(3000);
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return;
    } else if (p == FINGERPRINT_NOTFOUND) {
      Serial.println("Fingerprint not found in the database");
      delay(500);  // Add a delay of 500 milliseconds

      // Store the fingerprint in the database
      p = finger.storeModel(finger.fingerID);
      if (p != FINGERPRINT_OK) {
        Serial.println("Error storing fingerprint in the database");
        delay(500);  // Add a delay of 500 milliseconds
        return;
      }

      Serial.println("Fingerprint stored successfully");
      delay(2000);
    }
  }
}
