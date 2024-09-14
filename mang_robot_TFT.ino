#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define TFT_CS   4
#define TFT_RST  16
#define TFT_DC   5

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
String mystring = "", tmp="";
bool check=false;
int head=2, ra=15, la=0;
Servo bothe, botra, botla;
signed long timer,timee,timeee;

void setup() {
 Serial.begin(9600);
  bothe.attach(head);
  botra.attach(ra);
  botla.attach(la);
  
  tft.init(240, 320);
  tft.fillScreen(ST77XX_WHITE);
  tft.setRotation(1);
  tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
  bothe.write(75);
  botla.write(180);
  botra.write(0);
  
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("- -");
  delay(500);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");
  delay(1000);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("^ ^");
  tft.setCursor(180, 30);
  tft.setTextSize(4);
  tft.println("Hi!");
  delay(5000);
  tft.setCursor(180, 30);
  tft.setTextSize(4);
  tft.println("   ");
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");
 timer = millis();
 timeee = millis();
timee = millis();
}
void loop() {
    
   if(Serial.available() > 0) check=false;
       while( check==false){
        if(Serial.available() > 0)
          { String dulieu = String(Serial.readString());
            tft.setTextSize(2);
            tft.setCursor(20, 30);
            tft.fillScreen(ST77XX_WHITE);
           // Serial.print(wrapText(dulieu));
            tft.print(wrapText(dulieu));
            delay(1000);
            if(dulieu)  {Serial.print(dulieu);  botra.write(50);}
            timer=millis();
            
      } if(Serial.available() == 0 && millis() - timer >20000)
        {check=true;
        tft.fillScreen(ST77XX_WHITE);
              timeee = millis();
             timee = millis(); }
      } 
      if(millis() -timee < 100000 )
      {
        if(millis() -timeee < 4000 )
        {
          timeee=millis(); tftprintwait();
         }
      }
      if(millis() -timee > 100000 && millis() -timee < 105000 ) tftprintkiss();
      
      if(millis() -timee > 105000 && millis() -timee < 130000 ){   tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("   ");tftprintlook(); 
  }
      
      if(millis() -timee > 130000 && millis() -timee < 240000 ) tftprintwait();
      
     if(millis() -timee > 240000 && millis() -timee < 420000) tftprintsleep();
     if(millis() -timee > 420000) {
      timeee = millis();
timee = millis();
     }
  ESP.wdtFeed(); 
  
 // Reset the watchdog timer
}       
String wrapText(String text) {
    String tam="";
    int length = text.length();
    const int maxLineLength = 23;
    int charCount = 0;

    for (int i = 0; i < length; i++) {
            if(text[i]!=' ')
       {
           tam+= text[i];
            charCount++;
       }
        if(text[i]==' ')
         {
              if (charCount >= maxLineLength && i != length - 1) {
            tam=tam+'\n';
            charCount = 0;
        }
        else {tam=tam+" "; charCount++;}
         }
    }
    return tam;
}
void tftprintwait()
{
  delay(100);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");
  
   delay(100);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("- -");
  
  delay(100);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");

   delay(100);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("- -");

  delay(100);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");
}
void tftprintsleep()
{
   tft.setCursor(50, 80);
   tft.setTextSize(13);
  tft.println("- -");
  tft.setTextSize(4);
tft.setTextSize(4);
  tft.setCursor(180, 70);
  tft.println("z");
  delay(200);
  tft.setCursor(210, 50);
  tft.println("Z");
  delay(200);
  tft.setTextSize(5);
  tft.setCursor(240, 30);
  tft.println("Z");
  delay(500);
    tft.setTextSize(5);
  tft.setCursor(240, 30);
  tft.println(" ");
  delay(200);
    tft.setCursor(210, 50);
  tft.println(" ");
  delay(200);
    tft.setTextSize(4);
  tft.setCursor(180, 70);
  tft.println(" ");
 delay(500);
}




void tftprintlook()
{
  bothe.write(150);
  tft.setCursor(70, 30);
  tft.setTextSize(13);
  tft.println("o O");
  delay(2000);

  tft.setCursor(70, 30);
  tft.setTextSize(13);
  tft.println("   ");
  bothe.write(0);
  tft.setCursor(30, 30);
  tft.setTextSize(13);
  tft.println("O o");

  delay(2000);
  tft.setCursor(30, 30);
  tft.setTextSize(13);
  tft.println("   ");
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");
  bothe.write(75);
  delay(1000);
}

void tftprintUwU()
{
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("uwU");
  delay(500);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("Uwu");
  delay(500);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("uwU");
  delay(500);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("Uwu");
  delay(500);
}

void tftprintT_T()
{
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("T T");
}

void tftprintkiss()
{
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("> <");
  tft.setTextSize(5);
  tft.print("     3  ");
  tft.setRotation(0);
  tft.setTextColor(ST77XX_CYAN, ST77XX_WHITE);
  tft.setTextSize(4);
  tft.setCursor(25, 240);
  tft.print("<3");
  delay(500);
  tft.setCursor(25, 240);
  tft.print("  ");
  tft.setCursor(25, 240);
  tft.print("<3");
  delay(500);
  tft.setCursor(25, 240);
  tft.print("  ");
  delay(500);
  tft.setCursor(25, 240);
  tft.print("<3");
  delay(500);
  tft.setCursor(25, 240);
  tft.print("  ");
  tft.setCursor(25, 240);
  tft.print("<3");
  delay(500);
  tft.setCursor(25, 240);
  tft.print("  ");
  delay(500);
  tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
  tft.setRotation(1);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("> <");
  tft.setTextSize(5);
  tft.print("        ");
   tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");
  botla.write(50);
  delay(1000);
  botla.write(180);
}

void tftprintOO()
{
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("o O");
  delay(500);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O o");
  delay(500);
}

void tftprintshy()
{
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("^ ^");
  tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
  tft.println(""   "");
  delay(1000);
  tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
  tft.setCursor(50, 80);
  tft.setTextSize(13);
  tft.println("O O");
  tft.println("     ");
}
