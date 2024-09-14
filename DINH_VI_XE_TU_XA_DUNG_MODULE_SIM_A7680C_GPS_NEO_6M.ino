#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial neo6M(13, 15); //D7->Tx, D8->Rx
static const uint32_t GPSBaud = 38400;

SoftwareSerial moduleSim(2,14); //D4->Tx, D5->Rx
String bufferModuleSim="";

String Latitude,Longitude;

const int relay=12; //D6

void setup() {
  Serial.begin(115200);//Giao tiếp uart với PC
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don't proceed, loop forever
  }
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.clearDisplay();
  display.setTextSize(2);  
  display.setCursor(0,0);                     // Start at top-left corner
  display.print("Welcome!");
  display.display();
  delay(2000); // Pause for 2 seconds

  neo6M.begin(GPSBaud);
  Serial.println(TinyGPSPlus::libraryVersion());

  moduleSim.begin(115200);
  setupModuleSim();
}

void loop() {
  if(moduleSim.available()){
    int indexData;
    bufferModuleSim="";
    while(moduleSim.available()){
      bufferModuleSim = moduleSim.readString();
    }
    bufferModuleSim.c_str();
    bufferModuleSim.toLowerCase();
    Serial.println("Data: "+bufferModuleSim);
    indexData=-1;
    indexData = bufferModuleSim.indexOf("chk");
    if(indexData>=0){
      Serial.println("Đã nhận từ khóa CHK!");

      int index_start = bufferModuleSim.indexOf('"');
      int index_end = bufferModuleSim.indexOf('"', index_start + 1);
      String so_dt="";
      if (index_start != -1 && index_end != -1) {
          // Tách số điện thoại từ chuỗi gốc
          so_dt = bufferModuleSim.substring(index_start + 1, index_end);
          Serial.print("Số điện thoại: ");
          Serial.println(so_dt);
      } else {
          Serial.println("Không tìm thấy ký tự \" trong chuỗi.");
      }

      display.clearDisplay();
      display.setTextSize(1);  
      display.setCursor(0,0);
      display.print(so_dt);
      display.setCursor(0,15);
      display.print("SMS:CHK");
      display.display();
      String data = Latitude +","+Longitude;
      sendSMS(so_dt,data);
      // callPhone(sdt);
      display.clearDisplay();
      display.setTextSize(2);  
      display.setCursor(0,0);
      display.print("SMS sent!");
      display.display();
      delay(2000);
      indexData=-1;
    }

    indexData=-1;
    indexData = bufferModuleSim.indexOf("sos1");
    if(indexData>=0){
      Serial.println("Đã nhận từ khóa SOS1!");
      digitalWrite(relay,HIGH);
      int index_start = bufferModuleSim.indexOf('"');
      int index_end = bufferModuleSim.indexOf('"', index_start + 1);
      String so_dt="";
      if (index_start != -1 && index_end != -1) {
          // Tách số điện thoại từ chuỗi gốc
          so_dt = bufferModuleSim.substring(index_start + 1, index_end);
          Serial.print("Số điện thoại: ");
          Serial.println(so_dt);
      } else {
          Serial.println("Không tìm thấy ký tự \" trong chuỗi.");
      }

      display.clearDisplay();
      display.setTextSize(1);  
      display.setCursor(0,0);
      display.print(so_dt);
      display.setCursor(0,15);
      display.print("SMS:SOS1");
      display.display();
      sendSMS(so_dt,"SOS_ON");
      // callPhone(sdt);
      display.clearDisplay();
      display.setTextSize(2);  
      display.setCursor(0,0);
      display.print("SMS sent!");
      display.display();
      delay(2000);
      indexData=-1;
    }

    indexData=-1;
    indexData = bufferModuleSim.indexOf("sos0");
    if(indexData>=0){
      Serial.println("Đã nhận từ khóa SOS0!");
      digitalWrite(relay,LOW);
      int index_start = bufferModuleSim.indexOf('"');
      int index_end = bufferModuleSim.indexOf('"', index_start + 1);
      String so_dt="";
      if (index_start != -1 && index_end != -1) {
          // Tách số điện thoại từ chuỗi gốc
          so_dt = bufferModuleSim.substring(index_start + 1, index_end);
          Serial.print("Số điện thoại: ");
          Serial.println(so_dt);
      } else {
          Serial.println("Không tìm thấy ký tự \" trong chuỗi.");
      }

      display.clearDisplay();
      display.setTextSize(1);  
      display.setCursor(0,0);
      display.print(so_dt);
      display.setCursor(0,15);
      display.print("SMS:SOS0");
      display.display();
      sendSMS(so_dt,"SOS_OFF");
      // callPhone(sdt);
      display.clearDisplay();
      display.setTextSize(2);  
      display.setCursor(0,0);
      display.print("SMS sent!");
      display.display();
      delay(2000);
      indexData=-1;
    }
  }
  while (Serial.available()){ 
      char temp = char(Serial.read());
      Serial.print(temp);
      moduleSim.write(temp);
  }

  smartDelay(1000);
  if (gps.location.isUpdated()) {
    Latitude = String(gps.location.lat(),6);
    Longitude = String(gps.location.lng(), 6);
    Serial.print("Latitude= ");
    Serial.print(Latitude);
    Serial.print(" Longitude= ");
    Serial.println(Longitude);

    display.clearDisplay();
    display.setTextSize(1);  
    display.setCursor(0,0);                     // Start at top-left corner
    display.print("LAT:");
    display.print(Latitude);
    display.setCursor(0,20);
    display.print("LNG:");
    display.print(Longitude);
    display.display();
  }else{
    printDateTime(gps.date);
  }
}
//-------------Chương trình con--------------------
void smartDelay(unsigned long ms){
  unsigned long start = millis();
  do 
  {
    while (neo6M.available())
      gps.encode(neo6M.read());
  } while (millis() - start < ms);
}
static void printDateTime(TinyGPSDate &d){
  if (!d.isValid()){
    Serial.print(F("********** "));
  }else{
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d", d.day(),  d.month(), d.year());
    Serial.print("\nTimeGPS:");
    Serial.println(sz);
    display.clearDisplay();
    display.setTextSize(1);  
    display.setCursor(0,0);                     // Start at top-left corner
    display.print("Date:"+String(sz));
    display.display();
  }
}
void setupModuleSim(){
  moduleSim.println("ATE0"); //Tắt chế độ echo khi gửi lệnh đi
  delay(1000);
  moduleSim.println("AT+IPR=115200"); //Cài tốc độ baud 115200
  delay(1000);
  moduleSim.println("AT+CMGF=1"); //Hiển thị tin nhắn ở chế độ txt
  delay(1000);
  moduleSim.println("AT+CLIP=1"); //Hiển thị số điện thoại gọi đến
  delay(1000);
  moduleSim.println("AT+CNMI=2,2"); //Hiển thị trực tiếp nội dung tin nhắn gửi đến
  delay(1000);
}
void sendSMS(String phone, String content){
  moduleSim.println("AT+CMGS=\"" + phone + "\"");     // Lenh gui tin nhan
  delay(3000);                                        // Cho ky tu '>' phan hoi ve 
  moduleSim.print(content);                           // Gui noi dung
  moduleSim.print((char)26);                          // Gui Ctrl+Z hay 26 de ket thuc noi dung tin nhan va gui tin di
  delay(1000);
  Serial.println("Đã gửi tọa độ vị trí! "+content);
}
void callPhone(String phone){
  moduleSim.println("ATD" + phone + ";");
  delay(1000);
  Serial.println();
  Serial.print("Đang thực hiện cuộc gọi đến số: ");
  Serial.println(phone);
  delay(1000);
}