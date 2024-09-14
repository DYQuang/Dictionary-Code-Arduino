#include <Adafruit_TCS34725.h>
#include <Wire.h>

const int IR_1 = 5;
const int IR_2 = 6;

const int relay_BTAI = 2;
const int relay_XILANH1 = 3;
const int relay_XILANH2 = 4;

const int led_reset = 10;
const int button_reset = 7;
const int button_on = 8;
const int button_off = 9;
const int led_start = 11;
const int led_stop = 12;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);
int value = 0;

unsigned long previousMillis = 0;
unsigned long tg_bat_dau2 = 0;
unsigned long tg_bat_dau3 = 0;
unsigned long tg_bat_dau4 = 0;
const long interval = 500; // Interval for checking buttons in milliseconds
const long tg_ktr = 200;


bool isRunning = false;
int color = 0;

void setup() 
{
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(relay_BTAI, OUTPUT);
  pinMode(relay_XILANH1, OUTPUT);
  pinMode(relay_XILANH2, OUTPUT);
  pinMode(led_reset, OUTPUT);
  pinMode(button_reset, INPUT_PULLUP);
  pinMode(button_on, INPUT_PULLUP);
  pinMode(button_off, INPUT_PULLUP);
  pinMode(led_start, OUTPUT);
  pinMode(led_stop, OUTPUT);

  Serial.begin(9600);
  if (tcs.begin()) 
  {
    Serial.println("Cảm biến hoạt động");
  } 
  else 
  {
    Serial.println("Cảm biến không kết nối, hãy kiểm tra lại kết nối...");
    while (1);
  }
  // trạng thái ban đầu
  digitalWrite(led_stop, HIGH);
  digitalWrite(relay_BTAI, LOW);
  digitalWrite(relay_XILANH1, LOW);
  digitalWrite(relay_XILANH2, LOW);
  digitalWrite(led_reset, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    checkButtons();
  }

  if (isRunning) {
    check_color();

    // Bắt đầu kiểm tra các màu và thực hiện chức năng
    switch (value) {
      case 1:
        unsigned long tg_ketthuc2 = millis();
        if(tg_ketthuc2 - tg_bat_dau2 >= tg_ktr){
          tg_bat_dau2 = tg_ketthuc2;
          checkButtons();
        }
        break;
      case 2:
        unsigned long tg_ketthuc3 = millis();
        if(tg_ketthuc3 - tg_bat_dau3 >= tg_ktr){
          tg_bat_dau3 = tg_ketthuc3;
          checkButtons();
        }
        // chờ cho đến khi cảm biến IR_2 phát hiện vật cản

        break;
      case 3:
      unsigned long tg_ketthuc4 = millis();
        if(tg_ketthuc4 - tg_bat_dau4 >= tg_ktr){
          tg_bat_dau4 = tg_ketthuc4;
          checkButtons();
        }
        if(color == 3){
          // do vật màu xanh sẽ được bỏ qua để đi qua 2 cảm biến nên ta chỉ để delay ở đây, thời gian delay để vật chạy hết băng tải
          delay(1000);
        }
        break;
    }
  }
}

void reset_board() {
  digitalWrite(led_reset, HIGH);
  delay(2000);
  asm volatile ("jmp 0");
}

void checkButtons() {
  int btn_on = digitalRead(button_on);
  int btn_off = digitalRead(button_off);
  int btn_reset = digitalRead(button_reset);

  if (btn_reset == LOW) {
    digitalWrite(led_reset, HIGH);
    reset_board();
  }

  if (btn_off == LOW) {
    isRunning = false;
    digitalWrite(led_stop, HIGH);
    digitalWrite(relay_BTAI, LOW);
    digitalWrite(led_start, LOW);
  }

  if (btn_on == LOW) {
    isRunning = true;
    digitalWrite(led_start, HIGH);
    digitalWrite(led_stop, LOW);
    digitalWrite(relay_BTAI, HIGH);
  }
}

void check_color(){
  // Bắt đầu đọc cảm biến màu sắc
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b); // Nhiệt độ màu theo thang đo Kelvin
  lux = tcs.calculateLux(r, g, b); // Độ rọi soi, cường độ sáng
  if(r||b||g||c){
    value = 0;
    color = 0;
    Serial.println("no color");
  }
  if (r > g && r > b) { // Màu đỏ
    value = 1;
    color = 1;
    Serial.println("Màu sắc hiện tại là: MÀU ĐỎ");
    Serial.println(" ");
    Serial.println("chờ màu đỏ");
    // chờ cho đến khi cảm biến IR_1 phát hiện vật cản
    if(digitalRead(IR_1) == HIGH && color == 1) {
      // kiểm tra nút off và reset
      digitalWrite(relay_XILANH1, HIGH);
      delay(1000);
      digitalWrite(relay_XILANH1, LOW);
    }
  } else if (g > r && g > b && c > 350) { // Màu lục
    value = 2;
    color = 2;
    Serial.println("Màu sắc hiện tại là: MÀU LỤC");
    Serial.println(" ");
    Serial.println("đợi");
    delay(3500);
    if(digitalRead(IR_2==HIGH))
    {
      Serial.println(" phát hiện vật");
    digitalWrite(relay_XILANH2, HIGH);
    delay(1000);
    digitalWrite(relay_XILANH2, LOW);
    }
    else{
    Serial.println("chưa phát hiện vật");
    digitalWrite(relay_XILANH2, LOW);
    delay(1000);
    }
  } else if (b > r && b > g ) { // Màu xanh biển
    value = 3;
    color = 3;
    Serial.println("Màu sắc hiện tại là: MÀU XANH BIỂN");
    Serial.println(" ");
  }
}
