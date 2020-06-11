#include <Arduino.h>

#define PIN_napajeni_optosenzoru 3
#define PIN_SV 13

int litry = 0;
boolean StavSV;
boolean posledniStavSV;

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_napajeni_optosenzoru, OUTPUT);
  pinMode(PIN_SV, INPUT);
}

void OptoPower()
{
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
      Serial.println("on");
      Serial.print("number of button pushes:  ");
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
  
}

void loop()
{
  OptoPower();
}