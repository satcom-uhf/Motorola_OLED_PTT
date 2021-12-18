#include "OLED_I2C.h"           
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(2, 3);
OLED myOLED(SDA, SCL, 10);  

String LCD_buf = "";
String Text = "";
String DisplayUpdatePrefix="ff34011003433332";
                                   
String CloseSquelchPrefix="f53050";//rx
String OpenSquelchPrefix="f530580";//busy
String ScanONPrefix="f424a10dc";//scan on
String ScanOFFPrefix="f424a00dd";//scan off
String HIPrefix="f53530";//HI
String LOWPrefix="f53500";//LOW
extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumFont[];
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
        myOLED.print("  BUSY ", CENTER, 10);          
        }
        else if (LCD_buf.startsWith(CloseSquelchPrefix)){
         myOLED.print("   RX  ", CENTER, 10);          
        }
        else if (LCD_buf.startsWith(ScanONPrefix)){
         myOLED.print("SCAN ON ", CENTER, 25);          
        }
        else if (LCD_buf.startsWith(ScanOFFPrefix)){
         myOLED.print("SCAN OFF", CENTER, 25);          
        }
        else if (LCD_buf.startsWith(HIPrefix)){
         myOLED.print("HI ", 0, 10);          
        }
        else if (LCD_buf.startsWith(LOWPrefix)){
         myOLED.print("LOW", 0, 10);          
        }
        else if (LCD_buf.startsWith(DisplayUpdatePrefix))
        {
        
        myOLED.print(Text, 45, 40);
        myOLED.print("FREQ: ", 15, 40);           
        }
        myOLED.update();
        Serial.println(LCD_buf+":"+Text);
        LCD_buf=""; 
        Text="";
    } 
    else{
      LCD_buf += String(RIB_buf, HEX);
      char c=char(RIB_buf);
      if (isPrintable(c)&&!(Text=="" && c=='5')){
        Text+=c;         
      }
    }
  }
  delay(100);
}
