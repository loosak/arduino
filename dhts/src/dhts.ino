/* WeMos DHT Server
 *
 * Connect to WiFi and respond to http requests with temperature and humidity
 *
 * Based on Adafruit ESP8266 Temperature / Humidity Webserver
 * https://learn.adafruit.com/esp8266-temperature-slash-humidity-webserver
 *
 * Depends on Adafruit DHT Arduino library
 * https://github.com/adafruit/DHT-sensor-library
 */
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
//#include "../lib/DHT/DHT.h"
#include <DHT.h>
#include "../lib/interval/interval.h"
#include "nbnsota.h"
#include "embHTML.h"

#define DHTTYPE DHT22    // DHT Shield uses DHT22
#define DHTPIN  D4       // DHT Shield uses pin D4 (GPIO2)

ESP8266WebServer server(80);

DHT dht(DHTPIN, DHTTYPE);

float humidity, temperature;                  // Surove hodnoty z cidla
char str_humidity[10], str_temperature[10];   // Hodnoty pro vystup ve forme retezcu
Interval senstiming;                          // casovaci interval pro omezeni rychlosti vycitani cidla

void handle_root()
{
  server.send(200, "text/html", FPSTR(PAGE_INDEX));
}

void read_sensor()
{

  if (senstiming.expired())
  {
    senstiming.set(2000);
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    dtostrf(humidity, 1, 2, str_humidity);
    dtostrf(temperature, 1, 2, str_temperature);

    Serial.printf("Humidity: %s %\t Temperature: %s °C\n", str_humidity, str_temperature);
  }
}

void handle_data()
{
  String res;

  read_sensor();
  res = "{\"temp\":\"";
  res += str_temperature;
  res += "\",\"hum\":\"";
  res += str_humidity;
  res += "\"}";
  server.send(200, "application/json", res);
}

void setup(void)
{

  Serial.begin(115200);
  Serial.println("\r\nProgram started...");

  EEPROM.begin(1024);

  nbnsotasetup(NULL);

  Serial.printf("Wi-Fi mode set to WIFI_STA %s\n", WiFi.mode(WIFI_STA) ? "" : "Failed!");

  Serial.printf("Our IP: "); Serial.println(getOurIP());

  dht.begin();

  read_sensor();

  // Obsluzne handlery
  server.on("/", handle_root);

  server.on("/temp", [](){
    read_sensor();
    char response[50];
    snprintf(response, sizeof(response), "Temperature: %s °C", str_temperature);
    server.send(200, "text/plain", response);
  });

  server.on("/humidity", [](){
    read_sensor();
    char response[50];
    snprintf(response, sizeof(response), "Humidity: %s %", str_humidity);
    server.send(200, "text/plain", response);
  });

  server.on("/data", handle_data);

  // Spustime server
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
  nbnsotaloop();
}
// EOF
