#include "Variables.h"

ISR(ADC_vect)
{
  /*datoL =ADCL;
  datoH = ADCH;
  datolisto = 1;*/
  
  /*if(crescendo ==1)
  {
    bufferficticio[contadorcillo1] = contadorcillo2;
    contadorcillo1++;
    contadorcillo2++;
    bufferficticio[contadorcillo1] = contadorcillo3;
    contadorcillo1++;
    if(contadorcillo2 == 255)
    {
      contadorcillo2 = 0;
      contadorcillo3++;
      if (contadorcillo3 == 4)
      {
        crescendo = 0;
        contadorcillo3 = 0;     
      }
    }
  }
  else
  {
    bufferficticio[contadorcillo1] = (254 - contadorcillo2);
    contadorcillo1++;
    contadorcillo2++;
    bufferficticio[contadorcillo1] = (3 - contadorcillo3);
    contadorcillo1++;
    if (contadorcillo2 == 255)
    {
      contadorcillo2 = 0;
      contadorcillo3++;
      if (contadorcillo3 == 4)
      {
        crescendo = 1;
        contadorcillo3 = 0;
      }
    }
  }
  if (contadorcillo1 == (CAPACIDAD_BLOQUE *(numerobloque + 1)))
  {
    datolisto = 1;
  }
  if (contadorcillo1 >= (CAPACIDAD_BUFFER - 1))
  {
    contadorcillo1 = 0;
  }*/
  

  bufferADC[counterADC]= ADCL;
  ++counterADC;
  bufferADC[counterADC] = ADCH;
  ++counterADC;
  if (counterADC == (CAPACIDAD_BLOQUE *(numerobloque + 1)))
  {
    datolisto = 1;
  }
  if (counterADC >= (CAPACIDAD_BUFFER - 1))
  {
    counterADC = 0;
  }
  
  
}

void serialEvent()
{  
  if(Serial.available() == 17)
  {
    v.modoest = Serial.read();
    v.modorecep = Serial.read();
    v.mastnumero[0] = Serial.read();
    v.mastnumero[1] = Serial.read();
    v.multmastp = Serial.read();
    v.delaynumero[0] = Serial.read();
    v.delaynumero[1] = Serial.read();
    v.multdelay = Serial.read();
    v.traindnumero[0] = Serial.read();
    v.traindnumero[1] = Serial.read();
    v.multtraind = Serial.read();
    v.trainpnumero[0] = Serial.read();
    v.trainpnumero[1] = Serial.read();
    v.multtrainp = Serial.read();
    v.pulsednumero[0] = Serial.read();
    v.pulsednumero[1] = Serial.read();
    v.multpulsed = Serial.read();
    
    vt.masttotal = procesameste(v.mastnumero , v.multmastp);
    vt.delaytotal = procesameste(v.delaynumero, v.multdelay);
    vt.traindtotal = procesameste(v.traindnumero, v.multtraind);
    vt.trainptotal = procesameste (v.trainpnumero, v.multtrainp);
    vt.pulsedtotal = procesameste (v.pulsednumero, v.multpulsed);
    
    sacarcuentas();   
    estadoestimular();
    
    /*char hola[50];
    ultoa(vt.masttotal, hola, 10);
    Serial.write(hola);*/
  }
  
}
