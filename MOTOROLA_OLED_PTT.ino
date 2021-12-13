#include "OLED_I2C.h"           
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(2, 3);
OLED myOLED(SDA, SCL, 8);  

String LCD_buf = "";
String Text = "";
String DisplayUpdatePrefix="ff340";
String CloseSquelchPrefix="f5353ff";
String OpenSquelchPrefix="f53503f";

extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup(){
   myOLED.begin();  // инициализируем экран
   mySerial.begin(9600);
   Serial.begin(9600);
   mySerial.listen();
   myOLED.clrScr(); // очищаем экран от надписей
   myOLED.setFont(SmallFont);        
}

void loop(){
  while (mySerial.available()) {
    byte RIB_buf = mySerial.read();
    if (RIB_buf == 0x50) {         
        if (LCD_buf.startsWith(OpenSquelchPrefix)){
        myOLED.print("BUSY", 0, 10);          
        }
        else if (LCD_buf.startsWith(CloseSquelchPrefix)){
        myOLED.print(" RX ", 0, 10);          
        }
        else if (LCD_buf.startsWith(DisplayUpdatePrefix))
        {
        myOLED.print(Text, 0, 20);          
        }
        myOLED.update();
        Serial.println(LCD_buf+"_"+Text);
        LCD_buf=""; 
        Text="";
    } 
    else{
      LCD_buf += String(RIB_buf, HEX);
      char c=char(RIB_buf);
      if (isPrintable(c)&&!(Text=="" && c=='4')){
        Text+=c;         
      }
    }
  }
  delay(100);
}
