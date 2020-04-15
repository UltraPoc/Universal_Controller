void stopDelay( float t1 )
{
  unsigned long temp_timer = millis();
  
  while (millis() - temp_timer < (unsigned long)(t1 * 1000) && (!IN3_trig))
  {
    lcd.setCursor(0, 3); //ничего не делать :3
  }
}

void trigToZero( void )
{
  IN1_trig = 0; 
  IN2_trig = 0; 
  IN4_trig = 0; 
  IN5_trig = 0;
}

void stopInterrupts( void )
{
  detachPinChangeInterrupt(IN1);
  detachPinChangeInterrupt(IN2);
  detachPinChangeInterrupt(IN4);
  detachPinChangeInterrupt(IN5);
  detachInterrupt(0); 
}

void activateInterrupts( void )
{
  attachPinChangeInterrupt(IN1, IN1_interrupt, FALLING);
  attachPinChangeInterrupt(IN2, IN2_interrupt, FALLING);
  attachPinChangeInterrupt(IN4, IN4_interrupt, FALLING);
  attachPinChangeInterrupt(IN5, IN5_interrupt, FALLING);
  attachInterrupt(0, enc, CHANGE);
}

void settings( float *t1, float *t2, float *t3 )
{
  byte cnt = 1;
  float temp_t1 = *t1, temp_t2 = *t2, temp_t3 = *t3;

  stopInterrupts();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HACTPO");
  lcd.write(0);
  lcd.print("K");
  lcd.write(1);
  lcd.print(":");
  lcd.setCursor(0, 1);
  lcd.print("t1=");
  lcd.print(temp_t1);
  lcd.setCursor(8, 1);
  lcd.print("CEK");
  lcd.setCursor(0, 2);
  lcd.print("t2=");
  lcd.print(temp_t2);
  lcd.setCursor(10, 2);
  lcd.print("CEK");
  lcd.setCursor(0, 3);
  lcd.print("t3=");
  lcd.print(temp_t3);
  lcd.setCursor(10, 3);
  lcd.print("CEK");
  while (cnt < 4 && (!IN3_trig))
  {
    switch(cnt)
    {
      case 1:
        enc1.tick();
        lcd.setCursor(3, 1);
        lcd.blink();
        if (enc1.isRight())
        {
          if (temp_t1 + 0.5 > 10)
            temp_t1 = 0.5;
          else 
            temp_t1 += 0.5;
          lcd.setCursor(3, 1);
          lcd.print(temp_t1);
          lcd.setCursor(7, 1);
          lcd.print(" ");
        }
        if (enc1.isLeft())
        {
          if (temp_t1 - 0.5 < 0.5)
            temp_t1 = 10;
          else
            temp_t1 -= 0.5;
          lcd.setCursor(3, 1);
          lcd.print(temp_t1);
          lcd.setCursor(7, 1);
          lcd.print(" ");
        }
        if (enc1.isClick()){
          cnt++;
          lcd.noBlink();
        }
        break;
      case 2:
        enc1.tick();
        lcd.setCursor(3, 2);
        lcd.blink();
        if (enc1.isRight())
        {
          if (temp_t2 + 5 > 300)
            temp_t2 = 10;
          else 
            temp_t2 += 5;
          lcd.setCursor(3, 2);
          lcd.print(temp_t2);
        }
        if (enc1.isLeft())
        {
          if (temp_t2 - 5 < 10)
            temp_t2 = 300;
          else
            temp_t2 -= 5;
          lcd.setCursor(3, 2);
          lcd.print(temp_t2);
        }
        if (enc1.isClick()){
          cnt++;
          lcd.noBlink();
        }
        break;
      case 3:
        enc1.tick();
        lcd.setCursor(3, 3);
        lcd.blink();
        if (enc1.isRight())
        {
          if (temp_t3 + 10 > 600)
            temp_t3 = 10;
          else 
            temp_t3 += 10;
          lcd.setCursor(3, 3);
          lcd.print(temp_t3);
        }
        if (enc1.isLeft())
        {
          if (temp_t3 - 10 < 10)
            temp_t3 = 600;
          else
            temp_t3 -= 10;
          lcd.setCursor(3, 3);
          lcd.print(temp_t3);
        }
        if (enc1.isClick()){
          cnt++;
          lcd.noBlink();
        }
        break;
    }
  }
  if (!IN3_trig)
  {
    *t1 = temp_t1;
    *t2 = temp_t2;
    *t3 = temp_t3;
    eeprom_update_float(0, *t1);
    eeprom_update_float(4, *t2);
    eeprom_update_float(8, *t3);
  }
  lcd.noBlink();
  activateInterrupts();
  stopAll();
}

void stopAll(  void )
{
  lcd.clear();
  digitalWrite(OUT1, LOW);
  digitalWrite(OUT2, LOW);
  digitalWrite(OUT3, LOW);
  digitalWrite(OUT4, LOW);  
  lcd.setCursor(7, 1);
  lcd.write(3);
  lcd.print("OTOB!");
}

void IN1_interrupt( void )
{
  if (millis() - debounce >= 100)
  {
    debounce = millis();
    IN1_trig = 1;
  }
}

void IN2_interrupt( void )
{
  if (millis() - debounce >= 100)
  {
    debounce = millis();
    IN2_trig = 1;
  }
}

