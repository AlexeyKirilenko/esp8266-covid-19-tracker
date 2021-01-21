//NodeMCU with ILI9341 TFT display circuit:
//
//  
// The ILI9341 TFT display is connected to the NodeMCU board as follows:
// CS pin is connected to D2 (ESP8266EX GPIO4),
// RST pin is connected to D3 (ESP8266EX GPIO0),
// D/C pin is connected to D4 (ESP8266EX GPIO2),
// MOSI pin is connected to D7 (ESP8266EX GPIO13),
// SCK pin is connected to D5 (ESP8266EX GPIO14),
// VCC and BL are connected to pin 3V3,
// GND is connected to pin GND of the NodeMCU board
// see README.md for more info


#include <ESP8266WiFi.h>                  // lib for wiwi connection
#include <WiFiClient.h>                   // lib for network connection to remote hosts
#include <TimeLib.h>                      // time  - optional
#include <ArduinoJson.h>                  // JSON parsing (Use 5 or 6 version)
#include <Adafruit_GFX.h>       // include Adafruit graphics library
#include <Adafruit_ILI9341.h>   // include Adafruit ILI9341 TFT library
// #include "FontsRus/TimesNRCyr10.h" //custom RUSSIAN font see more: https://github.com/immortalserg/AdafruitGFXRusFonts/blob/master/README.md 



#define SCREEN_WIDTH 320 // OLED display width, in pixels
#define SCREEN_HEIGHT 240 // OLED display height, in pixels
 
#define TFT_CS   4     // TFT CS  pin is connected to NodeMCU pin D2
#define TFT_RST   0     // TFT RST pin is connected to NodeMCU pin D3
#define TFT_DC    2     // TFT DC  pin is connected to NodeMCU pin D4
#define ILI9341_BLACK       0x0000  ///<   0,   0,   0
#define ILI9341_NAVY        0x000F  ///<   0,   0, 123
#define ILI9341_DARKGREEN   0x03E0  ///<   0, 125,   0
#define ILI9341_DARKCYAN    0x03EF  ///<   0, 125, 123
#define ILI9341_MAROON      0x7800  ///< 123,   0,   0
#define ILI9341_PURPLE      0x780F  ///< 123,   0, 123
#define ILI9341_OLIVE       0x7BE0  ///< 123, 125,   0
#define ILI9341_LIGHTGREY   0xC618  ///< 198, 195, 198
#define ILI9341_DARKGREY    0x7BEF  ///< 123, 125, 123
#define ILI9341_BLUE        0x001F  ///<   0,   0, 255
#define ILI9341_GREEN       0x07E0  ///<   0, 255,   0
#define ILI9341_CYAN        0x07FF  ///<   0, 255, 255
#define ILI9341_RED         0xF800  ///< 255,   0,   0
#define ILI9341_MAGENTA     0xF81F  ///< 255,   0, 255
#define ILI9341_YELLOW      0xFFE0  ///< 255, 255,   0
#define ILI9341_WHITE       0xFFFF  ///< 255, 255, 255
#define ILI9341_ORANGE      0xFD20  ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define ILI9341_PINK        0xFC18  ///< 255, 130, 198

// initialize ILI9341 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ImageReader reader; // Class w/image-reading functions

const char* ssid = "ADD YOU SSID";            // wifi ssid
const char* password = "ADD YOU PASS";    // wifi pass

String regionID = "2";                  // only for exUSSR users https://tech.yandex.ru/xml/doc/dg/reference/regions-docpage/

String SunriseTime, SunsetTime, Temperature;
char icon[20];

WiFiClient client;                        // create object for remote host

void setup() {
  
 Serial.begin(9600);                                         // inizialixe port for serial
  Serial.println("\n\n");
   
 
  
  WiFi.begin(ssid, password);                                 // connect
   Serial.println("try connect"); 
  while (WiFi.status() != WL_CONNECTED)                       // connection
    delay(500);                                               //  delay for 500 ms
    Serial.println("Wifi connect OK"); 
  while (!TimeAndWeather())                                   // sync time from https://yandex.com/time/sync.json?geo=2 and ESP8266 localtime
    delay(500);
     
 

}

void loop() {
  
   
   void(* resetFunc) (void) = 0; // declare  reset function
  Serial.println("sync ok"); 
  Serial.print("Tekuschie data i vremya: ");
  digitalClockDisplay();                                      // print DATE and time
  Serial.println();
  Serial.println("Voshod v " + SunriseTime);                  // SunriseTime
  Serial.println("Zakat v " + SunsetTime);                    // SunsetTime
  Serial.println(); 
  Serial.println("Temperatura: " + Temperature + " C");       // Temperature
  WeatherDisplay();                                           // weather description in russia
  
 // Rotations 0,2 = portrait  : 0->USB=right,upper : 2->USB=left,lower
 // Rotations 1,3 = landscape : 1->USB=left,upper  : 3->USB=right,lower
  //tft start
 tft.begin();
  unsigned long start = micros();
  //tft.cp437(true);
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(76, 10); 
  tft.setTextSize(5);
  tft.println((leadNull(hour()))+":"+(leadNull(minute())));
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(40, 70);
  tft.setTextSize(2);

  //tft.setFont(&TimesNRCyr10pt8b); // optional custom font
  tft.println("на улице:  "+ Temperature + " с");

  tft.setTextColor(ILI9341_RED);
  tft.setCursor(20, 180);
  tft.setTextSize(2);
  tft.println("Восход в:  "+ SunriseTime); //восход
  tft.setCursor(20, 220);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLUE);
  tft.println("Закат в:  " + SunsetTime); //закат 
 // tft.println (String(out));
  tft.setFont();
  delay(58000);
  Serial.println("reset after loop"); 
    resetFunc(); // reset
  
  
  
  delay(500);
  Serial.println("end of loop"); 
     
} //end of loop


