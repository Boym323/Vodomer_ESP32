//#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "SWS_free";
const char *password = "password";
const char *mqttServer = "5.104.16.71";
const int mqttPort = 1883;
const char *mqttUser = "mqtt username";
const char *mqttPassword = "mqtt password";
WiFiClient espClient;
PubSubClient client(espClient);

#define PIN_napajeni_optosenzoru 12
#define PIN_SV 27

RTC_DATA_ATTR size_t PocetProbuzeni = 0;

RTC_DATA_ATTR size_t SpotrebaSV = 0;
RTC_DATA_ATTR size_t StavSV;
RTC_DATA_ATTR size_t posledniStavSV;

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_napajeni_optosenzoru, OUTPUT);
  pinMode(PIN_SV, INPUT_PULLUP);
}

void OdecetVody()
{
  digitalWrite(PIN_napajeni_optosenzoru, HIGH);
  // přečtěte vstupní pin tlačítka:
  StavSV = digitalRead(PIN_SV);

  // porovnejte buttonState (stav tlačítka) s předchozím stavem
  if (StavSV != posledniStavSV)
  {
    // jestliže se stav změnil, navyšte hodnotu počítadla
    if (StavSV == LOW)
    {
      // jestliže je současný stav HIGH, tlačítko přešlo
      //z off na on:
      SpotrebaSV++;
      Serial.println("impuls");
      Serial.print("Pocet litrů od startu:  ");
      Serial.println(SpotrebaSV);
    }
    else
    {
      // jestliže je současný stav LOW, tlačítko přešlo
      // z on na off:
      Serial.println("Není ");
    }
  }
  // uložte současný stav jako „poslední stav“,
  //abyste ho mohli v příští smyčce použít
  posledniStavSV = StavSV;
  digitalWrite(PIN_napajeni_optosenzoru, LOW);
}

void wifi()
{
  WiFi.begin(ssid); //  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  client.setServer(mqttServer, mqttPort);

  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {

      Serial.println("connected");
    }
    else
    {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  int waitmqqt = 50;
  client.publish("domov/Hello", "hello world");
  delay(waitmqqt);
  client.publish("domov/Cislo30", String(30).c_str(), true);
  delay(waitmqqt);
  client.publish("domov/PocetProbuzeni", String(PocetProbuzeni).c_str(), true);
  delay(waitmqqt);
  client.publish("domov/SpotrebaSV", String(SpotrebaSV).c_str(), true);
  delay(waitmqqt);
  client.publish("domov/SpotrebaTV", String(WiFi.RSSI()).c_str(), true);
}

void loop()
{
  PocetProbuzeni++;
  OdecetVody();
  Serial.print(PocetProbuzeni);
  if (PocetProbuzeni >= 10 && SpotrebaSV > 0)
  {
    wifi();
    PocetProbuzeni = 0;
    SpotrebaSV = 0;
  }

  esp_sleep_enable_timer_wakeup(1000000);
  esp_deep_sleep_start();
}