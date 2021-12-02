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
float oldT;
String wifiName;
String wifiPassword;

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
IPAddress IP(192, 168, 1, 1);

/* 定義函數 */
void webInit();
void callBtn(int date);
void upDataWifiConfig();
temperature loopDHT(DHT _dht);
void wsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void setWifiType();
String TimeString(String Type = "Date");

void setup()
{
    /* DHT setup */
    dht.begin();

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    Rtc.Begin();        // time setup
    Serial.begin(9600); // Serial setup
    SPIFFS.begin();     // load file setup

    upDataWifiConfig();
    setWifiType();

    Serial.print(__DATE__);
    Serial.println(__TIME__);

    if (debug)
        Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));

    webInit(); // web init
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
              { request->send(SPIFFS, "/web/index.html", "text/html"); });
    Dir dir = SPIFFS.openDir("/web");
    while (dir.next())
    {
        String fileName = dir.fileName();
        String Uri = fileName.substring(4);
        int str_len = fileName.length() + 1;
        int Uri_len = Uri.length() + 1;
        char char_array[str_len];
        char char_arrayUri[Uri_len];

        fileName.toCharArray(char_array, str_len);
        Uri.toCharArray(char_arrayUri, Uri_len);

        server.serveStatic(char_arrayUri, SPIFFS, char_array);

        Serial.print("load: ");
        Serial.print(char_array);
        Serial.print("url:");
        Serial.println(char_arrayUri);
    }
    server.on("/setTime", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                  Serial.print(request->args());
                  //                  String year = request->arg("year");
                  //                  String month = request->arg("month");
                  //                  String day = request->arg("day");
                  //                  String hour = request->arg("hour");
                  //                  String minute = request->arg("minute");
                  //                  String second = request->arg("second");
                  //                  if (year && month && day && hour && minute && second)
                  //                      setTime((int)year, (int)month, (int)day, (int)hour, (int)minute, (int)second);
                  //                  else
                  //                      return request->send(400, "text/plane", "格式錯誤");
                  String req = "{";
                  //                  req += "\"Time\":\"" + TimeString("Time") + "\",";
                  //                  req += "\"Date\":\"" + TimeString("Date") + "\"";
                  req += "}";
                  request->send(200, "text/plane", req);
              });
    server.on("/setWifi", HTTP_POST, [](AsyncWebServerRequest *request) {});
    server.begin();
}
String TimeString(String Type)
{
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
    return (String)(Type == "Date" ? Date : Time);
}
temperature loopDHT(DHT _dht)
{
    temperature _Data(_dht.readHumidity(),
                      _dht.readTemperature(),
                      _dht.readTemperature(true));

    display.clearDisplay();
    display.display();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(25, 0);
    display.setTextSize(1.5);
    display.println(TimeString(String("Date")));
    display.setTextSize(1);
    display.println("---------------------");
    display.setCursor(15, 20);
    display.setTextSize(2);
    display.println(TimeString(String("Time")));
    if (!_Data.error)
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
        oldT = _Data.getT();
        String data = "{";
        data += "\"Time\":\"" + String(TimeString("Time")) + "\",";
        data += "\"Date\":\"" + String(TimeString("Date")) + "\",";
        data += "\"H\":\"" + (String)_Data.getH() + "\",";
        data += "\"C\":\"" + (String)_Data.getT() + "\",";
        data += "\"F\":\"" + (String)_Data.getF() + "\"";
        data += "}";
        ws.textAll(data);
    }
    display.setCursor(28, 40);
    display.setTextSize(3);
    display.print(oldT, 1);
    display.print((char)247);
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

void setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    // YYYY, MM, DD, HH, II, SS
    Rtc.SetDateTime(RtcDateTime(year, month, day, hour, minute, second));
}

void upDataWifiConfig()
{
    File file = SPIFFS.open("/setting/wifi.txt", "r");
    String wifiConfig;
    while (file.available())
        wifiConfig = file.readString();
    wifiName = wifiConfig.substring(wifiConfig.indexOf("name:") + 5, wifiConfig.indexOf(";"));
    wifiPassword = wifiConfig.substring(wifiConfig.indexOf("password:") + 9, wifiConfig.indexOf(";"));
    Serial.print(wifiName);
    Serial.print(wifiPassword);
    file.close();
}

void setWifiType()
{
    if (wifiName.length() > 0 || (wifiName.length() > 0 && wifiPassword.length() > 0))
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifiName, wifiPassword);
    }
    else
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(IP, IP, IPAddress(255, 255, 255, 0));
        WiFi.softAP(AP_ssid, password);
    }
    display.clearDisplay();
    display.display();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("mode:");
    display.println(WiFi.getMode());
    display.print("IP:");
    display.println(WiFi.softAPIP() || WiFi.localIP());
    display.display();
    delay(s * 5);
}
