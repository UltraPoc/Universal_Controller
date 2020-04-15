#include <avr/eeprom.h>
#include <LiquidCrystal_I2C.h>
#include <PinChangeInt.h>
#include "GyverEncoder.h"

#define CLK 2
#define DT 4
#define SW 6
#define OUT1 13  //выходы под соответственными номерами 1-4
#define OUT2 12
#define OUT3 8
#define OUT4 7
#define IN1 10 //вперед
#define IN2 11 //назад
#define IN3 3 //стоп
#define IN4 9 //старт (авто)
#define IN5 5 //выгрузка

Encoder enc1(CLK, DT, SW);
LiquidCrystal_I2C lcd(0x27, 20, 4); //16

byte symb0[8] = {B01010, B10101, B10001, B10011, B10101, B11001, B10001, B00000}; //Й
byte symb1[8] = {B10001, B10001, B10011, B10101, B11001, B10001, B10001, B00000}; //И
byte symb2[8] = {0b10001, 0b10001, 0b10001, 0b11101, 0b10101, 0b10101, 0b11101, 0b00000}; //Ы
byte symb3[8] = {0b11111, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b00000}; //Г
byte symb4[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B10001, B00000}; //П
byte symb5[8] = {0b01111, 0b00101, 0b00101, 0b01001, 0b10001, 0b11111, 0b10001, 0b00000}; //Д
byte symb6[8] = {0b10101, 0b10101, 0b10101, 0b10101, 0b10101, 0b10101, 0b11111, 0b00000}; //Ш
byte symb7[8] = {0b10101, 0b10101, 0b01110, 0b00100, 0b01110, 0b10101, 0b10101, 0b00000}; //Ж
float t1 = eeprom_read_float(0), t2 = eeprom_read_float(4), t3 = eeprom_read_float(8);
volatile uint32_t debounce;
volatile boolean enc_trig = 0, IN1_trig = 0, IN2_trig = 0, IN3_trig = 0, IN4_trig = 0, IN5_trig = 0;

void setup() {
  digitalWrite(OUT1, LOW); //доп защита
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
  lcd.createChar(0, symb0);
  lcd.createChar(1, symb1);
  lcd.createChar(2, symb2);
  lcd.createChar(3, symb3);
  lcd.createChar(4, symb4);
  lcd.createChar(5, symb5);
  lcd.createChar(6, symb6);
  lcd.createChar(7, symb7);
  enc1.setType(TYPE2);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(IN1, INPUT_PULLUP);
  pinMode(IN2, INPUT_PULLUP);
  pinMode(IN3, INPUT_PULLUP);
  pinMode(IN4, INPUT_PULLUP);
  pinMode(IN5, INPUT_PULLUP);
  attachInterrupt(1, IN3_interrupt, FALLING);
  activateInterrupts();
  delay(2000);
  stopAll();
}


void loop() {
  if (enc_trig) //вызов настроек (если нужно будет переделать, то можно опрашивать энкодер без прерывания, прямо тут)
  {
    settings(&t1, &t2, &t3);
    enc_trig = 0;
    trigToZero();
  } 
  if (IN1_trig) //вызов команды "вперед"
  {
    forwardMode(t1);
    enc_trig = 0;
    trigToZero();
  }
  if (IN2_trig) //вызов команды "назад"
  {
    backMode(t1);
    enc_trig = 0;
    trigToZero();
  }
  if (IN3_trig) //вызов команды "стоп"
  {
    stopAll();
    IN3_trig = 0;
    enc_trig = 0;
    trigToZero();
  }
  if (IN4_trig) //вызов команды "авто"
  {
    autoMode(t1, t2, t3);
    enc_trig = 0;
    trigToZero();
  }
  if (IN5_trig) //вызов команды "выгрузка"
  {
    unloadingMode(t1);
    enc_trig = 0;
    trigToZero();
  }  
}
