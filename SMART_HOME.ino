#define BLYNK_TEMPLATE_ID "TMPL2Ld6i0zTz"
#define BLYNK_TEMPLATE_NAME "smart home"
#define BLYNK_AUTH_TOKEN "NBPQ3Th9t1JWm3-2vWqdH0QUCQnx-ae8"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D7     // Digital pin connected to the DHT sensor (temperature and humidity)
#define DHTTYPE DHT11 // Type of DHT sensor

#define RAIN_PIN D3    // Digital pin connected to the rain sensor
#define light_PIN D5
#define relay_PIN D6
#define MOISTURE_PIN A0
#define LCD_ADDRESS 0x27


char auth[] = BLYNK_AUTH_TOKEN;
// Thông tin mạng Wi-Fi
char ssid[] = "DUY QUANG";
char password[] = "123456789";
DHTesp dht;
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

void setup() 
{
  Serial.begin(115200);
  // Kết nối Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Blynk.begin(auth, ssid, password);
  pinMode(light_PIN, OUTPUT);
  dht.setup(DHTPIN, DHTesp::DHT11);

  pinMode(RAIN_PIN, INPUT);
  pinMode(light_PIN, INPUT);
  pinMode(relay_PIN,OUTPUT);
  digitalWrite(relay_PIN,LOW);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Humi:");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.setCursor(11, 0);
  lcd.print("Rain: Yes");
}

void loop() 
{
  Blynk.run();
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  int rainStatus = digitalRead(RAIN_PIN);
  int lightStatus = digitalRead(light_PIN);
  int MOISTUREStatus = analogRead(MOISTURE_PIN);


  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    lcd.setCursor(5, 0);
    lcd.print(humidity);
    lcd.setCursor(5, 1);
    lcd.print(temperature);
  }

  if (rainStatus == LOW) {
    Serial.println("It's raining!");
    String data = "It's raining!";
    Blynk.virtualWrite(V3, data);
    lcd.setCursor(11, 1);
    lcd.print("Yes");
  } else {
    Serial.println("It's not raining.");
    String data = "It's not raining";
    Blynk.virtualWrite(V3, data);
    lcd.setCursor(11, 1);
    lcd.print("Noo");
  }
  if(lightStatus == LOW)
  {
    Serial.println("DayTIME");
    String data = "Day TIME";
    Blynk.virtualWrite(V4, data);
    digitalWrite(relay_PIN,HIGH);
  }
  else
  {
    Serial.println("NIGHT");
    String data = "NIGHT";
    Blynk.virtualWrite(V4, data);
    digitalWrite(relay_PIN,LOW);
  }

  Serial.println(MOISTUREStatus);

  delay(2000);
  // Gửi nhiệt độ và độ ẩm lên ứng dụng Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, MOISTUREStatus);
  
  
}
