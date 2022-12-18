int trigpin = 8;
int echopin = 7;

long duration;
int distance;

void setup() {
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  Serial.begin(9600);
  delay(50);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(trigpin, LOW);
  delay(1);

  digitalWrite(trigpin, HIGH);
  delay(20);
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);
}
