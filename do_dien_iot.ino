/*
  SƠ ĐỒ CHÂN KẾT NỐI NHƯ SAU :
  LCD1602 SCL SDA nối với D1 D2
  D3 nối với chân IN1
  D4 nối với công tắc 
  PZEM-004T : chân D5-D6 ESP8266 nối với TX-RX PZEM-004T
  D7 nối với chân O cảm biến nhận diện người
  D8 nối với NO  relay2
  V0 Đèn kết nối
  V1 ĐIỆN ÁP
  V2 DÒNG ĐIỆN
  V3 CÔNG SUẤT
  V4 ĐIỆN NĂNG TIÊU THỤ HIỆN TẠI 
  V5 TẦN SỐ
  V6 HỆ SỐ CÔNG SUẤT
  V7 ĐIỆN NĂNG TIÊU THỤ HÔM QUA
  V8 TERMINAL
*/

#define BLYNK_TEMPLATE_ID "TMPL661pDlcJ6"
#define BLYNK_TEMPLATE_NAME "PZEM004T"
#define BLYNK_AUTH_TOKEN "0GJNZioSZW3n17B_MAUfcdJAuYVqlPW8"
#define BLYNK_WIFI_SSID "123123123"
#define BLYNK_WIFI_PASS "PZEM004T"

#define BLYNK_PRINT Serial

// bot telegram
#define BOT_TOKEN "7158993445:AAFSweNQ0FyM7XWPvX8JQUhFpu_pTmQrLJs"
#define CHAT_ID "-4153912012"

#define TERMINAL_VPIN V8


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiManager.h>

// Khởi tạo đối tượng LCD I2C với địa chỉ và số cột, số dòng
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Khởi tạo SoftwareSerial
SoftwareSerial pzemSWSerial(D5, D6); // D5 Rx, D6 Tx
PZEM004Tv30 pzem(pzemSWSerial);
WidgetTerminal terminal(TERMINAL_VPIN);
WiFiManager wifiManager;
// Khởi tạo UDP để giao tiếp với máy chủ NTP
WiFiUDP ntpUDP; 

// Khởi tạo client NTP
NTPClient timeClient(ntpUDP, "time.google.com", 25200, 60000); // GMT+7 offset in seconds and update interval
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Biến để theo dõi thời gian hiện tại
unsigned long previousMillis = 0;
const long interval = 10000; // Khoảng thời gian là 10 giây

//khai báo các chân led
const int ledPin = LED_BUILTIN; // GPIO pin cho đèn LED tích hợp
// Khai báo chân D7 làm chân INPUT nhận diện người
const int inputPin7 = D7;
// Khai báo chân D8 làm chân INPUT kiểm tra báo cháy
const int inputPin8 = D8;
// Khai báo chân D4 làm chân tắt còi
const int inputPin4 = D4;

// Địa chỉ EEPROM cho các biến
const int ADDR_ENEGRY_LAST_DAY = 0;
const int ADDR_ENEGRY_MONTH = 4;
const int ADDR_ENEGRY_NOW = 8;
const int ADDR_COST_MONTH = 12;
const int ADDR_ENEGRY_UPTO_YESTERDAY = 16;
const int ADDR_COST_LAST_MONTH = 20; 
const int ADDR_KWH_LAST_MONTH = 24;

// Biến để lưu trạng thái nhận diện người
bool nhandiennguoi = false;
// Khai báo biến toàn cục baochay
bool baochay = false;

// Giá tiền điện theo bậc
const float bac1 = 1806;
const float bac2 = 1866;
const float bac3 = 2167;
const float bac4 = 2729;
const float bac5 = 3050;
const float bac6 = 3100;

float enegry; // Biến này sẽ được cập nhật liên tục từ PZEM-004T
float enegrylastday; // Số kWh tiêu thụ của ngày hôm qua
float enegrymonth;   // Số kWh tiêu thụ trong tháng này
float enegrynow;     // Số kWh tiêu thụ hôm nay
float costMonth;     // Chi phí tiêu thụ điện trong tháng
float enegryuptoyesterday; // Số kWh tiêu thụ từ đầu tháng đến hôm qua
float costLastMonth; // Chi phí tiêu thụ điện tháng trước
float kwhLastMonth;  // Số kWh tiêu thụ tháng trước

