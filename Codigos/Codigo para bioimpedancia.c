#include "trtSettings.h"
#include "trtkernel_1284.c"
#include <stdio.h>
#include <avr / sleep.h>
#include <avr / io.h>
#include <avr / interrupt.h>
#include <avr / pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include <util / delay.h> // necesario para lcd_lib
#include "lcd_lib.h"
// biblioteca de comunicaciones en serie
// No te metas con los semáforos
#define SEM_RX_ISR_SIGNAL 1
#define SEM_STRING_DONE 2 // el usuario presionó <enter>
#include "trtUart.c"
// descriptor de archivo UART
// putchar y getchar están en uart.c
ARCHIVO uart_str = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);
// semáforo para proteger la variable compartida
#define SEM_SHARED 3
const int8_t LCD_initialize [] PROGMEM = "LCD inicializado \ 0";
// --- controlamos los LED desde los botones y uart --------------------
// ingresa argumentos a cada hilo
// no se usa realmente en este ejemplo
int args [2];
tampón de carbón [8];
// ********************************************** **********
// temporizador 0 desbordamiento ISR
ISR (TIMER0_COMPA_vect) 
{      
  PORTB ^ = (1 << PINB3); // genera una onda cuadrada al alternar B3
} 
// parámetros compartidos
uint16_t age;
uint16_t sex; // 1 si es hombre, 0 si es mujer
uint16_t peso;
float body_fat;
resistencia a la flotación;
char Ain;
// --- definimos la tarea 1 ----------------------------------------
estimación nula (void * args) 
  {  
    uint32_t rel, muerto;
  mientras (1)
  {
    trtWait (SEM_SHARED);
    // obtener la muestra  
    Ain = ADCH;
    // iniciar otra conversión
    ADCSRA | = (1 << ADSC); 
    // resistencia = (flotación) Ain * 5.0 / 255.0 / .000012; // convierte la lectura de ADC a voltaje bruto, R = V / I con un valor de corriente de 12 uA, del diseño del circuito
    // calcula la grasa corporal con ecuaciones de regresión
    si (sexo == 1)
    {
      body_fat = 0.0923 * peso + 0.1605 * edad - 0.0263 * (flotación) Ain;
    }
    más si (sexo == 0)
    {
      body_fat = 0.1871 * peso + 0.5800 * edad - 0.0920 * (flotación) Ain;
    }
    si (body_fat <0)
    {
      body_fat = 0.0;
    }
    trtSignal (SEM_SHARED);
    // Dormir
      rel = trtCurrentTime () + SECONDS2TICKS (0.04);
      muerto = trtCurrentTime () + SECONDS2TICKS (0.04);
      trtSleepUntil (rel, muerto);
  }
  }
// --- definimos la tarea 2 ----------------------------------------
void LCDComm (void * args) 
  {
    uint32_t rel, muerto;
  tampón de carbón [8];
  mientras (1)
  {
    trtWait (SEM_SHARED);
    // imprime el porcentaje de grasa y los parámetros
    LCDGotoXY (0,0);
    si (body_fat! = 0)
    {
      sprintf (búfer, "Grasa corporal:% .1f %%", grasa corporal);
      LCDstring (búfer, strlen (búfer));
      LCDGotoXY (0,1);
      sprintf (búfer, "A:% i W:% i G:% i", edad, peso, sexo);
      LCDstring (búfer, strlen (búfer));
    }
    más
    {
      sprintf (búfer, "No listo");
      LCDstring (búfer, strlen (búfer));
      LCDGotoXY (0,1);
      sprintf (búfer, "Sin entrada");
      LCDstring (búfer, strlen (búfer));
    }
    trtSignal (SEM_SHARED);
    // Dormir
      rel = trtCurrentTime () + SECONDS2TICKS (0.2);
      muerto = trtCurrentTime () + SECONDS2TICKS (0.4);
      trtSleepUntil (rel, muerto);
  }
  }
// --- definimos la tarea 3 ----------------------------------------
void serialComm (void * args) 
  {
  char cmd [5];
  float input_arg;
  mientras (1)
  {
    // comandos:
    fprintf (salida estándar, ">");
    fscanf (stdin, "% s% f", cmd, & input_arg);
    trtWait (SEM_STRING_DONE);
    // actualizar los parámetros compartidos
    trtWait (SEM_SHARED); 
    si (cmd [0] == 'a')
    {
      edad = input_arg;
    }
    si (cmd [0] == 'g')
    {
      sex = input_arg;
    } 
    si (cmd [0] == 'w') 
    {
      peso = entrada_arg;
    } 
    trtSignal (SEM_SHARED);
  }
  }
// ********************************************** **********
