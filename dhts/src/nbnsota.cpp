/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/
#include <Arduino.h>
#include "configuration.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include "../lib/espnbns/espnbns.h"
#include "nbnsota.h"

NBNS nbns; // Netbios (xPablo.cz)

void ICACHE_FLASH_ATTR nbnsotasetup(wificonfig_cb wificfgcb)
{
  WiFiConfig wifi; // konfigurace ESP modulu (xPablo.cz)

  pinMode(FORCE_CONFIG_BUTTON_PIN, INPUT_PULLUP);
  delay(10);
  wifi.begin(EEPROM_WIFICONFIG_ORIGIN, digitalRead(FORCE_CONFIG_BUTTON_PIN), wificfgcb); // startujeme pripojeni

  if (strlen(WiFiDeviceName))
  {
    nbns.begin(WiFiDeviceName);
    ArduinoOTA.setHostname(WiFiDeviceName);
  }

  ArduinoOTA.begin();
}

void nbnsotaloop()
{

  nbns.poll();
  ArduinoOTA.handle();
}
// EOF
