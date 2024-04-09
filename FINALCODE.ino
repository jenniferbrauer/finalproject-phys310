#include <Wire.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#include "SparkFun_SCD30_Arduino_Library.h"

#define sensorPin A1
const int dry = 380;
const int wet = 277;
int RelayPin = 13;

SCD30 airSensor;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
float calibration_value = 32.26;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float volt, ph_act;

#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

unsigned long wateringStartTime = 0;
const unsigned long wateringDuration = 5000;
const unsigned long wateringInterval = 60000;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Calibrating...");
  pinMode(RelayPin, OUTPUT);
  dht.begin();
  Wire.begin();
  if (!airSensor.begin()) {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("SCD30 Example");
  Serial.println("Temp out(C), Temp in(C), CO2 (ppm), pH, Humidity(%), moisture(%)");
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  int percentageHumidity = map(sensorVal, wet, dry, 100, 0);

  if (sensorVal > 400) {
    if (millis() - wateringStartTime >= wateringInterval || wateringStartTime == 0) {
      digitalWrite(RelayPin, HIGH);
      wateringStartTime = millis();
    }
    if (millis() - wateringStartTime >= wateringDuration) {
      digitalWrite(RelayPin, LOW);
    }
  } else {
    digitalWrite(RelayPin, LOW);
  }

  Serial.print("Analog output: ");
  Serial.println(sensorVal);
  if (sensorVal < wet) {
    Serial.println("Status: Soil is too wet - stop watering!");
  } else if (sensorVal >= wet && sensorVal < dry) {
    Serial.println("Status: Soil moisture is perfect");
  } else {
    Serial.println("Status: Soil is too dry - time to water!");
  }

  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++) {
    avgval += buffer_arr[i];
    volt = (float)avgval * 5.0 / 1024 / 6;
    ph_act = -6 * volt + calibration_value;
  }
  Serial.print(volt);
  Serial.print(" , ");
  Serial.println(ph_act);

  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Failed to read DHT11!"));
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print(F("t(out):"));
  lcd.print(t);
  Serial.print(t);
  Serial.print(", ");

  if (airSensor.dataAvailable()) {
    float tin = airSensor.getTemperature();
    int co2 = airSensor.getCO2();
    float h = airSensor.getHumidity();

    lcd.print(" t(in):");
    lcd.print(tin, 1);
    Serial.print(tin, 1);

    lcd.print(" pH:");
    lcd.print(ph_act);

    lcd.setCursor(0, 1);
    lcd.print("humd(%):");
    lcd.print(h, 1);
    Serial.println(", ");
    Serial.print(h, 1);
    Serial.println("%");

    lcd.print(" mst(%):");
    lcd.print(percentageHumidity);
    Serial.println();
    Serial.print(percentageHumidity);
    Serial.println("%");

    lcd.setCursor(24, 1);
    lcd.print(" co2:");
    lcd.print(co2);
    Serial.print(co2);
    Serial.print(", ");

    Serial.println(millis());

    delay(1200);
    for (int positionCounter = 0; positionCounter < 18; positionCounter++) {
      lcd.scrollDisplayLeft();
      delay(300);
    }
  } else {
    Serial.println("Waiting for new data");
  }

  delay(500);
  lcd.clear();
}
