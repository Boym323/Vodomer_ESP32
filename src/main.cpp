#include <WiFi.h>
#include <PubSubClient.h>
// Knihovna pro nastaveni pull-down rezistoru na RTCIO pinu,
// ktery bude fungovat i behem hlubokeho spanku
#include <driver/rtc_io.h>

const char *ssid = "SWS_free";
const char *password = "password";
const char *mqttServer = "5.104.16.71";
const int mqttPort = 1883;
const char *mqttUser = "mqtt username";
const char *mqttPassword = "mqtt password";

WiFiClient espClient;
PubSubClient client(espClient);

RTC_DATA_ATTR size_t SportebaStudeneVody;
RTC_DATA_ATTR size_t SportebaTepleVody;

void setup()
{
  // Nastaveni pinu GPIO27 na vstup (jazyckovy kontakt)
  pinMode(GPIO_NUM_27, INPUT);

  // Nastaveni interniho pull-down rezistoru na pinu GPIO27
  rtc_gpio_pulldown_en(GPIO_NUM_27);
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
  client.publish("domov/PocetProbuzeni", String(waitmqqt).c_str(), true);
}

void loop()
{

  wifi();

  esp_sleep_enable_timer_wakeup(1000000);
  esp_deep_sleep_start();
}