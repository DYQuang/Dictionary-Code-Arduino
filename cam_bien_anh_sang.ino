const int pinled = 10;
const int light = 4;

void setup() 
{
  Serial.begin(9600);
  pinMode(pinled, OUTPUT),
  pinMode(light, INPUT);
  digitalWrite(pinled,LOW);

}

void loop() 
{
  int val=digitalRead(light);
  Serial.println(val);
  if(val==1)
  {
    digitalWrite(pinled,HIGH);
  }
  else 
  {
    digitalWrite(pinled,LOW);
  }
}
