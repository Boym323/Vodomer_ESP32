// Knihovny pro praci s Wi-Fi a UDP/multicast
#include <WiFi.h>
#include <WiFiUdp.h>

// Knihovna pro nastaveni pull-down rezistoru na RTCIO pinu,
// ktery bude fungovat i behem hlubokeho spanku
#include <driver/rtc_io.h>

// Pomocna makra s pristupovymi udaji k Wi-Fi
#define SSID "ZiveCzWiFi"
#define PASWD "nevolimburese"

// Promenna s pocitadlem otevrenych dveri, ktera se vytvori v RTC casti RAM
RTC_DATA_ATTR size_t pocitadlo_otevreni = 0;

// Hlavni funkce setup se spusti hned po startu/restartu
void setup() {
  // Nastaveni pinu GPIO27 na vstup (jazyckovy kontakt)
  pinMode(GPIO_NUM_27, INPUT);
  
  // Nastaveni interniho pull-down rezistoru na pinu GPIO27
  rtc_gpio_pulldown_en(GPIO_NUM_27);

  // Nastartovani seriove linky pro potreby ladeni
  Serial.begin(115200);
  Serial.println("Start");

  // Reset Wi-Fi
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);

  // Zjisti duvod probuzeni z hlubokeho spanku
  esp_sleep_wakeup_cause_t duvodProbuzeni = esp_sleep_get_wakeup_cause();

  // Pokud je duvodem probuzeni preruseni typu EXT0,
  // probuzeni zpusobil jazykovy kontakt a nekdo tedy otevrel dvere
  if (duvodProbuzeni == ESP_SLEEP_WAKEUP_EXT0) {

    // Zvys pocitadlo otevreni dveri
    pocitadlo_otevreni++;


    // Nyni se pripoj k Wi-Fi
    Serial.print("Pripojuji se k Wi-Fi ");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASWD);
    // Dokud se nepripojim, vypisuj kazdych 500 ms do seriove linky tecku
    // Spravne bych mel implementovat jeste nejaky timeout,
    // jinak se budou v pripade chyby tecky vypisovat az do konce veku
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print('.');
    }

    // Jsem pripojeny k Wi-Fi
    Serial.println(" OK");


  }

  // Pokud byl oduvodem probuzeni neco jineho,
  // zobraz generickou zdravici. Zobrazi se treba po stisku tlacitka reset,
  // po pripojeni k napeti, po flashi pnoveho programu atp.
  else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    Serial.println("Cip probuzeny jinym zpusobem");
    tft.drawString("Chytre dvere 1.0",  tft.width() / 2, tft.height() / 2 );
    delay(5000);
  }

  // Pokud jsou dvere nadale otevrene,
  // GPIO27 by mel vracet logickou nulu.
  // Ve smyce proto cekam, dokud tento pin nevrati logickou jednicku,
  // pricemz po kazde kontrole pockam sekundu, cimz primitivnim zpusobem vyresim
  // pripadny bouncing (kmitani logicke hodnoty pri zavirani dveri)
  Serial.print("Cekam na zavreni dveri ");
  while (!digitalRead(GPIO_NUM_27)) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println(" OK");

  // Vse je hotovo, a tak jdu zase spat
  // Vyprazdni zasobnik seriove linky, pockej jeste sekundu a usni
  Serial.println("Jdu spat");
  Serial.flush();
  delay(1000);

  // Nastav znovu externi budicek typu EXT0 na pinu GPIO27
  // v pripade, ze se na nem objevi logicka nula
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 0);

  // Prejdi do rezimu hlubokeho spanku
  // Po tomto prikazu se nic dalsiho v programu nezpracuje,
  // coz plati i o smycce loop, ktera se spousti az po zpracovani funkce setup
  esp_deep_sleep_start();
}


// Smycka loop je prazdna, protoze k jejim uspusteni nikdy nedojde
// Cip o krok vyse prejsel do hlubokeho spanku
void loop() {
}