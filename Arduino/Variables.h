#ifndef VARIABLES
#define VARIABLES
//------------------------------------------------------------------------------
//Includes
//-------------------------------------------------------------------------------
#include <Arduino.h>

//--------------------------------------------------------------------------------
//Defines and Typedefs
//-------------------------------------------------------------------------------

#define CAPACIDAD_BUFFER 20
#define CAPACIDAD_BLOQUE 2
#define NUMERO_BLOQUES 10

// Defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//------------------------------------------------------------------------------------
//Prototipo de Funciones
//------------------------------------------------------------------------------------

void ADCinicio(void);
void tdelay();
void pulsito();
void durap();
void durat1();
void durat2();
void duratfinal();
void textra();
unsigned long procesameste(char a[2], char b);
void sacarcuentas();
void calcularpwm();
void estadoestimular();

//------------------------------------------------------------------------------------
//Declaracion de Variables Globales
//------------------------------------------------------------------------------------

extern volatile unsigned int datoL;
extern volatile unsigned int datoH;
extern volatile int datolisto;
extern volatile int locura;
extern volatile int finalrapido;
extern volatile int batisenal;
extern volatile int tiempo;
extern volatile int counter;
extern volatile int parte;
extern volatile int cuentatotal;
extern volatile int extrat;
extern volatile int final;
extern volatile int veceslow;
extern volatile int veceshigh;
extern volatile int rep;
extern volatile int duty;
extern volatile int counterADC;
extern volatile uint8_t bufferADC[CAPACIDAD_BUFFER];
extern volatile int numerobloque;
extern volatile uint8_t bufferficticio[CAPACIDAD_BUFFER];
extern volatile int contadorcillo1;
extern volatile int contadorcillo2;
extern volatile int contadorcillo3;
extern volatile int crescendo;


struct Variables
{
  char modoest;
  char modorecep;
  char mastnumero[2];
  char delaynumero[2];
  char traindnumero[2];
  char trainpnumero[2];
  char pulsednumero[2];
  char multmastp;
  char multdelay;
  char multtraind;
  char multtrainp;
  char multpulsed;
};

struct Variablestotales
{
  unsigned long masttotal;
  unsigned long delaytotal;
  unsigned long traindtotal;
  unsigned long trainptotal;
  unsigned long pulsedtotal;
  unsigned long tiempoextra;
  unsigned long residuo;
  unsigned long tlow;
  unsigned long trestante;
};

extern Variables v;
extern Variablestotales vt;

#endif
