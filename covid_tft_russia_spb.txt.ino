/* This code works with ESP8266 12E (NodeMcu v1.0) its main function is to read the data from https://www.worldometers.info/
  and https://xn--80aesfpebagmfblc0a.xn--p1ai/
 */

#include <Adafruit_GFX.h>       //Libraries required to use the Display
#include <Adafruit_ST7735.h>  ////Libraries required to use the Display
#include <SPI.h>
#include <NTPClient.h>        // include NTPClient library
#include <TimeLib.h>          // Include Arduino time library

#include <ESP8266WiFi.h>        //Use ESP8266 functions                                              
#include <ESP8266HTTPClient.h> //http client

// -----------  Display pins  --------------------------------------
#define TFT_CS         15       //
#define TFT_RST        0   //
#define TFT_DC         2   // my pins - you shoud use own
                                                                       
const char* ssid = " ";              //Your router SSID and password                             
const char* password =  " ";    //Your router SSID and password
const char* host = "api.thingspeak.com";  //I read the data from this host                                   
const int httpPortRead = 80;                                           
                                                
const char* url1 = "/apps/thinghttp/send_request?api_key= ";     //cases //use your own parse string from https://thingspeak.com/ 
const char* url2 = "/apps/thinghttp/send_request?api_key= "; //death  //use your own parse string from https://thingspeak.com/  
 
int To_remove;      //There are some irrelevant data on the string and here's how I keep the index
                    //of those characters 

String Cases,Death,Recover,Data_Raw;  //Here I keep the numbers that I got

WiFiClient client;      //Create a WiFi client and http client                                                     
HTTPClient http; 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); //Those things are for the display
float p = 3.1415926;


void setup() {
  
  Serial.begin(9600);            //Start the serial communication and the display
  tft.initR(INITR_GREENTAB);                                                              
 
  WiFi.disconnect();             //Disconnect and reconnect to the Wifi you set                                                 
  delay(5000);                                                                  
  WiFi.begin(ssid, password);                                                   

  Serial.println("Connected to the WiFi network");   //Display feedback on the serial monitor                                        
  Serial.println(WiFi.localIP()); 
    
   
}

 //start loop part

 void loop()                                                                      
{
   

  
      
      //Reading 1: Reading of cases
    if( http.begin(host,httpPortRead,url1))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw = http.getString();   //Here we store the raw data string

              Serial.print("Cases (RAW value) :");  //I choosed to display it on the serial monitor to help you debug
              Serial.println(Data_Raw);
                                                    
              To_remove = Data_Raw.indexOf(">");                      //I look for the position of this symbol ">"                                 
              Data_Raw.remove(0,To_remove+1);                        //I remove it and everything that's before
              To_remove = Data_Raw.indexOf("<");                     //I look for the position of this symbol ">"                                
              Data_Raw.remove(To_remove,Data_Raw.length());          //I remove it and everything that's after
                                                                     //Example: This is the raw data received <td style="font-weight: bold; text-align:right">63,927</td>
                                                                     //First we look for ">" and we remove everything before it including it
                                                                     //We stay with this 63,927</td>
                                                                     //We look for "<" symbol and we remove it + everything after
                                                                     //We keep only this 63,927 as string
              Cases=Data_Raw; //cases=data_raw
                                                                 
                                                                                        
            }
        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }



        http.end();                                                                 
      } 
    else //If we can't connect to the HTTP
      {
        Serial.printf("[HTTP} Unable to connect\n");
      }
 
//Reading 2 is the same thing as reading 1

    if( http.begin(host,httpPortRead,url2))                                              
      {
        int httpCode = http.GET();                                                      
        if (httpCode > 0)                                                               
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw = http.getString();   
              
              Serial.print("Deaths (RAW value) :");
              Serial.println(Data_Raw);
                                                    
              To_remove = Data_Raw.indexOf(">");                                   
              Data_Raw.remove(0,To_remove+1); 
              To_remove = Data_Raw.indexOf("<");                                   
              Data_Raw.remove(To_remove,Data_Raw.length());

              Death=Data_Raw;

                                                                
                                                                                        
            }
        }
        else 
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }



        http.end();                                                                 
      } 
    else 
      {
        Serial.printf("[HTTP} Unable to connect\n");
      }
    
    
    ///////end////
    
    Print_TFT();  //Display to the TFT LCD, this function will vary depending on your display
    delay(5000);

    while (WiFi.status() != WL_CONNECTED)     //In case the Wifi connexion is lost                                    
      { 
        
        WiFi.disconnect();                                                        
        delay(5000);                                                             
        
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       //you can set it only in port monitor
        delay(1000);                                                             
      }

} 
  



void Print_TFT(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);
  tft.setCursor(20, 10);    //Horiz/Vertic
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_GREEN);
  tft.print("RUSSIA"); 

  tft.setCursor(15, 30);    //Horiz/Vertic
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print("COVID-19 TODAY");

  tft.setCursor(0, 55);    //Horiz/Vertic
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_BLUE);
  tft.print("Infected:");
  tft.setCursor(0, 80);
  tft.print(Cases);  

  tft.setCursor(0, 115);    //Horiz/Vertic
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_RED);
  //tft.print("DEAD:");
  tft.setCursor(5, 115);
  tft.setTextColor(ST77XX_RED);
  tft.print(Death); //
 delay(5000); 
  
}

//
 

  
