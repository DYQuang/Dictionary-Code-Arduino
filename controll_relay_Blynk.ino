#define BLYNK_TEMPLATE_ID "TMPL2WtmUDzvs"
#define BLYNK_TEMPLATE_NAME "may rot nuoc"
#define BLYNK_AUTH_TOKEN "0c30yV_s4GlF9vs0a-n_iuiIYlIZrL51"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define RELAYPIN D1

char auth[] = BLYNK_AUTH_TOKEN;
// Thông tin mạng Wi-Fi
char ssid[] = "Thao My";
char password[] = "24122007";
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

  pinMode(RELAYPIN,OUTPUT);
  digitalWrite(RELAYPIN,LOW);
}

void loop() 
{
  Blynk.run();
}
BLYNK_WRITE(V1)  // Chỉ số V1 tương ứng với widget Button trong ứng dụng Blynk
{
  int relayState = param.asInt();

  if (relayState == 1)
  {
    digitalWrite(RELAYPIN, HIGH);  // Bật relay
  }
  else
  {
    digitalWrite(RELAYPIN, LOW);  // Tắt relay
  }
}
