#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WifiConfig.h>
#include <espnbns.h>

ESP8266WebServer wwwserver(80); // webovy server - pouzity je jak pro nastaveni ESP modulu pomoci WifiConfig, tak pro praci aplikace
String content;
WifiConfig wifi; // konfigurace ESP modulu (xPablo.cz)
NBNS nbns; // Netbios (xPablo.cz)

static void handleRoot(void)
{

	content = F("<!DOCTYPE HTML>\n<html>Hello world from ESP8266");
	content += F("<p>");
	content += F("</html>");

	wwwserver.send(200, F("text/html"), content);
}

void setup()
{

	EEPROM.begin(512); // zahajujeme praci s EEPROM
	pinMode(3, INPUT_PULLUP); // RXD slouzi jako vstup tlacitka
	wifi.begin(0, digitalRead(3)); // startujeme pripojeni

    wwwserver.on("/", handleRoot);
	wwwserver.begin(); // startujeme webovy server

	Serial.begin(115200);

	nbns.begin("ESP");
}

void loop()
{
	wwwserver.handleClient(); // osetrujeme praci serveru
	nbns.poll();
}
// EOF
