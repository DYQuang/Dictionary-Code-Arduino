

#define BLYNK_TEMPLATE_ID "TMPL6NNjLVg9D"
#define BLYNK_TEMPLATE_NAME "controll 8 led"

#include <WiFiManager.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
// thiết lập các chân nút nhấn
const int bt1 = 23;
const int bt2 = 22;
const int bt3 = 21;
const int bt4 = 19;
const int bt5 = 18;
const int bt6 = 5;
const int bt7 = 17;
const int bt8 = 16;
// thiết lập các chân led
const int led1 = 32;
const int led2 = 33;
const int led3 = 15;
const int led4 = 26;
const int led5 = 27;
const int led6 = 14;
const int led7 = 12;
const int led8 = 13;
char auth[] = "I0v-B_nXWrgc7UcXjqacYD9uKqk7yo3s";

bool bt1State = HIGH;
bool bt2State = HIGH;
bool bt3State = HIGH;
bool bt4State = HIGH;
bool bt5State = HIGH;
bool bt6State = HIGH;
bool bt7State = HIGH;
bool bt8State = HIGH;

void setup()
{
  Serial.begin(115200);
  pinMode(bt1, INPUT_PULLUP);
  pinMode(bt2, INPUT_PULLUP);
  pinMode(bt3, INPUT_PULLUP);
  pinMode(bt4, INPUT_PULLUP);
  pinMode(bt5, INPUT_PULLUP);
  pinMode(bt6, INPUT_PULLUP);
  pinMode(bt7, INPUT_PULLUP);
  pinMode(bt8, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);

  // Khởi tạo đối tượng WiFiManager
  WiFiManager wifiManager;

  // Reset cấu hình WiFi trước đó
  wifiManager.resetSettings();

  // Kết nối với WiFi
  if (!wifiManager.autoConnect("ESP32-AP")) {
    Serial.println("Lỗi kết nối và khởi động lại...");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  // Kết nối thành công
  Serial.println("Kết nối WiFi thành công!");
  // In địa chỉ IP đã cấu hình
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
}
void loop()
{
  Blynk.run();
  checkButton();
}
void checkButton(){
  if(digitalRead(bt1)==LOW){
    if(bt1State==HIGH){
      digitalWrite(led1,!digitalRead(led1));
      Blynk.virtualWrite(V1,digitalRead(led1));
      delay(200);
      bt1State=LOW;
    }
  }else{
    bt1State=HIGH;
  }
  if(digitalRead(bt2)==LOW){
    if(bt2State==HIGH){
      digitalWrite(led2,!digitalRead(led2));
      Blynk.virtualWrite(V2,digitalRead(led2));
      delay(200);
      bt2State=LOW;
    }
  }else{
    bt2State=HIGH;
  }
  if(digitalRead(bt3)==LOW){
    if(bt3State==HIGH){
      digitalWrite(led3,!digitalRead(led3));
      Blynk.virtualWrite(V3,digitalRead(led3));
      delay(200);
      bt3State=LOW;
    }
  }else{
    bt3State=HIGH;
  }
  if(digitalRead(bt4)==LOW){
    if(bt4State==HIGH){
      digitalWrite(led4,!digitalRead(led4));
      Blynk.virtualWrite(V4,digitalRead(led4));
      delay(200);
      bt4State=LOW;
    }
  }else{
    bt4State=HIGH;
  }
  if(digitalRead(bt5)==LOW){
    if(bt5State==HIGH){
      digitalWrite(led5,!digitalRead(led5));
      Blynk.virtualWrite(V5,digitalRead(led5));
      delay(200);
      bt5State=LOW;
    }
  }else{
    bt5State=HIGH;
  }
  if(digitalRead(bt6)==LOW){
    if(bt6State==HIGH){
      digitalWrite(led6,!digitalRead(led6));
      Blynk.virtualWrite(V6,digitalRead(led6));
      delay(200);
      bt6State=LOW;
    }
  }else{
    bt6State=HIGH;
  }
  if(digitalRead(bt7)==LOW){
    if(bt7State==HIGH){
      digitalWrite(led7,!digitalRead(led7));
      Blynk.virtualWrite(V7,digitalRead(led7));
      delay(200);
      bt7State=LOW;
    }
  }else{
    bt7State=HIGH;
  }
  if(digitalRead(bt8)==LOW){
    if(bt8State==HIGH){
      digitalWrite(led8,!digitalRead(led8));
      Blynk.virtualWrite(V8,digitalRead(led8));
      delay(200);
      bt8State=LOW;
    }
  }else{
    bt8State=HIGH;
  }
}
// các nút trên app
// nút 1
BLYNK_WRITE(V1)
{
  int button1 = param.asInt();

  if (button1 == 1)
  {
    digitalWrite(led1, HIGH);
  }
  else
  {
    digitalWrite(led1, LOW);
  }
}
// nút 2
BLYNK_WRITE(V2)
{
  int button2 = param.asInt();

  if (button2 == 1)
  {
    digitalWrite(led2, HIGH);
  }
  else
  {
    digitalWrite(led2, LOW);
  }
}
// nút 3
BLYNK_WRITE(V3)
{
  int button3 = param.asInt();

  if (button3 == 1)
  {
    digitalWrite(led3, HIGH);
  }
  else
  {
    digitalWrite(led3, LOW);
  }
}
// nút 4
BLYNK_WRITE(V4)
{
  int button4 = param.asInt();

  if (button4 == 1)
  {
    digitalWrite(led4, HIGH);
  }
  else
  {
    digitalWrite(led4, LOW);
  }
}
// nút 5
BLYNK_WRITE(V5)
{
  int button5 = param.asInt();

  if (button5 == 1)
  {
    digitalWrite(led5, HIGH);
  }
  else
  {
    digitalWrite(led5, LOW);
  }
}
// nút 6
BLYNK_WRITE(V6)
{
  int button6 = param.asInt();

  if (button6 == 1)
  {
    digitalWrite(led6, HIGH);
  }
  else
  {
    digitalWrite(led6, LOW);
  }
}
// nút 7
BLYNK_WRITE(V7)
{
  int button7 = param.asInt();

  if (button7 == 1)
  {
    digitalWrite(led7, HIGH);
  }
  else
  {
    digitalWrite(led7, LOW);
  }
}
// nút 8
BLYNK_WRITE(V8)
{
  int button8 = param.asInt();

  if (button8 == 1)
  {
    digitalWrite(led8, HIGH);
  }
  else
  {
    digitalWrite(led8, LOW);
  }
}