const char* primarySSID = "primaryWiFi";   // Thay thế bằng SSID WiFi chính
const char* primaryPassword = "primaryPass"; // Thay thế bằng mật khẩu WiFi chính
const char* secondarySSID = "secondaryWiFi"; // Thay thế bằng SSID WiFi phụ
const char* secondaryPassword = "secondaryPass"; // Thay thế bằng mật khẩu WiFi phụ

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  // Khởi tạo PZEM
  pzemSWSerial.begin(9600);
  pzem.setAddress(0xF8);
  lcd.begin();
  lcd.backlight(); //bật đèn lcd
  pinMode(inputPin7, INPUT);
  pinMode(inputPin8, INPUT);
  pinMode(inputPin4, INPUT);
  pinMode(ledPin, OUTPUT);
  // Cài đặt các chân GPIO
  pinMode(D3, OUTPUT);
  digitalWrite(D3, LOW);
  nhandiennguoi = digitalRead(inputPin7  );
  baochay = digitalRead(inputPin8);
  connectwifi();
  secured_client.setInsecure(); // Thêm chứng chỉ gốc cho api.telegram.org
  if (secured_client.connect("api.telegram.org", 443)) {
    Serial.println("Connected to Telegram server");
    // Gửi thông báo khi kết nối thành công
    bot.sendMessage(CHAT_ID, "Thiết bị đã kết nối thành công với WiFi và Telegram!", "");
  } else {
    Serial.println("Connection to Telegram server failed");
  }
  Serial.println("Bot đã khởi động");
  ketnoithanhcongsetup();
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str());
  timeClient.begin();
  EEPROM.get(ADDR_ENEGRY_LAST_DAY, enegrylastday);
  EEPROM.get(ADDR_ENEGRY_MONTH, enegrymonth);
  EEPROM.get(ADDR_ENEGRY_NOW, enegrynow);
  EEPROM.get(ADDR_COST_MONTH, costMonth);
  EEPROM.get(ADDR_KWH_LAST_MONTH, kwhLastMonth);
  EEPROM.get(ADDR_COST_LAST_MONTH, costLastMonth);
}

void loop() 
{
  Blynk.run();
  ketnoithanhcongloop();
  pzem004t();
  kiemtrabaochay();
  inramanhinhlcd();
  timeClient.update();
  diennangtieuthu(); // Gọi hàm xử lý điện năng tiêu thụ
  guilenblynk();
   // Kiểm tra xem đã đủ 10 giây chưa
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Đã đủ 10 giây, thực hiện hành động
    previousMillis = currentMillis;
    // Xóa terminal và hiển thị thông tin tháng hiện tại
    terminal.clear();
    terminal.println("Điện năng tiêu thụ hôm qua: " + String(enegrylastday) + " kWh");
    terminal.println("Điện năng tiêu thụ trong tháng: " + String(enegrymonth) + " kWh");
    terminal.println("Chi phí tiêu thụ điện trong tháng: " + String(costMonth) + " VND");
    terminal.flush();
    }

  // In giá trị ra Serial Monitor
  Serial.print("Điện năng tiêu thụ hôm qua: ");
  Serial.print(enegrylastday);
  Serial.println(" kWh");

    Serial.println("Điện năng tiêu thụ trong tháng: " + String(enegrymonth) + " kWh");
    Serial.println("Chi phí tiêu thụ điện trong tháng: " + String(costMonth) + " VND");
  delay (2000);

}

