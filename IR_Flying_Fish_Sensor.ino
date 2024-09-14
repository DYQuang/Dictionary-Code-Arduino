int IR_SENSOR = 4;   // Setze den Pin 4 als Anschluss für den Sensor
int LED       = 10;  // Setze den Pin 10 als Anschluss für die LED

void setup() {
  Serial.begin(9600);
  pinMode(IR_SENSOR, INPUT);  // Setze den Sensor-Pin als INPUT, da dieser ein
                              // Signal an den Arduino senden soll
  pinMode(LED, OUTPUT);       // Setze die LED als OUTPUT
}
  
void loop() 
{
  int val = digitalRead(IR_SENSOR);
  Serial.println(val);
  if ( val== 0) {  // Auslesen, ob IR-Sensor das Signal gibt,
                                      // dass ein Hindernis vor ihm ist
    digitalWrite(LED, HIGH);  // Wenn ja, soll die LED leuchten
  } else {
    digitalWrite(LED, LOW);  // Wenn nicht, soll die LED nicht leuchten
  }
}
