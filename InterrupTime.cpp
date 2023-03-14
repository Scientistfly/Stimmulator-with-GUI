#include "Variables.h"

void tdelay()
{
  if(locura == 1)
  {
    if (finalrapido == 2 && batisenal == 1)
    {
      tiempo = 0;
    }
    else
    {
      tiempo = 1;
      finalrapido ++;
      pulsito();
    }
  }
  else
  {
    /*PORTB = B10;*/
    PORTD |= B10000000;
    /*digitalWrite(outPin, HIGH);*/
    tiempo = 2;
    if (v.modoest == 't' || v.modoest == 'u')
    {
      counter = cuentatotal;
      parte = 1;
    }
  }
}

void durap()
{
  if(vt.delaytotal == 0 && extrat == 0)
  {
    if(finalrapido == 2 && batisenal == 1)
    {
      /*PORTB = B00;*/
      PORTD &= ~(B11111100);
      /*digitalWrite(outPin, LOW);*/
      tiempo = 0;
    }
    else
    {
      finalrapido ++;
      tiempo = 2;
      pulsito();
    }
  }
  else
  {
    /*digitalWrite(outPin, LOW);*/
    if (extrat == 0)
    {
      if (finalrapido == 2 && batisenal == 1)
      {
        /*PORTB = B00;*/
        PORTD &= ~(B11111100);
        tiempo = 0;
      }
      else
      {
        PORTD ^= B10001000;
        PORTD &= ~(B00001000);
        /*PORTB = B01;
        PORTB = B00;*/
        finalrapido ++;
        tiempo = 1;
        /*pulsito();*/
      }
    }
    else
    {
      PORTD &= ~(B11111100);
      /*PORTB = B00;*/
      tiempo = 3;
    }
  }
}

void durat1()
{
  PORTD &= ~(B11111100);
  /*PORTB = B00;*/
  /*digitalWrite(outPin, LOW);*/
  tiempo = 2;
  parte = 2;
  counter = counter -1;
  if (counter == 1)
  {
    final = 1;
  }
}

void durat2()
{
  PORTD |= B10000000;
  /*PORTB = B10;*/
  /*digitalWrite(outPin, HIGH);*/
  tiempo = 2;
  parte = 1;
  counter = counter -1;
  if (counter == 1)
  {
    final = 1;
  }
}

void duratfinal()
{
  if (finalrapido == 2 && batisenal == 1)
  {
    PORTD &= ~(B11111100);
    /*PORTB = B00;*/
    /*digitalWrite(outPin, LOW);*/
    tiempo = 0;
  }
  else if(vt.delaytotal == 0 && extrat == 0)
  {
    PORTD |= B10001000;
    PORTD &= ~(B00001000);
    /*PORTB = B11;
    PORTB = B10;*/
    /*digitalWrite(outPin, HIGH);
    pulsito();*/
    tiempo = 2;
    finalrapido ++;
    counter = cuentatotal;
    parte = 1;
    final = 0;
  }
  else if (extrat == 0)
  {
    if ((PORTD >> 7) == B1)
    {
      char jj[] = "1";
      Serial.print(jj);
      PORTD ^= B10001000;
    }
    else
    {
      char jj[] = "2";
      Serial.print(jj);
      PORTD |= B00001000;
    }
    PORTD &= ~(B11111100);
    /*PORTB = B01;
    PORTB = B00;*/
    /*digitalWrite(outPin, LOW);
    pulsito();*/
    finalrapido ++;
    tiempo = 1;
    final = 0;
  }
  else
  {
    PORTD &= ~(B11111100);
    /*PORTB = B00;*/
    /*digitalWrite(outPin, LOW);*/
    tiempo = 3;
    final = 0;
  }
}

void textra()
{
  if (finalrapido == 2 && batisenal == 1)
  {
    tiempo = 0;
  }
  else if(vt.delaytotal == 0)
  {
    finalrapido ++;
    PORTD |= B10001000;
    PORTD &= ~(B00001000);
    /*PORTB = B11;
    PORTB = B10;*/
    /*digitalWrite(outPin, HIGH);
    pulsito();*/
    tiempo = 2;
    if (v.modoest == 't')
    {
      counter = cuentatotal;
      parte = 1;
    }
  }
  else
  {
    finalrapido ++;
    tiempo = 1;
    pulsito();
  }
}
