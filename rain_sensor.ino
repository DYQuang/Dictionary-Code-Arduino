// Pin connected to the digital output of the rain sensor
const int rainSensorPin = 4; // Use any digital pin you prefer

void setup() {
  Serial.begin(9600);
  pinMode(rainSensorPin, INPUT); // Set the pin as input
}

void loop() {
  // Read the digital value from the rain sensor
  int sensorValue = digitalRead(rainSensorPin);

  // Print the raw sensor value to the serial monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Interpret the sensor value
  if (sensorValue == HIGH) {
    Serial.println("Dry");
  } else {
    Serial.println("Wet");
  }

  delay(1000); // Delay between readings
}
