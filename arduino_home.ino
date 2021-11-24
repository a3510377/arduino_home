/** 
 * use LoLin
 * pins:
 * * button -> d0
 * * oled.SCK -> D1
 * * oled.SDA -> D2
 * * dht11.DATA -> D5
 * * rtc -> 
 * https://cdcdengue.azurewebsites.net/VectorMap.aspx
 */

#include "main.h"
#include "sets.h"
#include <FS.h>
#include <SPI.h>
#include <DHT.h>
#include <Wire.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAsyncWebServer.h>

/* btn Low time */
int checkBtnTime = 0;
int delayDHT = 0;
/** web */
AsyncWebServer server(80);
/** ws */
AsyncWebSocket ws("/ws");
/** OLED */
Adafruit_SSD1306 display(128, 64, &Wire, -1);
ThreeWire myWire(D7, D8, D3);
/** Clock */
RtcDS1302<ThreeWire> Rtc(myWire); // Clock
/** DHT */
DHT dht(D5, dhtType);

/* 定義函數 */
void webInit();
void callBtn(int date);
void wsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
temperature loopDHT(DHT _dht);

void setup()
{
    /* DHT setup */
    dht.begin();

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    Rtc.Begin();        // time setup
    SPIFFS.begin();     // load file setup
    Serial.begin(9600); // Serial setup
    webInit();          // web init
    Serial.print(__DATE__);
    Serial.println(__TIME__);
    if (debug)
        Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));

    ws.onEvent(wsEvent);
    server.addHandler(&ws);

    /* pin setup */
    pinMode(D0, INPUT);
}

/* 主程式延遲 .5s */
void loop()
{
    ws.cleanupClients();
    bool btnIs = digitalRead(D0);
    if (btnIs)
        checkBtnTime++;
    else if (checkBtnTime > 0)
    {
        callBtn(checkBtnTime);
        checkBtnTime = 0;
    }
    loopDHT(dht);
    delay(s * .5); //延時.5s
}

/** server init */
void webInit()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", R"rawliteral(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <link rel="icon" href="/favicon.ico" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Vite App</title>
    <script type="module" crossorigin src="/assets/index.b11105bc.js"></script>
    <link rel="modulepreload" href="/assets/vendor.41b16fa2.js" />
    <link rel="stylesheet" href="/assets/index.32536ca2.css" />
  </head>

  <body>
    <div id="app"></div>
  </body>
</html>
)rawliteral"); });
    Dir dir = SPIFFS.openDir("/web");
    while (dir.next())
    {
        int str_len = dir.fileName().length() + 1;
        char char_array[str_len];
        dir.fileName().toCharArray(char_array, str_len);
        server.serveStatic(char_array, SPIFFS, char_array);
        Serial.print("load: ");
        Serial.println(char_array);
    }
    server.on("/setTime", HTTP_POST, [](AsyncWebServerRequest *request) {

    });
    server.begin();
}
temperature loopDHT(DHT _dht)
{
    temperature _Data(_dht.readHumidity(),
                      _dht.readTemperature(),
                      _dht.readTemperature(true));
    RtcDateTime dt = Rtc.GetDateTime();
    int second = dt.Second(),
        minute = dt.Minute(),
        hour = dt.Hour(),
        month = dt.Month(),
        year = dt.Year(),
        day = dt.Day();

    char Date[25],
        Time[25];
    sprintf(Date, "%02u/%02u/%02u", year, month, day);
    sprintf(Time, "%02u:%02u:%02u", hour, minute, second);
    display.clearDisplay();
    display.display();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(25, 0);
    display.setTextSize(1.5);
    display.println(Date);
    display.setTextSize(1);
    display.println("---------------------");
    display.setCursor(15, 20);
    display.setTextSize(2);
    display.println(Time);
    if (!_Data.checkError())
    {
        Serial.print("濕度: ");
        Serial.print(_Data.getH());
        Serial.print("%\t");
        Serial.print("攝氏溫度: ");
        Serial.print(_Data.getT());
        Serial.print("*C\t");
        Serial.print("華氏溫度: ");
        Serial.print(_Data.getF());
        Serial.print("*F\n");

        display.setCursor(28, 40);
        display.setTextSize(3);
        display.print(_Data.getT(), 1);
        display.print((char)247);
        String data = "";
        data += "Time:" + String(Time) + ";";
        data += "Date" + String(Date) + ";";
        data += "H:" + (String)_Data.getH() + ";";
        data += "C:" + (String)_Data.getT() + ";";
        data += "F:" + (String)_Data.getF() + ";";
        ws.textAll(data);
    }
    display.display();
    /*  */
    return _Data;
}

int toSbtnDelayTime(int date)
{
    return date / (s * .5);
}

void callBtn(int date)
{
    if (date < toSbtnDelayTime(2e3))
    {
    }
    else if (date < toSbtnDelayTime(3e3))
    {
    }
    else if (date < toSbtnDelayTime(3e3))
    {
    }
    else if (date < toSbtnDelayTime(3e3))
    {
    }
    else if (date < toSbtnDelayTime(3e3))
    {
    }
}

void wsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}