bool TimeAndWeather () { 
   void(* resetFunc) (void) = 0; // объявляем функцию reset 
   // Функция синхронизации времени работы программы с реальным временем и получения информации о погоде
  if (client.connect("yandex.com",443)) {                                   // Если удаётся установить соединение с указанным хостом (Порт 443 для https)
    client.print("GET /time/sync.json?geo=" + regionID + " HTTP/1.1\r\nHost: yandex.com\r\nConnection: close\r\n\r\n"); // Отправляем параметры запроса
    delay(200);                                                             // Даём серверу время, чтобы обработать запрос
    char endOfHeaders[] = "\r\n\r\n";                                       // Системные заголовки ответа сервера отделяются от остального содержимого двойным переводом строки
    if (!client.find(endOfHeaders)) {                                       // Отбрасываем системные заголовки ответа сервера
      Serial.print("Invalid response");                                   // Если ответ сервера не содержит системных заголовков, значит что-то пошло не так
      return false;                                                         // и пора прекращать всё это дело
    }
    const size_t capacity = 700;                                            // Эта константа определяет размер буфера под содержимое JSON (https://arduinojson.org/v5/assistant/)
    DynamicJsonBuffer jsonBuffer(capacity);                                 // Инициализируем буфер под JSON
    
    JsonObject& root = jsonBuffer.parseObject(client);                      // Парсим JSON-модержимое ответа сервера
    client.stop();     // Разрываем соединение с сервером
    Serial.print("client stop"); 
    String StringCurrentTime = root["time"].as<String>().substring(0,10);   // Достаём значение реального текущего времени из JSON и отбрасываем от него миллисекунды
    String StringOffset =  root["clocks"][regionID]["offset"].as<String>(); // Достаём значение смещения времени по часовому поясу (в миллисекундах)
    SunriseTime =  root["clocks"][regionID]["sunrise"].as<String>();        // Достаём время восхода - Третий уровень вложенности пары ключ/значение clocks -> значение RegionID -> sunrise 
    SunsetTime =  root["clocks"][regionID]["sunset"].as<String>();          // Достаём время заката - Третий уровень вложенности пары ключ/значение clocks -> значение RegionID -> sunset
    Temperature =  root["clocks"][regionID]["weather"]["temp"].as<String>();// Достаём время заката - Четвёртый уровень вложенности пары ключ/значение clocks -> значение RegionID -> weather -> temp
    strcpy(icon, root["clocks"][regionID]["weather"]["icon"].as<String>().c_str());       // Достаём иконку - Четвёртый уровень вложенности пары ключ/значение clocks -> значение RegionID -> weather -> icon

    jsonBuffer.clear();                                                     // Очищаем буфер парсера JSON
    
    unsigned long CurrentTime = StringToULong(StringCurrentTime);           // Переводим значение реального времени в секундах, считанное с Яндекса, из String в unsigned long
    unsigned long Offset = StringToULong(StringOffset) / 1000;              // Переводим значение смещения времени по часовому поясу, считанное с Яндекса, из String в unsigned long и переводим его в секунды
    setTime(CurrentTime + Offset);                                          // Синхронизируем время
    Serial.println("get time"); 
    return true;
    
  }
} 

unsigned long StringToULong(String Str) {                     // Эта функция преобразует String в unsigned long
  unsigned long ULong = 0;
  for (int i = 0; i < Str.length(); i++) {                    // В цикле посимвольно переводим строку в unsigned long
     char c = Str.charAt(i);
     if (c < '0' || c > '9') break;
     ULong *= 10;
     ULong += (c - '0');
  }
  return ULong;
}

void digitalClockDisplay(){                                   // Эта функция выводит дату и время на монитор серийного порта
  Serial.print(leadNull(day()));
  Serial.print(".");
  Serial.print(leadNull(month()));
  Serial.print(".");
  Serial.print(year());
  Serial.print(" ");
  Serial.print(leadNull(hour()));
  Serial.print(":");
  Serial.print(leadNull(minute()));
  Serial.print(":");
  Serial.print(leadNull(second()));
  Serial.println();
}

String leadNull(int digits){                 //add main null
  String out = "";
  if(digits < 10)
    out += "0";                                               
  return out + String(digits);
}

void WeatherDisplay(){  
       //TFT START
       tft.begin();
       unsigned long start = micros();
       //tft.cp437(true);
       tft.fillScreen(ILI9341_BLACK);
       tft.setRotation(1);
       tft.setTextColor(ILI9341_WHITE);
       tft.setCursor(20, 150);
       tft.setTextSize(2);
       tft.setTextColor(ILI9341_BLUE);
   void(* resetFunc) (void) = 0; // declare reset for reconect to JSON URL every 60 seconds
  char * out = strtok(icon,"-");        // use first part of string before '-'
  while (out != NULL) {                 // use other parts in cirle before out was null
      if (String(out) == "skc")         // check all favicons from url
        Serial.println("Yasno");
      else if (String(out) == "ovc")
       Serial.println("Пасмурно");
        else if (String(out) == "bkn")
      Serial.println("Oblachno");
      else if (String(out) == "ra")
        Serial.println("Dozhd'");
      else if (String(out) == "ts")
        Serial.println("Groza");
      else if (String(out) == "sn")
        Serial.println("Sneg");
      else if (String(out) == "bl")
        Serial.println("Metel'");
      else if (String(out) == "fg")
        Serial.println("Tuman");
      else if (String(out) == "n")
        Serial.println("\nTemnoe vremya sutok");
      else if (String(out) == "d")
        Serial.println("\nSvetloe vremya sutok");
      
      out = strtok(NULL,"-");              // end part
   }
}