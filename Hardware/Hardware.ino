//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           WEATHER STATION REPORT     
//              620157609
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Load required libraries
#include <SPI.h>
#include <ctype.h>
#include <DHT.h>
#include <Wire.h>
#include <math.h>
#include <rom/rtc.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Adafruit_ST7735.h>
#include <Adafruit_BMP280.h>

#ifndef _WIFI_H 
#include <WiFi.h>
#endif

#ifndef STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#include <stdio.h>
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif 
 
#ifndef ARDUINOJSON_H
#include <ArduinoJson.h>
#endif



//IMPORT IMAGE
#include "uwi.h"

// MQTT CLIENT CONFIG  
static const char* pubtopic      = "620157609";                    // Add your ID number here
static const char* subtopic[]    = {"620157609_sub","/elet2415"};  // Array of Topics(Strings) to subscribe to
static const char* mqtt_server   = "www.yanacreations.com";         // Broker IP address or Domain name as a String 
static uint16_t mqtt_port        = 1883;

// WIFI CREDENTIALS
const char* ssid       = "MonaConnect"; // Add your Wi-Fi ssid
const char* password   = ""; // Add your Wi-Fi password 

// const char* ssid       = "Digicel_WiFi_Qdyx"; // Add your Wi-Fi ssid
// const char* password   = "524CgnEF"; // Add your Wi-Fi password 

// TASK HANDLES 
TaskHandle_t xMQTT_Connect          = NULL; 
TaskHandle_t xNTPHandle             = NULL;  
TaskHandle_t xLOOPHandle            = NULL;  
TaskHandle_t xUpdateHandle          = NULL;
TaskHandle_t xButtonCheckeHandle    = NULL;  

// FUNCTION DECLARATION   
void checkHEAP(const char* Name);   // RETURN REMAINING HEAP SIZE FOR A TASK
void initMQTT(void);                // CONFIG AND INITIALIZE MQTT PROTOCOL
unsigned long getTimeStamp(void);   // GET 10 DIGIT TIMESTAMP FOR CURRENT TIME
void callback(char* topic, byte* payload, unsigned int length);
void initialize(void);
bool publish(const char *topic, const char *payload); // PUBLISH MQTT MESSAGE(PAYLOAD) TO A TOPIC
void vButtonCheck( void * pvParameters );
void vUpdate( void * pvParameters ); 
void GDP(void);   // GENERATE DISPLAY PUBLISH

//TFT DISPLAY 
#define TFT_CS    13
#define TFT_RST   12
#define TFT_DC    14
#define TFT_MOSI  27
#define TFT_CLK   32
#define TFT_MISO  35

//  DHT SENSOR
#define DHTPIN 23           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22       // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// SOIL MOISTURE SENSOR
#define AOUT_PIN 33  /* Soil moisture sensor O/P pin */

// SMP SENSOR
Adafruit_BMP280 bmp;
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

//Colour Definitions for TFT DISPLAY
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x2108
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF


// variables 
uint32_t runTime = -99999;       // time for next update
int tesmod =  0;
float ah; //absolute humidity = water vapor density in g/m*3
float relhum; // relative humidity in %
float t; //temperature in celsius

//float td; //dew point in celsius

float tf; //temperature in fahrenheit
float tdf; // dew point temperature in fahrenheit
// char TempCelciusFahrenheit[6];

#ifndef NTP_H
#include "NTP.h"
#endif

#ifndef MQTT_H
#include "mqtt.h"
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////
//          CODE BEGINS 
//              HERE
///////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(9600);
  Serial.println("WEATHER STATION TEST");

  // dht 
  dht.begin();

  //BMP
  bmp.begin();

  // INIT WIFI, MQTT & NTP
  initialize();           

  // tft init
  tft.begin();
  tft.fillScreen(BLUE);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);


  tft.drawRGBBitmap(0,0, uwi, 240, 320); // DRAW IMAGE ON SCREEN

  tft.setRotation(1); // Set Rotation at 0 degress (default)

  tft.fillScreen(RED); //Set Background Color with BLACK
  tft.setCursor (7,20);
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("Temperature "); 

  tft.setCursor (7,50);
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("Fahrenheit "); 

  tft.setCursor (7,85);
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("Humidity"); 

  tft.setCursor (7,120);
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("Heat Index C");

  tft.setCursor (7,155);
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("Soil Moisture "); 

  tft.setCursor (7,220);
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("Pressure "); 

  tft.setCursor (7,187);
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("Altitude ");

  tft.fillRect(0,238,320,2,BLACK);
  tft.fillRect(0,210,320,2,BLACK);
  tft.fillRect(0,178,320,2,BLACK);
  tft.fillRect(0,145,320,2,BLACK);
  tft.fillRect(0,110,320,2,BLACK);
  tft.fillRect(0,75,320,2,BLACK);
  tft.fillRect(0,41,320,2,BLACK);
  tft.fillRect(0,10,320,2,BLACK);
}

