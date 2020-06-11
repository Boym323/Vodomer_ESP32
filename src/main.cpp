#include <Arduino.h>

#define PIN_napajeni_optosenzoru 12
#define PIN_SV 13

int litry = 0;
boolean StavSV;
boolean posledniStavSV;

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_napajeni_optosenzoru, OUTPUT);
  pinMode(PIN_SV, INPUT);
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
      litry++;
      Serial.println("impuls");
      Serial.print("Pocet litrů od startu:  ");
      Serial.println(litry);
    }
    else
    {
      // jestliže je současný stav LOW, tlačítko přešlo
      // z on na off:
      Serial.println("off");
    }
  }
  // uložte současný stav jako „poslední stav“,
  //abyste ho mohli v příští smyčce použít
  posledniStavSV = StavSV;
  digitalWrite(PIN_napajeni_optosenzoru, LOW);
}

void loop()
{
OdecetVody();
ESP.deepSleep(5e6);
}