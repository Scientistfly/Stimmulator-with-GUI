//-------------------------------------------------------------------------------------
//Includes
//-------------------------------------------------------------------------------------
#include <TimerOne.h>
#include "Variables.h"

//-------------------------------------------------------------------------------------
//Variables Globales
//-------------------------------------------------------------------------------------

Variables v;
Variablestotales vt;
const int outPin = 9;
const int sincroPin = 8;
int bitestimulacion = 7;
int bitsincronizacion = 4;
volatile int tiempo = 0;
volatile int counter = 0;
volatile int extrat = 0;
volatile int locura =0;
volatile int duty = 0;
volatile int rep = 0;
volatile int veceshigh = 0;
volatile int veceslow = 0;
volatile int cuentatotal = 0;
volatile int final = 0;
volatile int parte = 0;
volatile int finalrapido = 0;
volatile int batisenal = 0;
volatile unsigned int datoL = 0;
volatile unsigned int datoH = 0;
volatile int datolisto = 0;
volatile int counterADC = 0;
volatile uint8_t bufferADC[CAPACIDAD_BUFFER];
volatile int numerobloque = 0;
volatile uint8_t bufferficticio[CAPACIDAD_BUFFER];
volatile int contadorcillo1 = 0;
volatile int contadorcillo2 = 0;
volatile int contadorcillo3 = 0;
volatile int crescendo = 1;

//-------------------------------------------------------------------------------------
//Rutinas Principales
//-------------------------------------------------------------------------------------

void setup()
{
  ADCinicio();
  Serial.begin(115200);
  //Serial.begin(57600);
  /*DDRB |= B00111111;*/
  DDRD |= B11111100;
  /*pinMode(outPin, OUTPUT);
  pinMode(sincroPin, OUTPUT);*/
  Timer1.initialize(1000);
  pinMode(3, OUTPUT);
}

void loop()
{
  if (datolisto == 1)
  {
    /*Serial.write((uint8_t *)bufferficticio, 2);
    Serial.write((uint8_t *)bufferficticio + 2, 2);
    datolisto = 0;*/
    
    cbi(ADCSRA, ADEN);
    PORTD |= B01000000;
    Serial.write((uint8_t *)bufferADC + (CAPACIDAD_BLOQUE * numerobloque), CAPACIDAD_BLOQUE);
    PORTD &= ~(B01000000);
    sbi(ADCSRA, ADEN);
    sbi(ADCSRA, ADSC);
    numerobloque++;
    if(numerobloque == NUMERO_BLOQUES)
    {
      numerobloque = 0;
    }
    datolisto = 0;

    /*datoH = datoH << 8;
    unsigned int mensaje = datoH | datoL;
    String ari = String(mensaje, HEX);
    while (ari.length() < 3)
    {
    ari = "0" + ari;
    }
    Serial.print(ari);*/
    
    datolisto = 0;
  }
  
  if (tiempo != 0)
  {
    switch(tiempo)
    {
      case 1:
        Timer1.detachInterrupt();       
        Timer1.attachInterrupt(tdelay, vt.delaytotal);
        tiempo = 0;
        break;
      case 2:
        Timer1.detachInterrupt();
        if (v.modoest == 's'|| v.modoest == 'p')
        {
          Timer1.attachInterrupt(durap, vt.pulsedtotal);
        }
        else if (v.modoest == 'u' || v.modoest == 't')
        {
          if (final == 1)
          {
            Timer1.attachInterrupt(duratfinal, vt.trestante);
          }
          else if (parte == 1)
          {
            Timer1.attachInterrupt(durat1, vt.pulsedtotal);
          }
          else
          {
            Timer1.attachInterrupt(durat2, vt.tlow);
          }
        }
        tiempo = 0;
        break;
      case 3:
        Timer1.detachInterrupt();
        Timer1.attachInterrupt(textra, vt.tiempoextra);
        tiempo = 0;
        break;
    }
  }
}