void IN3_interrupt( void )
{
  if (millis() - debounce >= 100)
  {
    debounce = millis();
    IN3_trig = 1;
  }
}

void IN4_interrupt( void )
{
  if (millis() - debounce >= 100)
  {
    debounce = millis();
    IN4_trig = 1;
  }
}

void IN5_interrupt( void )
{
  if (millis() - debounce >= 100)
  {
    debounce = millis();
    IN5_trig = 1;
  }
}

void enc( void )
{
  if (millis() - debounce >= 100)
  {
    debounce = millis();
    enc_trig = 1;
  }
}

void forwardMode( float t1 )
{
  stopInterrupts();
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("B");
  lcd.write(4);
  lcd.print("EPE");
  lcd.write(5);
  stopDelay(t1);   
  while (!IN3_trig)
  {
    digitalWrite(OUT1, HIGH);
    digitalWrite(OUT3, HIGH);
  }
  activateInterrupts();
}

void backMode( float t1 )
{
  stopInterrupts();
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print("HA3A");
  lcd.write(5);
  stopDelay(t1);
  while (!IN3_trig)
  {
    digitalWrite(OUT2, HIGH);
    digitalWrite(OUT4, HIGH);
  }
  activateInterrupts();
}

void unloadingMode( float t1 )
{
  stopInterrupts();
  lcd.clear();
  lcd.setCursor(6, 1);
  lcd.print("B");
  lcd.write(2);
  lcd.write(3);
  lcd.print("PY3KA");
  stopDelay(t1); 
  while (!IN3_trig)
  {
    digitalWrite(OUT1, HIGH);
    digitalWrite(OUT4, HIGH);
  }
  activateInterrupts();    
}

void cntDown( signed long cnt ) //инпут в секундах
{
  if (cnt > 1000) //защита от вывода херни в конце
    cntDown(0);
  else
    if (cnt >= 100)
    {
      lcd.setCursor(13, 1);
      lcd.print(cnt);
    }
    else
      if ((cnt < 100) && (cnt > 9))
      {
        lcd.setCursor(15, 1);
        lcd.print(" ");
        lcd.setCursor(13, 1);
        lcd.print(cnt);
      }
      else
      {
        lcd.setCursor(14, 1);
        lcd.print("  ");
        lcd.setCursor(13, 1);
        lcd.print(cnt);
      }
}

void stopDelay2( float t1, unsigned long end_time )
{
  unsigned long temp_timer = millis();
  
  while (millis() - temp_timer < (unsigned long)(t1 * 1000) && (!IN3_trig))
  {
    cntDown((signed long)((end_time - millis()) * 0.001)); //ничего не делать :3
  }
}

void autoMode( float t1, float t2, float t3 )
{
  unsigned long local_timer, end_time;
  boolean mode_trig = 1; //1 - вперед, 0 - назад 
  
  stopInterrupts();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ABTOMAT");   
  lcd.write(1);
  lcd.print("4ECK");
  lcd.write(1);
  lcd.write(0);
  lcd.print(" PE");
  lcd.write(7);
  lcd.write(1);
  lcd.print("M"); 
  lcd.setCursor(0, 1);
  lcd.print("OCT.BPEMEH");
  lcd.write(1);
  lcd.print(":     CEK");
  stopDelay(t1);
  end_time = millis() + (unsigned long)(t3 * 1000);
  while (((signed long)(end_time - millis()) >= 0) && (!IN3_trig))
  {
    if (mode_trig)
    {
      local_timer = millis() + (unsigned long)(t2 * 1000);
      while (((signed long)(local_timer - millis()) > 0) && ((signed long)(end_time - millis()) >= 0) && (!IN3_trig))
      {
        digitalWrite(OUT1, HIGH);
        digitalWrite(OUT3, HIGH);
        cntDown((signed long)((end_time - millis()) * 0.001)); 
      }
        digitalWrite(OUT1, LOW);
        digitalWrite(OUT3, LOW);
        mode_trig = 0;
        if ((signed long)(end_time - millis()) <= 0)
        {
          lcd.setCursor(13, 1);
          lcd.print(0);
          stopDelay(t1);
        }
        else
          stopDelay2(t1, end_time);
    }
    else
    {
      local_timer = millis() + (unsigned long)(t2 * 1000);
      while (((signed long)(local_timer - millis()) > 0) && ((signed long)(end_time - millis()) >= 0) && (!IN3_trig))
      {
        digitalWrite(OUT2, HIGH);
        digitalWrite(OUT4, HIGH);
        cntDown((signed long)((end_time - millis()) * 0.001));  
      }
        digitalWrite(OUT2, LOW);
        digitalWrite(OUT4, LOW);
        mode_trig = 1;
        if ((signed long)(end_time - millis()) <= 0)
        {
          lcd.setCursor(13, 1);
          lcd.print(0);
          stopDelay(t1);
        }
        else
          stopDelay2(t1, end_time);
    }
  }
  if (!IN3_trig)
  {
    stopAll();
    lcd.setCursor(3, 0);
    lcd.print("PE");
    lcd.write(7);
    lcd.write(1);
    lcd.print("M 3ABEP");
    lcd.write(6);
    lcd.print("EH");
  }
  activateInterrupts();
}
