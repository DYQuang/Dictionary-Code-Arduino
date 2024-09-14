
#define BLYNK_TEMPLATE_ID "TMPL2VmH7UbRz"
#define BLYNK_TEMPLATE_NAME "DHT"
#define BLYNK_AUTH_TOKEN "Zv4x7uVQ0G5kVPibA95ZgdqlPNJwJBwa"
#define BLYNK_WIFI_SSID "DuyQuang"
#define BLYNK_WIFI_PASS "DuyQuang"

#define BLYNK_PRINT Serial
#define DHTPIN D1
#define DHTTYPE DHT11 

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <DHT.h>

WiFiManager wifimanager;
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  wifimanager.autoConnect("config wifi");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str());
}

void loop() {
  Blynk.run();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  updata_sensor_data(humidity,temperature);
}

void updata_sensor_data(float a, float b){
  Blynk.virtualWrite(V0,a);
  Blynk.virtualWrite(V1,b);
}
