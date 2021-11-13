/** 
 * use LoLin
 * pins:
 * * dht11.data(室內) -> D0
 * * dht11.data(室外) -> D1
 */

#include "main.h"
#include <FS.h>
#include <DHT.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

int checkBtnTime = 0;
Adafruit_SSD1306 display(-1);
AsyncWebServer server(80);
DHT dht(D0, DHT11);  // 室內
DHT dht2(D1, DHT11); // 室外

void webInit();
_Data loopDHT(DHT _dht);

void setup()
{
    SPIFFS.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
    display.clearDisplay();
    Serial.begin(9600);
    dht.begin();
    dht2.begin();
    webInit();
}

void loop()
{
    loopDHT(dht);
    loopDHT(dht2);
    delay(1e3); //延時5秒
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
