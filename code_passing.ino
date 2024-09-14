const int buttonPin = 3;   // Chân kết nối nút nhấn
const int relayPin = 5;    // Chân kết nối relay

bool relayState = false;   // Trạng thái hiện tại của relay (bật/tắt)
bool buttonState;          // Trạng thái nút nhấn hiện tại
bool lastButtonState = LOW; // Trạng thái nút nhấn trước đó

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Thiết lập chân nút nhấn là INPUT_PULLUP
  pinMode(relayPin, OUTPUT);         // Thiết lập chân relay là OUTPUT
  digitalWrite(relayPin, relayState); // Đặt relay ở trạng thái hiện tại (bật hoặc tắt)
}

void loop() {
  unsigned long buttonPressTime = 0; // Biến để lưu thời gian nhấn nút
  buttonState = digitalRead(buttonPin); // Đọc trạng thái nút nhấn

  // Xác định khi nào nút được nhấn hoặc thả ra
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      // Khi nút được nhấn, kiểm tra thời gian đã nhấn
      if (millis() - buttonPressTime >= 2000) {
        // Nếu đã nhấn hơn 2 giây, thay đổi trạng thái relay và cập nhật thời gian
        relayState = !relayState;
        digitalWrite(relayPin, relayState);
        buttonPressTime = millis();
      }
    } else {
      // Khi nút được thả ra, cập nhật thời gian
      buttonPressTime = millis();
    }
    delay(50); // Chờ để tránh đọc nhiễu do sự chuyển đổi nhanh của nút nhấn
  }

  lastButtonState = buttonState; // Lưu trạng thái nút cho lần lặp tiếp theo

  // Nếu relay đang bật, thực hiện nhấp nháy
  if (relayState) {
    digitalWrite(relayPin, HIGH); // Bật relay
    delay(100);                    // Đợi 100ms
    digitalWrite(relayPin, LOW);  // Tắt relay
    delay(100);                    // Đợi 100ms
  }
}
