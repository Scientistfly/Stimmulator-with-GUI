#include "Variables.h"
#include <TimerOne.h>

unsigned long procesameste(char a[2], char b)
{
  long dial1;
  String parapasar;
  for (int i = 0; i < 2; i++)
  {
    const char ch = a[i];
    parapasar += ch;
  }
  char *parapasar2 = new char [2];
  strcpy (parapasar2, parapasar.c_str());        /*esto cambia de string a c-string*/
  
  dial1 = strtol(parapasar2, NULL, 16);
  
  int prueba = b - '0';     /*esto pasa de char a int*/
  prueba = prueba - 1;
  
  unsigned long dial2 = 1;
  if (prueba != -1)
  {
    for (int i = 0; i < prueba; i++)
    {
      dial2 = dial2 * 10;
    }
  }
  unsigned long total;
  
  total =dial1 * dial2;
  delete [] parapasar2;
  return total; 
}

//int calcularduty()
//{
//  /*int prescale[] = {0,1,8,64,256,1024};
//  int prescaleValue = prescale[Timer1.clockSelectBits];
//  long precision = (F_CPU / 128000) * prescaleValue;
//  long period = precision * ICR1 / 100;
//  int hola = map(vt.pulsedtotal, 0, period, 0,1024);*/
//  int hola = map(vt.pulsedtotal, 0, vt.trainptotal, 0, 1024);
//  return hola;
//}

void sacarcuentas()
{
  if (vt.masttotal == 0)
  {
    v.modoest = 'a';
  }
  else if(vt.delaytotal > vt.masttotal || vt.pulsedtotal == 0)
  {
    vt.delaytotal = vt.masttotal;
    locura = 1;
  }
  else
  {
    if (v.modoest == 'p' || v.modoest == 's')
    {
      if((vt.delaytotal + vt.pulsedtotal) >= vt.masttotal)
      {
        vt.pulsedtotal = vt.masttotal - vt.delaytotal;
        extrat = 0;
        locura = 0;
      }
      else
      {
        vt.tiempoextra = vt.masttotal - vt.delaytotal - vt.pulsedtotal;
        extrat = 1;
        locura = 0;
      }
    }
    else if (v.modoest == 't' || v.modoest == 'u')
    {
      if(vt.traindtotal == 0 || vt.trainptotal == 0)
      {
        vt.delaytotal = vt.masttotal;
        locura = 1;
      }
      else
      {
        if (vt.trainptotal > vt.traindtotal)
        {
          vt.trainptotal = vt.traindtotal;         
        }
      
        if ((vt.delaytotal + vt.traindtotal) > vt.masttotal)
        {
          vt.traindtotal = vt.masttotal - vt.delaytotal;
          if (vt.pulsedtotal >= vt.trainptotal)
          {
            vt.pulsedtotal = vt.traindtotal;
            if (v.modoest == 't')
            {
              v.modoest = 'p';
            }
            else
            {
              v.modoest = 's';
            }
            sacarcuentas();
          }
          else
          {
            calcularpwm();
            extrat = 0;
            locura = 0;
          }
        }
        else
        {
          vt.tiempoextra = vt.masttotal - vt.delaytotal - vt.traindtotal;
          if (vt.pulsedtotal >= vt.trainptotal)
          {
            vt.pulsedtotal = vt.traindtotal;
            if (v.modoest == 't')
            {
              v.modoest = 'p';
            }
            else
            {
              v.modoest = 's';
            }
            sacarcuentas();
          }
          else
          {
            calcularpwm();
            extrat = 1;
            locura = 0;
          }
        }       
      }
    }
  }
}       


void calcularpwm()
{
  rep = vt.traindtotal / vt.trainptotal;
  vt.residuo = vt.traindtotal - (vt.trainptotal * rep);
  vt.tlow = vt.trainptotal - vt.pulsedtotal;
  if(vt.residuo == 0)
  {
    veceshigh = rep;
    veceslow = rep;
    cuentatotal = veceshigh + veceslow;
    vt.trestante = vt.tlow;
  }
  if(vt.residuo > vt.pulsedtotal)
  {
    veceshigh = rep + 1;
    veceslow = rep +1;
    cuentatotal = veceshigh + veceslow;
    vt.trestante = vt.residuo - vt.pulsedtotal;
  }
  else
  {
    veceshigh = rep + 1;
    veceslow = rep;
    cuentatotal = veceshigh + veceslow;
    vt.trestante = vt.residuo;
  }
}

void pulsito()
{
  /*PORTB |= 1;
  PORTB &= (~1);*/
  PORTD |= B00010000;
  PORTD &= ~(B00010000);
  /*digitalWrite(sincroPin, HIGH);
  digitalWrite(sincroPin, LOW);*/
}

void estadoestimular()
{
  if (v.modorecep == 'e')
  {
    sbi(ADCSRA, ADEN);
    sbi(ADCSRA, ADSC);
    //datolisto = 1;
  }
  else
  {
    datolisto = 0;
    cbi(ADCSRA, ADEN);
  }
  if (v.modoest == 's'|| v.modoest == 'u')
  {
    batisenal = 1;
    finalrapido = 0;
  }
  else
  {
    batisenal = 0;
    finalrapido = 0;
  }
  
  if (v.modoest == 'a')
  {
    tiempo = 0;
    Timer1.detachInterrupt();
    /*PORTB = B00;*/
    PORTD &= ~(B11111100);
    
    /*digitalWrite(outPin, LOW);*/
  }
  else if (locura == 1)
  {
    tiempo = 1;
    /*PORTB = B01;
    PORTB = B00;*/
    PORTD &= ~(B11111100);
    PORTD |= B00010000;
    PORTD &= ~(B00010000);
    /*digitalWrite(outPin, LOW);
    pulsito();*/
  }
  else
  {
    if (v.modoest == 's' || v.modoest == 'p')
    {
      if (vt.delaytotal == 0)
      {
        tiempo = 2;
        /*PORTB &= ~(B00111111)
        PORTB |= B00010011;
        PORTB &= ~(B00000001);*/
        PORTD &= ~(B11111100);
        PORTD |= B10010000;
        PORTD &= ~(B00010000);
        /*digitalWrite(outPin, HIGH);
        pulsito();*/
      }
      else
      {
        tiempo = 1;
        /*PORTB = B01;
        PORTB = B00;*/
        PORTD &= ~(B11111100);
        PORTD |= B00010000;
        PORTD &= ~(B00010000);
        /*digitalWrite(outPin, LOW);
        pulsito();*/
      }
    }
    else if (v.modoest == 't' || v.modoest == 'u')
    {
      if (vt.delaytotal == 0)
      {
        tiempo = 2;
        /*PORTB = B11;
        PORTB = B10;*/
        PORTD &= ~(B11111100);
        PORTD |= B10010000;
        PORTD &= ~(B00010000);
        /*digitalWrite(outPin, HIGH);
        pulsito();*/
        counter = cuentatotal;
        parte = 1;
      }
      else
      {
        tiempo = 1;
        /*PORTB = B01;
        PORTB = B00;*/
        PORTD &= ~(B11111100);
        PORTD |= B00010000;
        PORTD &= ~(B00010000);
        /*digitalWrite(outPin, LOW);
        pulsito();*/
      }
    }
  }
}
