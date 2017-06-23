/**
 * NodeMCU + SHT75 = Meteostation
 *
 * made by Petus
 * http://petus.cz
 * http://time4ee.com
 *
 * SHT1x library - www.practicalarduino.com
 */

#include <SHT1x.h>
#include <ESP8266WiFi.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  D3
#define clockPin D4

#define SLEEP_DELAY_IN_SECONDS  300


SHT1x sht1x(dataPin, clockPin);

const char* ssid     = "SSID";
const char* password = "PASSWORD";

const char* host = "petus.tmep.cz";

void setup()
{
   //pinMode(LED_BUILTIN, OUTPUT); DON'T USE RED LED on NodeMCU when you want to use Deep Sleep mode
   Serial.begin(115200); // Open serial connection to report values to host
   delay(10);
   
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();

  // Print the values to the serial port
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C");

  Serial.print("F. Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/index.php?";

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + "tempC=" + temp_c + "&humV=" + humidity + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond:");
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  
  //digitalWrite(LED_BUILTIN, LOW); DON'T USE RED LED on NodeMCU when you want to use Deep Sleep mode
  //delay(2000);
  //digitalWrite(LED_BUILTIN, HIGH); DON'T USE RED LED on NodeMCU when you want to use Deep Sleep mode
  ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
  delay(500);

}

