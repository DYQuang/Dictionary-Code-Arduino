#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);  // Khởi tạo giao tiếp Serial với tốc độ 115200 bps

  // Khởi động quét mạng WiFi xung quanh
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Bắt đầu quét mạng WiFi xung quanh:");
}

void loop() {
  // Quét mạng WiFi xung quanh
  int networks = WiFi.scanNetworks();

  if (networks == 0) {
    Serial.println("Không tìm thấy mạng nào.");
  } else {
    Serial.print("Số lượng mạng WiFi xung quanh: ");
    Serial.println(networks);

    Serial.println("Danh sách mạng WiFi:");
    for (int i = 0; i < networks; i++) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));  // Tên mạng (SSID)
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));  // Cường độ tín hiệu
      Serial.print(" dBm)");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");  // Kiểu mã hóa (nếu có)
      delay(10);
    }
  }
  Serial.println();

  delay(5000);  // Đợi 5 giây trước khi quét lại
}
