#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- LCD I2C ----------
LiquidCrystal_I2C lcd(0x27,16,2);

// ---------- Capteurs / sorties ----------
const int pinTMP = A0;
const int pinSoil = A1;
const int pinLED = 8;
const int pinButton = 2;

// ---------- 74HC595 SPI-like ----------
#define DATA_PIN 11
#define CLOCK_PIN 13
#define LATCH_PIN 10

// ---------- Variables ----------
float tempThreshold = 30.0;     // seuil alerte
int mode = 0;                    // 0=temp,1=soil,2=mix
unsigned long ledBlinkMillis = 0;
bool ledState = false;
unsigned long lastDisplayMillis = 0;
unsigned long buttonPressTime = 0;
bool inConfigMode = false;
int lastButtonState = HIGH;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);

  digitalWrite(pinLED, LOW);

  lcd.setCursor(0,0);
  lcd.print("Systeme Pret");
  delay(1000);
  lcd.clear();
}

void loop() {
  // ---------- Lecture capteurs ----------
  int rawTMP = analogRead(pinTMP);
  float voltage = rawTMP * 5.0 / 1023.0;
  float tempC = (voltage - 0.5) * 100.0;

  int soilRaw = analogRead(pinSoil);

  // ---------- Barre LED 74HC595 ----------
  int ledsOn = map(tempC, 0, 60, 0, 8);
  ledsOn = constrain(ledsOn,0,8);

  byte pattern = 0;
  for(int i=0;i<ledsOn;i++){
    pattern |= (1<<i);
  }
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
  digitalWrite(LATCH_PIN, HIGH);

  // ---------- LED rouge alerte ----------
  if(tempC > tempThreshold){
    if(millis() - ledBlinkMillis >= 200){
      ledBlinkMillis = millis();
      ledState = !ledState;
      digitalWrite(pinLED, ledState ? HIGH : LOW);
    }
  } else {
    digitalWrite(pinLED, LOW);
    ledState = false;
  }

  // ---------- Lecture bouton poussoir ----------
  int buttonState = digitalRead(pinButton);

  // Détection appui court / long
  if(buttonState == LOW && lastButtonState == HIGH){
    buttonPressTime = millis(); // début appui
  }
  else if(buttonState == HIGH && lastButtonState == LOW){
    unsigned long pressDuration = millis() - buttonPressTime;
    if(pressDuration >= 2000){ // appui long → configuration seuil
      inConfigMode = !inConfigMode;
      lcd.clear();
    } else { // appui court → changement de mode si pas en config
      if(!inConfigMode){
        mode++;
        if(mode > 2) mode = 0;
        lcd.clear();
      } else { // en config, chaque appui court augmente le seuil
        tempThreshold += 1.0;
        if(tempThreshold > 150) tempThreshold = 150;
      }
    }
  }
  lastButtonState = buttonState;

  // ---------- Affichage LCD ----------
  if(millis() - lastDisplayMillis >= 200){
    lastDisplayMillis = millis();
    lcd.clear();
    if(inConfigMode){
      lcd.setCursor(0,0);
      lcd.print("Mode config seuil");
      lcd.setCursor(0,1);
      lcd.print("Seuil=");
      lcd.print(tempThreshold,0);
      lcd.print(" C");
    } else {
      switch(mode){
        case 0:
          lcd.setCursor(0,0);
          lcd.print("Temp: ");
          lcd.print(tempC,1);
          lcd.print((char)223);
          lcd.print("C");
          lcd.setCursor(0,1);
          lcd.print("Volt:");
          lcd.print(voltage,2);
          lcd.print(" V");
          break;
        case 1:
          lcd.setCursor(0,0);
          lcd.print("Humidite Sol");
          lcd.setCursor(0,1);
          lcd.print(soilRaw);
          lcd.print(" /1023");
          break;
        case 2:
          lcd.setCursor(0,0);
          lcd.print("T:");
          lcd.print(tempC,1);
          lcd.print((char)223);
          lcd.print("C Thr:");
          lcd.print(tempThreshold,0);
          lcd.setCursor(0,1);
          lcd.print("H:");
          lcd.print(soilRaw);
          break;
      }
    }
  }
}
