#include <WiFi.h>
#include "AdafruitIO_WiFi.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Khai báo thông tin WiFi và Adafruit IO
#define IO_USERNAME  "quang262k7"
#define IO_KEY       "aio_Feku43uWOxXT3FEFAqDIwwp304R4"
#define WIFI_SSID "DUY QUANG"  // Thay thế với tên WiFi của bạn
#define WIFI_PASS "123456789"  // Thay thế với mật khẩu WiFi của bạn

// Khai báo chân DHT và kiểu cảm biến
#define DHTPIN 4       // Chân kết nối với DHT11
#define DHTTYPE DHT11  // Kiểu cảm biến là DHT11

DHT dht(DHTPIN, DHTTYPE);

// Khai báo kết nối Adafruit IO
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Khai báo feed của Adafruit IO để lưu trữ dữ liệu nhiệt độ
AdafruitIO_Feed *temperatureFeed = io.feed("nhietdo");

void setup() {
  // Khởi tạo cảm biến DHT
  dht.begin();

  // Khởi động kết nối nối tiếp
  Serial.begin(115200);

  // Khởi động kết nối Adafruit IO
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // Chờ kết nối thành công
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Adafruit IO Connected!");
}

void loop() {
  io.run();
  // Lấy nhiệt độ hiện tại từ cảm biến DHT11
  float currentTemperature = dht.readTemperature();

  // Kiểm tra xem cảm biến có đọc được dữ liệu không
  if (isnan(currentTemperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Hiển thị nhiệt độ hiện tại
  Serial.print("Current Temperature: ");
  Serial.println(currentTemperature);

  // Gửi nhiệt độ hiện tại lên Adafruit IO
  temperatureFeed->save(currentTemperature);

  // Chờ 10 giây trước khi gửi dữ liệu tiếp theo
  delay(10000);
}