void connectwifi()
{
  // Kết nối tới WiFi chính
  Serial.println("Connecting to primary WiFi...");
  WiFi.begin(primarySSID, primaryPassword);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Primary WiFi failed, trying secondary WiFi...");

    // Kết nối tới WiFi phụ nếu WiFi chính không khả dụng
    WiFi.begin(secondarySSID, secondaryPassword);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Secondary WiFi failed, starting WiFi Manager...");

      // Khởi động WiFiManager nếu cả hai mạng WiFi đều không khả dụng
      wifiManager.autoConnect("connect-wifi", "11111111");
    }
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void ketnoithanhcongsetup()
{
  Serial.print("\nWiFi đã kết nối. Địa chỉ IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Đang lấy thời gian: ");
  configTime(0, 0, "pool.ntp.org"); // lấy thời gian UTC qua NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  bot.sendMessage(CHAT_ID, "Bot đã khởi động", "");
  
}
void ketnoithanhcongloop()
{
  Serial.print("\nWiFi đang kết nối. Địa chỉ IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Đang lấy thời gian: ");
  configTime(0, 0, "pool.ntp.org"); // lấy thời gian UTC qua NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void pzem004t()
{
  // Đọc giá trị PZEM-004T
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  // Làm tròn các giá trị
  int roundedVoltage = round(voltage);
  float roundedCurrent = round(current * 100) / 100.0;
  int roundedPower = round(power);
  float roundedEnergy = round(energy * 100) / 100.0;
  // Kiểm tra giá trị của PZEM-004T
  if (isnan(voltage)) {
    Serial.println("LỖI ĐỌC ĐIỆN ÁP");
  } else if (isnan(current)) {
    Serial.println("LỖI ĐỌC DÒNG ĐIỆN");
  } else if (isnan(power)) {
    Serial.println("LỖI ĐỌC CÔNG SUẤT");
  } else if (isnan(energy)) {
    Serial.println("LỖI ĐỌC MỨC TIÊU THỤ");
  } else if (isnan(frequency)) {
    Serial.println("LỖI ĐỌC TẦN SỐ");
  } else if (isnan(pf)) {
    Serial.println("LỖI ĐỌC HỆ SỐ CÔNG SUẤT");
  } else {
    // Nếu không có lỗi, in giá trị ra Serial
    Serial.print("Điện áp: ");        Serial.print(roundedVoltage);               Serial.println("V");
    Serial.print("Dòng điện: ");      Serial.print(String(roundedCurrent, 2));    Serial.println("A");
    Serial.print("Công suất: ");      Serial.print(roundedPower);                 Serial.println("W");
    Serial.print("Tiêu thụ: ");       Serial.print(String(roundedEnergy, 2));     Serial.println("kWh");
  }
  if ((current >= 0.5) && (current <= 0.8))
  {
    bot.sendMessage(CHAT_ID, "HỆ THỐNG ĐIỆN ĐANG CÓ DẤU HIỆU QUÁ TẢI", "");////tin nhắn mức thấp bật
    digitalWrite (D3, LOW);
  }
  else if (current >= 0.8 )
  {
    digitalWrite (D3, HIGH); ////còi báo cháy
    Blynk.logEvent("canh_bao_chay", String("Hiện đang phát hiện một số bất thường có nguy cơ cháy nổ, đề nghị kiểm tra lại."));
    bot.sendMessage(CHAT_ID, "ĐANG CÓ NGUY CƠ CHÁY DO QUÁ TẢI ĐIỆN ĐÃ,NGẮT HỆ THỐNG ĐIỆN,BẬT CÒI CẢNH BÁO", "");////tin nhắn mức cao bật
    if (nhandiennguoi == 1) {
      bot.sendMessage(CHAT_ID, "Hiện đang có người trong nhà", "");
    } else {
      bot.sendMessage(CHAT_ID, "Hiện không có người trong nhà", "");
    }
  }
  else ///hệ thống = 0A
  {
    digitalWrite (D3, LOW);
  }
  if (digitalRead(D4)  == HIGH) 
  {
    digitalWrite(D3, LOW); // Kích chân số D3 ở mức thấp
    Serial.println("Chân số D4: HIGH -> Kích chân số D3 ở mức thấp");
  }
}

void inramanhinhlcd()
{
  // Đọc giá trị PZEM-004T
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  // Làm tròn các giá trị
  int roundedVoltage = round(voltage);
  float roundedCurrent = round(current * 100) / 100.0;
  int roundedPower = round(power);
  float roundedEnergy = round(energy * 100) / 100.0;
  // in giá trị ra lcd
  lcd.clear(); // làm mới lcd
  // sử dụng millis() để chuyển đổi hiển thị cứ sau 5 giây
  if ((millis() / 5000) % 2 == 0) {
    lcd.setCursor(0, 0); // Đặt con trỏ ở góc trên cùng bên trái
    lcd.print("V: "); lcd.print(roundedVoltage); lcd.print(" V");
    lcd.setCursor(0, 1); // di chuyển tới hàm tiếp theo
    lcd.print("I: "); lcd.print(String(roundedCurrent, 2)); lcd.print(" A");
  } else {
    lcd.setCursor(0, 0); // Đặt con trỏ ở góc trên cùng bên trái
    lcd.print("P: "); lcd.print(roundedPower); lcd.print(" W");
    lcd.setCursor(0, 1); // di chuyển tới hàm tiếp theo
    lcd.print("E: "); lcd.print(String(roundedEnergy, 2)); lcd.print(" kWh");
  }
  Serial.print("ĐÃ IN RA LCD ");
}

void kiemtrabaochay()
{
  if (baochay == 1)
  {
    bot.sendMessage(CHAT_ID, "Hiện đang có cháy trong nhà, đã ngắt điện hệ thống, bật còi cảnh báo", "");
    Blynk.logEvent("canh_bao_chay", String("Hiện đang phát hiện một số bất thường có nguy cơ cháy nổ,đề nghị kiểm tra lại. "));
  }
}

void guilenblynk()
{
  // Đọc các giá trị từ PZEM-004T
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  // Gửi các giá trị lên Blynk IoT
  Blynk.virtualWrite(V1, voltage);
  Blynk.virtualWrite(V2, current);
  Blynk.virtualWrite(V3, power);
  Blynk.virtualWrite(V5, frequency);
  Blynk.virtualWrite(V6, pf);
  // Thêm một đèn LED trên Blynk để hiển thị trạng thái kết nối
  bool isConnected = Blynk.connected(); // Trả về true nếu kết nối thành công
  Blynk.virtualWrite(V0, isConnected); // Gửi trạng thái kết nối đến Blynk
  // Hàm xử lý điện năng tiêu thụ và cập nhật các biến liên quan
}

void diennangtieuthu()
{
  enegry = pzem.energy();
  // Cập nhật enegrynow cho ngày hiện tại
  enegrynow = enegry - enegryuptoyesterday;
  Serial.print("Năng lượng hiện tại: ");
  Serial.println(enegrynow);
  Serial.print("Tiền điện tháng này: ");
  Serial.println(costMonth);
  if (timeClient.getHours() == 0 && timeClient.getMinutes() == 0) {
    // Cập nhật enegrylastday và enegryuptoyesterday vào đầu ngày
    enegrylastday = enegrynow;
    enegryuptoyesterday += enegrylastday;
    EEPROM.put(ADDR_ENEGRY_LAST_DAY, enegrylastday);
    EEPROM.put(ADDR_ENEGRY_UPTO_YESTERDAY, enegryuptoyesterday);
    enegrynow = 0;
  }
  if (timeClient.getDay() == 1 && timeClient.getHours() == 0 && timeClient.getMinutes() == 0)
  {
    // Đầu tháng, cộng dồn enegrylastday vào enegrymonth và reset
    enegrymonth += enegrylastday;
    EEPROM.put(ADDR_ENEGRY_MONTH, enegrymonth);
    costMonth = tinhtiendien(enegrymonth);
    EEPROM.put(ADDR_COST_MONTH, costMonth);
    // Đầu tháng, gán enegrymonth bằng kwhLastMonth và costMonth bằng costLastMonth
    kwhLastMonth = enegrymonth;
    costLastMonth = tinhtiendien(enegrymonth);
    // Lưu giá trị kWh và chi phí của tháng trước vào EEPROM
    EEPROM.put(ADDR_KWH_LAST_MONTH, kwhLastMonth);
    EEPROM.put(ADDR_COST_LAST_MONTH, costLastMonth);
    enegrylastday = 0;
    enegryuptoyesterday = 0; // Reset tổng kWh tiêu thụ từ đầu tháng
    enegrymonth = 0; // Reset số kWh tiêu thụ trong tháng
    // Reset giá trị năng lượng trên PZEM-004T
    pzem.resetEnergy();
  }
  // Gửi giá trị enegrynow lên Virtual Pin V4
  Blynk.virtualWrite(V4, enegrynow);
  // Gửi giá trị enegrylastday lên Virtual Pin V7
  Blynk.virtualWrite(V7, enegrylastday);
}
// Hàm tính tiền điện dựa trên điện năng tiêu thụ
float tinhtiendien(float enegry){
  float cost = 0;
  if (enegry <= 50) {
    cost = enegry * bac1;
  } else if (enegry <= 100) {
    cost = 50 * bac1 + (enegry - 50) * bac2;
  } else if (enegry <= 200) {
    cost = 50 * bac1 + 50 * bac2 + (enegry - 100) * bac3;
  } else if (enegry <= 300) {
    cost = 50 * bac1 + 50 * bac2 + 100 * bac3 + (enegry - 200) * bac4;
  } else if (enegry <= 400) {
    cost = 50 * bac1 + 50 * bac2 + 100 * bac3 + 100 * bac4 + (enegry - 300) * bac5;
  } else {
    cost = 50 * bac1 + 50 * bac2 + 100 * bac3 + 100 * bac4 + 100 * bac5 + (enegry - 400) * bac6;
  }
  return cost;
}

BLYNK_WRITE(TERMINAL_VPIN) {
  String command = param.asStr();
  // Kiểm tra nếu lệnh là "dulieuthangtruoc"
  if (command == "dulieuthangtruoc") {
    // Lấy dữ liệu từ EEPROM
    float kwhLastMonth;
    float costLastMonth;
    EEPROM.get(ADDR_KWH_LAST_MONTH, kwhLastMonth);
    EEPROM.get(ADDR_COST_LAST_MONTH, costLastMonth);
    // Gửi thông tin lên terminal của Blynk
    terminal.clear(); // Xóa thông tin cũ trên terminal
    terminal.println("Số kWh tháng trước: " + String(kwhLastMonth) + " kWh");
    terminal.println("Tiền điện tháng trước: " + String(costLastMonth) + " VND");
    terminal.flush(); // Đảm bảo thông tin được gửi ngay lập tức
    // Cập nhật thời gian để bắt đầu đếm 10 giây
    previousMillis = millis();
  }
}
