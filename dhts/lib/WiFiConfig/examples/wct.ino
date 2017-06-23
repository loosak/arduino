#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <espnbns.h>
#include "libraries/WiFiConfig/WiFiConfig.h"

// Plati pro desticku Witty s modulem ESP-12E
#define PIN_FORCE_CONFIG 4
#define PIN_LED_RED 15
#define PIN_LED_GREEN 12
#define PIN_LED_BLUE 13

//#define DEBUG_OUT(a) {}
#define DEBUG_OUT(a) Serial.print(a)

ESP8266WebServer wwwserver(80); // webovy server
String content;
NBNS nbns; // Netbios (xPablo.cz)

static void handleRoot(void)
{

  content = F("<!DOCTYPE HTML>\n<html>Hello world from ESP8266");
  content += F("<p>");
  content += F("</html>");

  wwwserver.send(200, F("text/html"), content);
}

void ICACHE_FLASH_ATTR wcb(wificonfigstate_t state)
{

  switch (state)
  {
    case WCS_CONNECTSTART:
      DEBUG_OUT(F("Starting connect...\r\n"));
      break;

    case WCS_CONNECTING:
      break;

    case WCS_CONNECTED:
      DEBUG_OUT(F("Connected.\r\n"));
      break;
      
    case WCS_CONFIGSTART:
      DEBUG_OUT(F("Starting config...\r\n"));
      break;

    case WCS_CONFIGWAIT:
      break;
  }
}

void saveServer(const char *param)
{

  DEBUG_OUT(param);
}

void ICACHE_FLASH_ATTR setup()
{
  WiFiConfig wifi; // konfigurace ESP modulu
  WiFiConfigUsrParameter testOnly("server", "adresa serveru", "Default server", 32, saveServer);

  EEPROM.begin(512); // zahajujeme praci s EEPROM
  pinMode(PIN_FORCE_CONFIG, INPUT_PULLUP); // pin, co slouzi jako vstup tlacitka
  int fc = digitalRead(PIN_FORCE_CONFIG); // pozadavek na vynucene vyvolani konfigurace
  Serial.begin(9600);
  wifi.addParameter(&testOnly);
  wifi.begin(0, fc, wcb); // startujeme pripojeni


  wwwserver.on("/", handleRoot);
  wwwserver.begin(); // startujeme webovy server

  if (strlen(WiFiDeviceName) > 0)
  {
    nbns.begin(WiFiDeviceName);
    ArduinoOTA.setHostname(WiFiDeviceName);
  }

  ArduinoOTA.onStart([]() {
    DEBUG_OUT(F("Start\r\n"));
  });
  ArduinoOTA.onEnd([]() {
    DEBUG_OUT(F("End\r\n"));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DEBUG_OUT(F("Error["));
    DEBUG_OUT(error);
    DEBUG_OUT(F("]: "));
    switch (error)
    {
      case OTA_AUTH_ERROR:
        DEBUG_OUT(F("Auth Failed\r\n"));
        break;

      case OTA_BEGIN_ERROR:
        DEBUG_OUT(F("Begin Failed\r\n"));
        break;

      case OTA_CONNECT_ERROR:
        DEBUG_OUT(F("Connect Failed\r\n"));
        break;

      case OTA_RECEIVE_ERROR:
        DEBUG_OUT(F("Receive Failed\r\n"));
        break;

      case OTA_END_ERROR:
        DEBUG_OUT(F("End Failed\r\n"));
        break;

      default:
        DEBUG_OUT(F("\r\n"));
    }
  });
  ArduinoOTA.begin();

}

void loop()
{
  wwwserver.handleClient(); // osetrujeme praci serveru
  ArduinoOTA.handle();
  nbns.poll();
}
