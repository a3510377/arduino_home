/** 
 * use LoLin
 * pins:
 * * button -> d0
 * * oled.SCK -> D1
 * * oled.SDA -> D2
 * * dht11.DATA(室內) -> D5
 * * dht11.DATA(室外) -> D6
 * * 
 */

#include "main.h"
#include <FS.h>
#include <DHT.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

static const uint8_t dhtType = DHT11; // 室內室外 dht 型號
const bool debug = false;             // 是否開啟 Debug 模式

/* btn Low time */
int checkBtnTime = 0;
/** web */
AsyncWebServer server(80);
/** ws */
AsyncWebSocket ws("/ws");
/** OLED */
// Adafruit_SSD1306 display(-1);
ThreeWire myWire(D6, D5, D3);
/** Clock */
RtcDS1302<ThreeWire> Rtc(myWire); // Clock
/** 室內 */
DHT dht(D5, dhtType);
/** 室外 */
DHT dht2(D6, dhtType);

/* 定義函數 */
void webInit();
void callBtn(int date);
temperature loopDHT(DHT _dht);

void setup()
{
    /* DHT setup */
    dht.begin();
    dht2.begin();

    Rtc.Begin();    // time setup
    SPIFFS.begin(); // load file setup
    // display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // OLED setup
    // display.clearDisplay();                    // OLED clear display
    Serial.begin(9600); // Serial setup
    webInit();          // web init

    if (build)
        Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));

    /* pin setup */
    pinMode(D0, INPUT);
}

/* 主程式延遲 .2s */
void loop()
{
    bool btnIs = digitalRead(D0);
    if (btnIs)
        checkBtnTime++;
    else if (checkBtnTime > 0)
    {
        callBtn(checkBtnTime);
        checkBtnTime = 0;
    }
    loopDHT(dht);
    loopDHT(dht2);
    delay(s * .2); //延時.2s
}

/** server init */
void webInit()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", R"rawliteral()rawliteral"); });
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
        int str_len = dir.fileName().length() + 1;
        char char_array[str_len];
        dir.fileName().toCharArray(char_array, str_len);
        server.serveStatic(char_array, SPIFFS, char_array);
        Serial.print("load: ");
        Serial.println(char_array);
    }
    server.begin();
}
temperature loopDHT(DHT _dht)
{
    temperature _Data = temperature(_dht.readHumidity(),
                                    _dht.readTemperature(),
                                    _dht.readTemperature(true));
    Serial.print("濕度: ");
    Serial.print(_Data.getH());
    Serial.print("%\t");
    Serial.print("攝氏溫度: ");
    Serial.print(_Data.getT());
    Serial.print("*C\t");
    Serial.print("華氏溫度: ");
    Serial.print(_Data.getF());
    Serial.print("*F\n");
    return _Data;
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