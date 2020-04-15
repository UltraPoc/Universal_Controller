#include <LiquidCrystal_I2C.h>
#include <PinChangeInt.h>

volatile int intr = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void interruptFunction( void )
{
  intr++;
}

void setup() {
   pinMode(10, INPUT_PULLUP);
   attachPinChangeInterrupt(10, interruptFunction, FALLING);
   lcd.init();
   lcd.backlight();
   lcd.setCursor(0, 0);
   lcd.print("Made by aleksander");
   delay(1500);
   lcd.clear();
}

void loop() {
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print(intr);
  //Serial.println(intr);
}
