#include <stdio.h>
#include "trtSettings.h"
#include "trtkernel_1284.c"
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
// Configuración de LCD
void init_lcd (vacío) 
{
	LCDinit (); // inicializar la pantalla
	LCDcursorOFF ();
	LCDclr (); // limpiar la pantalla
	LCDGotoXY (0,0);
	CopyStringtoLCD (LCD_initialize, 0, 0);
}
// --- Programa principal ----------------------------------
int main (void) {
	init_lcd ();
	DDRB = (1 << PINB3); // establece B3 en salida, enciende las resistencias pullup
	PORTB = 0xff;
	DDRD = 0x04; // enciende el LED
	// configurar el temporizador 0 para una base de tiempo de 1 mSec 
	TIMSK0 = (1 << OCIE0A); // enciende el temporizador 0 cmp coincide con ISR
	OCR0A = 20; // establece el registro de comparación en 20 ticks de tiempo. Genera una onda de ~ 50 kHz: ajuste para otras frecuencias
	// establecer prescalar
	TCCR0B = 2;
	// activar clear-on-match
	TCCR0A = (1 << WGM01);
	// iniciar el convertidor A a D 
   // canal cero / ajuste izquierdo / EXTERNAL Aref
   // conecta AVcc a Vcc, a través del condensador
   ADMUX = (1 << ADLAR) | (1 << REFS0); // | (1 << REFS1);  
   // habilita ADC y configura el preescalador en 1/128 * 16MHz = 125,000
   // y borrar la habilitación de interrupciones
   // e inicia una conversión
   ADCSRA = (1 << ADEN) | (1 << ADSC) + 7;
  // iniciar la UART - trt_uart_init () está en trtUart.c
  trt_uart_init ();
  stdout = stdin = stderr = & uart_str;
  fprintf (stdout, "\ n \ r Analizador de impedancia bioeléctrica \ n \ r \ n \ r");
  fprintf (stdout, "\ n \ r Por favor ingrese la edad, el peso y el sexo \ n \ r \ n \ r");
  fprintf (stdout, "\ n \ r Formato de comando: a <edad>, w <peso>, g <sexo> \ n \ r \ n \ r");
  // iniciar TRT
  trtInitKernel (80); // 80 bytes para la pila de tareas inactiva
  // --- creamos semáforos ----------
  // Debes crear los dos primeros semáforos si usas el uart
  trtCreateSemaphore (SEM_RX_ISR_SIGNAL, 0); // uart recibe el semáforo ISR
  trtCreateSemaphore (SEM_STRING_DONE, 0); // el usuario escribió <enter>
  // protección variable
  trtCreateSemaphore (SEM_SHARED, 1); // protege las variables compartidas
 // --- creamos tareas ----------------
  trtCreateTask (estimación, 100, SECONDS2TICKS (0.1), SECONDS2TICKS (0.15), & (args [0]));
  trtCreateTask (LCDComm, 100, SECONDS2TICKS (0.1), SECONDS2TICKS (0.2), & (args [0]));
  trtCreateTask (serialComm, 100, SECONDS2TICKS (0.1), SECONDS2TICKS (0.1), & (args [1]));
  // --- Tarea inactiva --------------------------------------
  // solo duerme la CPU para ahorrar energía 
  // cada vez que se ejecuta
  set_sleep_mode (SLEEP_MODE_IDLE);
  sleep_enable ();
  mientras (1) 
  {
  	sleep_cpu ();
  }
}