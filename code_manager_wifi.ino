

#include <ESP8266WiFi.h>
#include <WiFiManager.h>

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  // Reset cấu hình WiFi trước đó
  wifiManager.resetSettings();

  // Tạo AP cấu hình WiFi nếu không tìm thấy thông tin đăng nhập trước đó
  if (!wifiManager.autoConnect("AP-Name", "11111111")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Thực hiện các hoạt động khác trong vòng lặp chính
}