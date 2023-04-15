#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Data wire is plugged into port D2 on the ESP8266
#define ONE_WIRE_BUS D4
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

float tempSensor1, tempSensor2, tempSensor3, tempSensor4, tempSensor5;

// Addresses of 5 DS18B20s
uint8_t sensor1[8] = { 0x28, 0x4A, 0x75, 0x57, 0x04, 0x35, 0x3C, 0x14 };
uint8_t sensor2[8] = { 0x28, 0x46, 0xDF, 0x57, 0x04, 0xA2, 0x3C, 0x4A };
uint8_t sensor3[8] = { 0x28, 0xC6, 0x6A, 0x57, 0x04, 0xB1, 0x3C, 0x84 };
uint8_t sensor4[8] = { 0x28, 0x39, 0x3E, 0x57, 0x04, 0x85, 0x3C, 0x4E };
uint8_t sensor5[8] = { 0x28, 0x9D, 0x64, 0x57, 0x04, 0xF1, 0x3C, 0x6B };

/*Put your SSID & Password*/
const char* ssid = "yourWifi";  // Enter SSID here
const char* password = "yourpassword";  //Enter Password here

ESP8266WebServer server(80);             
 
void setup() {
  Serial.begin(115200);
  delay(100);
  
  sensors.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


 // Clear the buffer
  display.clearDisplay();

  // text display Starttext
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(F("Heizung")); 
  display.setTextSize(1);
  display.setCursor(0,20);
  display.println("Markus Weber");
  display.display();
  delay(2000);
  
}
void loop() {
  server.handleClient();
  
display.clearDisplay(); 
      display.setCursor(0,0);
      display.setTextSize(1);
      display.println("Heizungstemperaturen");
      display.println();
              display.print("Direktkreis:   ");
      display.print((int)tempSensor1);
      display.print(" ");
      display.print((char)247);
      display.println(F("C"));
            display.print("Mischkreis:    ");
      display.print((int)tempSensor2);
        display.print(" ");
      display.print((char)247);
      display.println(F("C"));
            display.print("Ruecklauf:     ");
      display.print((int)tempSensor3);
        display.print(" ");
      display.print((char)247);
      display.println(F("C"));
            display.print("Puffer oben:   ");
      display.print((int)tempSensor4);
        display.print(" ");
      display.print((char)247);
      display.println(F("C"));
            display.print("Puffer unten:  ");
      display.print((int)tempSensor5);
        display.print(" ");
      display.print((char)247);
      display.println(F("C"));
               display.print("  IP: ");
      display.println(WiFi.localIP());
      display.display();

}

