#include <avr/eeprom.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "GyverEncoder.h"

#define CLK 2
#define DT 4
#define SW 6
#define BUZ 5
#define ONE_WIRE_BUS 10
#define LED 9
#define DELTA 60      //в минутах  (60)
#define UPDATING 4 //в секуднах   обновление экрана (4)
#define DELTA2 24 //в секундах   откат энкодера  (20)
#define REBOOT 30  //время до перезагрузки в днях
#define ENCDEL 1500 //в миллисекундах
#define BOOP 5 //пищалка в секундах
#define INTERR 30 //перезагрузка прерывания в минутах

OneWire oneWire(ONE_WIRE_BUS);
Encoder enc1(CLK, DT, SW);
LiquidCrystal_I2C lcd(0x27, 20, 4); //16
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

float temp1[4], temp2[4];   //сначала время, месяц и день потом год
volatile boolean triger = 0, beginning = 0, encFlag = 0;
byte symb0[8] = {B01010, B10101, B10001, B10011, B10101, B11001, B10001, B00000}; //Й
byte symb1[8] = {B10001, B10001, B10011, B10101, B11001, B10001, B10001, B00000}; //И
byte symb2[8] = {B10101, B10101, B10101, B10101, B10101, B11111, B00001, B00001}; //Щ
byte symb3[8] = {B11111, B10001, B11111, B00011, B00101, B01001, B10001, B00000}; //Я
byte symb4[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B10001, B00000}; //П
unsigned long time1, time2, time3, time4;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();// Включаем подсветку дисплея
  lcd.setCursor(0, 0); //сначала место на строке, потом ряд
  lcd.print("Made by");
  lcd.setCursor(0, 1);
  lcd.print("Aleksandr Orlov");
  lcd.setCursor(0, 2);
  lcd.print("Loading...");
  lcd.setCursor(0, 3);
  lcd.print("V1.0");
  sensors.setResolution(insideThermometer, 12);
  sensors.getAddress(insideThermometer, 0);
  pinMode(3, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  attachInterrupt(1, buttonTick, FALLING);
  attachInterrupt(0, isr, CHANGE);
  lcd.createChar(0, symb0);
  lcd.createChar(1, symb1);
  lcd.createChar(2, symb2);
  lcd.createChar(3, symb3);
  lcd.createChar(4, symb4);
  time.begin();
  enc1.setType(TYPE2);
  delay(5); //1600 200
  time1 = millis();
  time2 = millis();
  time4 = millis();
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEKY");
  lcd.write(2);
  lcd.print("A");
  lcd.write(3);
  lcd.print(" TEM");
  lcd.write(4);
  lcd.print("EPATYPA");
  lcd.setCursor(0, 1);
  lcd.print(time.gettime("d-m-Y H:i"));
  lcd.setCursor(0, 2);
  lcd.print("TEM");
  lcd.write(4);
  lcd.print(":");
  temp[0] = getTemp();
  temp[0] = getTemp();
  temp[0] = getTemp();
  temp[0] = getTemp();
  delay(1000);
  temp[0] = getTemp();
  temp[1] = getTemp();
  temp[2] = getTemp();
  temp[3] = getTemp();
  lTemp = quadVal(temp[0], temp[1], temp[2], temp[3]);
}

void loop() {
  if (millis() - time4 >= INTERR * 60000)
  {
    Interrupts();
  }
  if (triger == 1)      //срабатывание кнопки настройки
  {
    pushSound();
    Settings();
    triger = 0;
  }
}