void loop() {
  //////////////Soil Moisture Sensor//////////////////////////////////////////
  //int value = analogRead(AOUT_PIN); // read the analog value from sensor

  //Serial.print("Moisture value: ");
  //Serial.println(value);
  //////////////////Soil Moisture Sensor//////////////////////////////////////////

  

  ////////////////////////////////////////////////////////////


  /////////////////DHT CODE FOR THE LOOP////////////////////////////////////////////
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float heat_cel = dht.computeHeatIndex(t, h, false);
  
  ////////////////DHT CODE FOR THE LOOP////////////////////////////////////////////

  ////////////////SOIL MOISTURE///////////////////////////////////////////////////
  float value= analogRead(AOUT_PIN);
  int _moisture = map(value, 3500, 1650, 0, 100); 

  // int min_value = 1000;  // Adjust this value based on your sensor's characteristics
  // int _moisture = 0;

  // if (value >= min_value) {
  //   _moisture = (value - min_value) / (300 - min_value);
  // } else {
  //   // Set moisture to 0 (or a minimum value) if value is below minimum
  //   _moisture = 0;
  // }
  ////////////////SOIL MOISTURE///////////////////////////////////////////////////


  ////////////////DHT CODE FOR THE LOOP////////////////////////////////////////////
  relhum = dht.readHumidity();
  t = dht.readTemperature();
  ////////////////DHT CODE FOR THE LOOP////////////////////////////////////////////

  ////////////////BMP SENSOR///////////////////////////////////////////////////////
  float alt= bmp.readAltitude();
  float pa= bmp.readPressure();
  ////////////////BMP SENSOR///////////////////////////////////////////////////////

   if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // UPDATING TFT

  tft.setCursor(233, 50);  
  tft.setTextSize (2);
  tft.setTextColor ( YELLOW , BLACK);
  tft.print (" ");

  tft.setCursor (160,120); // Heat index in celsius
  tft.setTextSize (2);
  tft.setTextColor (MAGENTA,BLACK);
  tft.print (heat_cel,0); 
  tesmod=1;

  tft.setCursor (160,155); // Space
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print ("");
  tesmod=1;

   
  tft.setCursor (140,50); //Fahrenheit
  tft.setTextSize (2);
  tft.setTextColor (BLUE,BLACK);
  tft.print (f,0); 

  tft.setCursor (120, 85); // Humidity
  tft.setTextSize (2);
  tft.setTextColor (YELLOW, BLACK);
  tft.print (h, 1); tft.print ('%');
  tesmod = 1;

  tft.setCursor (160, 157); //SOILM
  tft.setTextSize(2);
  tft.setTextColor( CYAN,BLACK);
  tft.print(" ");
  tft.print(_moisture); tft.print(" % ");
  // tesmod=1;

  tft.setCursor (150, 20); //DHT sensor
  tft.setTextSize (2);
  tft.setTextColor (MAGENTA, BLACK);
  tft.print (t, 1); tft.print ('C');
  tesmod = 1;

  tft.setCursor (90,187); // Altitude
  tft.setTextSize(2);
  tft.setTextColor( CYAN,BLACK);
  tft.print(" ");
  tft.print(alt); tft.print('m');
  

  tft.setCursor (150,220); // Prssure
  tft.setTextSize (2);
  tft.setTextColor (GREEN,BLACK);
  tft.print("");
  tft.print (pa); tft.print('Pa');
  tesmod=1;

if (millis() - runTime >= 1000) { // 500ms
  runTime = millis();   
}

  // UPDATING SERIAL MONITOR
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C ");

  Serial.print("Fahrenheight: ");
  Serial.print(f);
  Serial.println(" °F\t");

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");

  Serial.print("Heat index C: ");
  Serial.print(heat_cel);
  Serial.println(" °C ");

  Serial.print("Soil Moisture: ");
  Serial.print(_moisture);
  Serial.println();


  Serial.print("Altitude: ");
  Serial.print(alt);
  Serial.println(" m ");

  Serial.print("Pressure: ");
  Serial.print(pa);
  Serial.println(" Pa ");

  
  delay(3000);
}