void handle_OnConnect() {
  sensors.requestTemperatures();
  tempSensor1 = sensors.getTempC(sensor1); // Gets the values of the temperature
  tempSensor2 = sensors.getTempC(sensor2); // Gets the values of the temperature
  tempSensor3 = sensors.getTempC(sensor3); // Gets the values of the temperature
  tempSensor4 = sensors.getTempC(sensor4); // Gets the values of the temperature
  tempSensor5 = sensors.getTempC(sensor5); // Gets the values of the temperature
  server.send(200, "text/html", SendHTML(tempSensor1,tempSensor2,tempSensor3, tempSensor4, tempSensor5)); 
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
String SendHTML(float tempSensor1,float tempSensor2,float tempSensor3,float tempSensor4,float tempSensor5){
  String ptr = "<!DOCTYPE html>";
  ptr +="<html>";
  ptr +="<head>";
  ptr +="<title>Temperaturen Heizung</title>";
  ptr +="<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr +="<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
  ptr +="<style>";
  ptr +="html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444;}";
  ptr +="body{margin-top: 50px;} ";
  ptr +="h1 {margin: 50px auto 30px;} ";
  ptr +=".side-by-side{display: table-cell;vertical-align: middle;position: relative;}";
  ptr +=".text{font-weight: 600;font-size: 19px;width: 200px;}";
  ptr +=".temperature{font-weight: 300;font-size: 50px;padding-right: 15px;}";
  ptr +=".living-room .temperature{color: #3B97D3;}";
  ptr +=".bedroom .temperature{color: #F29C1F;}";
  ptr +=".kitchen .temperature{color: #26B99A;}";
  ptr +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -5px;top: 15px;}";
  ptr +=".data{padding: 10px;}";
  ptr +=".container{display: table;margin: 0 auto;}";
  ptr +=".icon{width:82px}";
  ptr +="</style>";
  ptr +="<script>\n";
ptr +="setInterval(loadDoc,1000);\n";
ptr +="function loadDoc() {\n";
ptr +="var xhttp = new XMLHttpRequest();\n";
ptr +="xhttp.onreadystatechange = function() {\n";
ptr +="if (this.readyState == 4 && this.status == 200) {\n";
ptr +="document.body.innerHTML =this.responseText}\n";
ptr +="};\n";
ptr +="xhttp.open(\"GET\", \"/\", true);\n";
ptr +="xhttp.send();\n";
ptr +="}\n";
ptr +="</script>\n";
  ptr +="</head>";
  ptr +="<body>";
  ptr +="<h1>Temperaturen Heizung</h1>";
  ptr +="<div class='container'>";
  ptr +="<div class='Direktkreis'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<?xml version='1.0' encoding='UTF-8'?>";
  ptr +="<svg version='1.1' viewBox='0 0 100 100' xmlns='http://www.w3.org/2000/svg'><desc>Created with Fabric.js 3.6.6</desc><defs><marker id='Arrow2Send' overflow='visible' orient='auto'><path transform='scale(.3) rotate(180) translate(-2.3)' d='m8.7 4-11-4 11-4c-1.7 2.4-1.7 5.6-6e-7 8z' fill='context-stroke' fill-rule='evenodd' stroke='context-stroke' stroke-linejoin='round' stroke-width='.62'/></marker><radialGradient id='radialGradient5179' cx='49' cy='53' r='23' gradientTransform='matrix(1 .023 -.054 2.4 3.5 -83)' gradientUnits='userSpaceOnUse'><stop stop-color='#f90909' offset='0'/><stop stop-color='#0100ff' stop-opacity='0' offset='1'/></radialGradient></defs><g transform='matrix(1 0 0 -1 0 87)'><circle cx='49' cy='43' r='22' fill='url(#radialGradient5179)' opacity='.65' stroke='#000' stroke-linecap='round' stroke-width='2.7'/><path transform='matrix(.72 -.6 .57 .69 -19 43)' d='m70 32c4.6 3.9-7.7 8.2-5.7 14 2 5.7 14 1.3 13 7.2-1.1 5.9-11-2.5-15 2-3.9 4.6 6 13 0.32 15-5.7 2-3.3-11-9.2-12-5.9-1.1-8.3 12-13 7.8-4.6-3.9 7.7-8.2 5.7-14s-14-1.3-13-7.2c1.1-5.9 11 2.5 15-2s-6-13-0.32-15c5.7-2 3.3 11 9.2 12 5.9 1.1 8.3-12 13-7.8z' opacity='.28' stroke='#000' stroke-linecap='round' stroke-opacity='.63' stroke-width='.5'/><path d='m49 44-0.28 34' fill='none' marker-end='url(#Arrow2Send)' stroke='#ff7d00' stroke-width='4.8'/><path d='m49 2.2-0.28 34' fill='none' marker-end='url(#Arrow2Send)' stroke='#ff7d00' stroke-width='4.8'/><circle cx='49' cy='43' r='2.9' fill='#040000' opacity='.74' stroke='#010101' stroke-linecap='round' stroke-width='2.7'/></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Direktkreis (Altbau)</div>";
  ptr +="<div class='side-by-side temperature'>";
  ptr +=(int)tempSensor1;
  ptr +="<span class='superscript'>&deg;C</span></div>";
  ptr +="</div>";
  ptr +="<div class='Mischkreis'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<?xml version='1.0' encoding='UTF-8'?>";
  ptr +="<svg version='1.1' viewBox='0 0 100 100' xmlns='http://www.w3.org/2000/svg'><desc>Created with Fabric.js 3.6.6</desc><defs><marker id='Arrow2Send' overflow='visible' orient='auto'><path transform='scale(.3) rotate(180) translate(-2.3)' d='m8.7 4-11-4 11-4c-1.7 2.4-1.7 5.6-6e-7 8z' fill='context-stroke' fill-rule='evenodd' stroke='context-stroke' stroke-linejoin='round' stroke-width='.62'/></marker><radialGradient id='radialGradient5179' cx='49' cy='53' r='23' gradientTransform='matrix(1 .023 -.054 2.4 3.5 -83)' gradientUnits='userSpaceOnUse'><stop stop-color='#f90909' offset='0'/><stop stop-color='#0100ff' stop-opacity='0' offset='1'/></radialGradient></defs><g transform='matrix(1 0 0 -1 0 94)'><circle cx='49' cy='43' r='22' fill='url(#radialGradient5179)' opacity='.65' stroke='#000' stroke-linecap='round' stroke-width='2.7'/><path transform='matrix(.72 -.6 .57 .69 -19 43)' d='m70 32c4.6 3.9-7.7 8.2-5.7 14 2 5.7 14 1.3 13 7.2-1.1 5.9-11-2.5-15 2-3.9 4.6 6 13 0.32 15-5.7 2-3.3-11-9.2-12-5.9-1.1-8.3 12-13 7.8-4.6-3.9 7.7-8.2 5.7-14s-14-1.3-13-7.2c1.1-5.9 11 2.5 15-2s-6-13-0.32-15c5.7-2 3.3 11 9.2 12 5.9 1.1 8.3-12 13-7.8z' opacity='.28' stroke='#000' stroke-linecap='round' stroke-opacity='.63' stroke-width='.5'/><path d='m83 12-15 0.23-13 22' fill='none' marker-end='url(#Arrow2Send)' stroke='#fb0511' stroke-width='5.7'/><path d='m15 12 15 0.11 12 21' fill='none' marker-end='url(#Arrow2Send)' stroke='#131bf7' stroke-width='5.7'/><path d='m49 44-0.28 34' fill='none' marker-end='url(#Arrow2Send)' stroke='#ff7d00' stroke-width='4.8'/><circle cx='49' cy='43' r='2.9' fill='#040000' opacity='.74' stroke='#010101' stroke-linecap='round' stroke-width='2.7'/></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Mischkreis (Anbau)</div>";
  ptr +="<div class='side-by-side temperature'>";
  ptr +=(int)tempSensor2;
  ptr +="<span class='superscript'>&deg;C</span></div>";
  ptr +="</div>";
  ptr +="<div class='Ruecklauf'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<?xml version='1.0' encoding='UTF-8'?>";
  ptr +="<svg version='1.1' viewBox='0 0 100 100' xmlns='http://www.w3.org/2000/svg'><desc>Created with Fabric.js 3.6.6</desc><defs><marker id='Arrow2Send' overflow='visible' orient='auto'><path transform='scale(.3) rotate(180) translate(-2.3)' d='m8.7 4-11-4 11-4c-1.7 2.4-1.7 5.6-6e-7 8z' fill='context-stroke' fill-rule='evenodd' stroke='context-stroke' stroke-linejoin='round' stroke-width='.62'/></marker><radialGradient id='radialGradient5179' cx='49' cy='53' r='23' gradientTransform='matrix(1 .023 -.054 2.4 3.5 -83)' gradientUnits='userSpaceOnUse'><stop stop-color='#f90909' offset='0'/><stop stop-color='#0100ff' stop-opacity='0' offset='1'/></radialGradient></defs><g transform='matrix(1 0 0 -1 0 105)'><circle cx='49' cy='43' r='22' fill='url(#radialGradient5179)' opacity='.65' stroke='#000' stroke-linecap='round' stroke-width='2.7'/><path transform='matrix(.72 -.6 .57 .69 -19 43)' d='m70 32c4.6 3.9-7.7 8.2-5.7 14 2 5.7 14 1.3 13 7.2-1.1 5.9-11-2.5-15 2-3.9 4.6 6 13 0.32 15-5.7 2-3.3-11-9.2-12-5.9-1.1-8.3 12-13 7.8-4.6-3.9 7.7-8.2 5.7-14s-14-1.3-13-7.2c1.1-5.9 11 2.5 15-2s-6-13-0.32-15c5.7-2 3.3 11 9.2 12 5.9 1.1 8.3-12 13-7.8z' opacity='.28' stroke='#000' stroke-linecap='round' stroke-opacity='.63' stroke-width='.5'/><path d='m49 85-0.28-34' fill='none' marker-end='url(#Arrow2Send)' stroke='#0021ff' stroke-width='4.8'/><circle cx='49' cy='43' r='2.9' fill='#040000' opacity='.74' stroke='#010101' stroke-linecap='round' stroke-width='2.7'/></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Ruecklauf</div>";
  ptr +="<div class='side-by-side temperature'>";
  ptr +=(int)tempSensor3;
  ptr +="<span class='superscript'>&deg;C</span></div>";
  ptr +="<div class='Pufferunten'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<?xml version='1.0' encoding='UTF-8'?>";
  ptr +="<svg version='1.1' viewBox='0 0 100 100' xmlns='http://www.w3.org/2000/svg'><desc>Created with Fabric.js 3.6.6</desc><g transform='matrix(0 -.55 .95 0 46 53)'><path transform='translate(-62 -21)' d='m20 0h84c11 0 20 9.3 20 21 0 11-9 21-20 21h-84c-11 0-20-9.3-20-21 0-11 9-21 20-21z' fill='#00ceff' stroke='#000' stroke-width='4' vector-effect='non-scaling-stroke'/></g><g transform='matrix(-.28 0 0 .34 63 27)'><path transform='translate(-60 -30)' d='m0 15h90v-15l30 30-30 30v-15h-90z' fill='#ff3600' fill-rule='evenodd' stroke='#000' stroke-miterlimit='10' stroke-width='3' vector-effect='non-scaling-stroke'/></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Puffer oben (Warmwasser)</div>";
  ptr +="<div class='side-by-side temperature'>";
  ptr +=(int)tempSensor4;
  ptr +="<span class='superscript'>&deg;C</span></div>";
  ptr +="<div class='Pufferoben'>";
  ptr +="<div class='side-by-side icon'>";
  ptr +="<?xml version='1.0' encoding='UTF-8'?>";
  ptr +="<svg version='1.1' viewBox='0 0 100 100' xmlns='http://www.w3.org/2000/svg'><desc>Created with Fabric.js 3.6.6</desc><g transform='matrix(0 -.55 .95 0 46 53)'><path transform='translate(-62 -21)' d='m20 0h84c11 0 20 9.3 20 21 0 11-9 21-20 21h-84c-11 0-20-9.3-20-21 0-11 9-21 20-21z' fill='#00ceff' stroke='#000' stroke-width='4' vector-effect='non-scaling-stroke'/></g><g transform='matrix(-.28 0 0 .34 63 81)'><path transform='translate(-60 -30)' d='m0 15h90v-15l30 30-30 30v-15h-90z' fill='#ff3600' fill-rule='evenodd' stroke='#000' stroke-miterlimit='10' stroke-width='3' vector-effect='non-scaling-stroke'/></g></svg>";
  ptr +="</div>";
  ptr +="<div class='side-by-side text'>Puffer unten</div>";
  ptr +="<div class='side-by-side temperature'>";
  ptr +=(int)tempSensor5;
  ptr +="<span class='superscript'>&deg;C</span></div>";
  ptr +="</div>";
  ptr +="</div>";
  ptr +="</body>";
  ptr +="</html>";
  return ptr;
}