//####################################################################
//#                          UTIL FUNCTIONS                          #       
//####################################################################

void vUpdate( void * pvParameters )  {
    //configASSERT( ( ( uint32_t ) pvParameters ) == 1 );    

    for( ;; ) {
          // Task code goes here.   
           
          float h = dht.readHumidity();
          float t = dht.readTemperature();
          float f = dht.readTemperature(true);
          float heat_cel = dht.computeHeatIndex(t, h, false);
          float alt= bmp.readAltitude();
          float pa= bmp.readPressure();
          float value= analogRead(AOUT_PIN);
          int _moisture = map(value, 3500, 1650, 0, 100); 


          // PUBLISH to topic every second.
          StaticJsonDocument<1000> doc; // Create JSon object
          char message[1100]  = {0};

          // Add key:value pairs to JSon object
          doc["id"]         = "620157609";
          doc["timestamp"]  = getTimeStamp();
          //doc["number"]     = number;
          doc["Temperature"]= t;
          doc["Farenheit"]  = f;
          doc["Humidity"]  = h;
          doc["HeatIndex"]  = heat_cel;
          doc["SoilMoisture"]  = _moisture;
          doc["Altitude"]  = alt;
          doc["Pressure"]  = pa;

          

          serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array

          if(mqtt.connected() ){
            publish(pubtopic, message);
          }
          
            
        vTaskDelay(1000 / portTICK_PERIOD_MS);  
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // ############## MQTT CALLBACK  ######################################
  // RUNS WHENEVER A MESSAGE IS RECEIVED ON A TOPIC SUBSCRIBED TO
  
  Serial.printf("\nMessage received : ( topic: %s ) \n",topic ); 
  char *received = new char[length + 1] {0}; 
  
  for (int i = 0; i < length; i++) { 
    received[i] = (char)payload[i];    
  }

  // PRINT RECEIVED MESSAGE
  Serial.printf("Payload : %s \n",received);

 /////////////////////////////////////////////////////MIGHT NEED TO DELETE
 
  // CONVERT MESSAGE TO JSON
  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, received);  

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }


  // PROCESS MESSAGE
  const char* type = doc["type"];

  }

bool publish(const char *topic, const char *payload){   
     bool res = false;
     try{
        res = mqtt.publish(topic,payload);
        // Serial.printf("\nres : %d\n",res);
        if(!res){
          res = false;
          throw false;
        }
     }
     catch(...){
      Serial.printf("\nError (%d) >> Unable to publish message\n", res);
     }
  return res;
}

void GDP(void){
  // GENERATE, DISPLAY THEN PUBLISH INTEGER

  // GENERATE a random integer 
  /* Add code here to generate a random integer and then assign 
     this integer to number variable below
  */

   //number = random(0,10);   

  // DISPLAY integer on 7Seg. by 
  /* Add code here to calling appropriate function that will display integer to 7-Seg*/

  //Display(number);

  
  // PUBLISH number to topic.
  StaticJsonDocument<1000> doc; // Create JSon object
  char message[1100]  = {0};
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float heat_cel = dht.computeHeatIndex(t, h, false);
  float alt= bmp.readAltitude();
  float pa= bmp.readPressure();
  float value= analogRead(AOUT_PIN);
  int _moisture = map(value, 3500, 1650, 0, 100); 

  // Add key:value pairs to Json object according to below schema
  // ‘{"id": "student_id", "timestamp": 1702212234, "number": 9, "ledA": 0, "ledB": 0}’
  doc["id"]          = "620157609";
  doc["timestamp"]   = getTimeStamp();
  //doc["number"]      = number;
  doc["Temperature"] = t;
  doc["Farenheit"]   = f;
  doc["Humidity"]    = h;
  doc["HeatIndex"]= heat_cel;
  doc["SoilMoisture"]= _moisture;
  doc["Altitude"]     = alt;
  doc["Pressure"]    = pa;
  /*Add code here to insert all other variabes that are missing from Json object
  according to schema above
  */



  serializeJson(doc, message);  // Seralize / Covert JSon object to JSon string and store in char* array
  publish(pubtopic, message);

}

unsigned long getTimeStamp(void) {
          // RETURNS 10 DIGIT TIMESTAMP REPRESENTING CURRENT TIME
          time_t now;         
          time(&now); // Retrieve time[Timestamp] from system and save to &now variable
          return now;
}