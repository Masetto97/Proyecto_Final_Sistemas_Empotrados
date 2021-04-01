/* ###################################################################
**     Filename    : main.c
**     Project     : Proyecto_Final_Kinetis
**     Processor   : MK64FN1M0VLQ12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2021-04-01, 17:13, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Pins1.h"
#include "PulsadorHora.h"
#include "ExtIntLdd1.h"
#include "PulsadorTemperatura.h"
#include "ExtIntLdd2.h"
#include "PulsadorAjuste.h"
#include "ExtIntLdd3.h"
#include "LedD1.h"
#include "BitIoLdd1.h"
#include "LedD2.h"
#include "BitIoLdd2.h"
#include "LedD3.h"
#include "BitIoLdd3.h"
#include "LedD4.h"
#include "BitIoLdd4.h"
#include "TU1.h"
#include "Zumbador.h"
#include "PwmLdd1.h"
#include "Display.h"
#include "STCP1.h"
#include "BitIoLdd5.h"
#include "DS1.h"
#include "BitIoLdd6.h"
#include "SHCP1.h"
#include "BitIoLdd7.h"
#include "WAIT1.h"
#include "MCUC1.h"
#include "PuertoSerie.h"
#include "ASerialLdd1.h"
#include "Tiempo.h"
#include "CS1.h"
#include "UTIL1.h"
#include "Contador.h"
#include "TimerIntLdd1.h"
#include "TU2.h"
#include "Temperatura.h"
#include "AdcLdd1.h"
#include "WAIT2.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PDD_Includes.h"
#include "Init_Config.h"
/* User includes (#include below this line is not maintained by Processor Expert) */

/**********************
 * variables goblales *
 **********************/
/**
 * variable para contabilizar segundos (necesario para el cambio de modo)
 */
int segundos = 0;
/**
 * variable con el valor leido de la temperatura
 */
word valor_temperatura;
/**
 * Variable para indicar que se ha leido el valor de la temperatura
 * en la interrupción.
 */
bool valor_temperatura_disponible = FALSE;
/**
 * variable para indicar el modo de funcionamiento del sistema
 * 	- modo mostrar hora (por defecto)
 * 	- modo mostrar temperatura
 * 	- modo ajuste hora/alarma
 */
int modo_funcionamiento = 0;
/**
 * variable para detectar la pulsación del S3A3 (modo configuración)
 * 	- es necesario detectar que mantiene pulsado el pulsador sin soltarlo
 */
bool cambio_pulsador_s3a3 = FALSE;



/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  // byte maps para los números del 0 al 9
  const byte digito[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
  // byte maps para seleccionar los dígitos del display de la multifunction shield
  const byte segmento[] = {0xF1,0xF2,0xF4,0xF8};
  // variable para gestionar la hora GenericTimeDate
  TIMEREC hora;
  // variables auxiliares para la hora
  int value_hora, value_minutos;
  // variable auxiliar para la temperatura
  float temp;
  // variable para saber que se ha detectado la pulsación de S3A3
  bool activando_configuracion = FALSE;
  // variable puerto serie
  word env;
  // mensajes puerto serie
  char modo_ajuste_cabecera[] = "MODO AJUSTE HORA/ALARMA";
  char modo_ajuste_hora[] = "Introduzca hora (HH:MM): ";
  char modo_ajuste_alarma[] = "Introduzca la hora de la alarma (HH:MM): ";

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  // habilitamos el display
  Display_Enable();
  Display_Init();
  // deshabilitamos el zumbido
  Zumbador_Disable();

  /* For example: for(;;) { } */
  for(;;) {
	  // gestión de la hora
	  if (modo_funcionamiento == 0) {
		  // obtenemos la hora y la almacenamos en variables temporales
		  Tiempo_GetTime(&hora);
		  value_hora = hora.Hour;
		  value_minutos = hora.Min;
		  // dibujamos los valores en el display
		  Display_ShiftByte(digito[(((int)value_hora % 100) / 10)]);
		  Display_ShiftByte(segmento[0] );
		  Display_Latch();
		  Display_ShiftByte(digito[(((int)value_hora % 10))] & ~(1 << 7));
		  Display_ShiftByte(segmento[1] );
		  Display_Latch();
		  Display_ShiftByte(digito[(((int)value_minutos % 100) / 10)]);
		  Display_ShiftByte(segmento[2] );
		  Display_Latch();
		  Display_ShiftByte(digito[(((int)value_minutos % 10))]);
		  Display_ShiftByte(segmento[3] );
		  Display_Latch();
	  }

	  // gestión de la temperatura
	  if (modo_funcionamiento == 1) {
		  Temperatura_Measure(FALSE);
		  if (valor_temperatura_disponible) {
			  // reiniciamos la variable bandera
			  valor_temperatura_disponible = FALSE;
			  // formateamos el valor de temperatura
			  temp = (float) valor_temperatura*250/65535;
			  // dibujamos los valores en el display
			  Display_ShiftByte(digito[(((int)temp % 100) / 10)]);
			  Display_ShiftByte(segmento[0] );
			  Display_Latch();
			  Display_ShiftByte(digito[(((int)temp % 10))] & ~(1 << 7));
			  Display_ShiftByte(segmento[1] );
			  Display_Latch();
			  Display_ShiftByte(digito[(((int)(temp * 10) % 10))]);
			  Display_ShiftByte(segmento[2] );
			  Display_Latch();
			  Display_ShiftByte(digito[(((int)(temp * 100) % 10))]);
			  Display_ShiftByte(segmento[3] );
			  Display_Latch();
		  }
	  }

	  // gestión de la pulsación del S3A3
	  if (cambio_pulsador_s3a3) { // detectamos una pulsación del S3A3
		  activando_configuracion = !activando_configuracion;
		  cambio_pulsador_s3a3 = FALSE;
		  segundos = 0;
	  }

	  // estamos manteniendo pulsado el S3A3
	  if(activando_configuracion) {
		  if (segundos == 2) {
			  modo_funcionamiento = 2;
		  }
	  }

	  // gestión de la configuracion
	  if (modo_funcionamiento == 2) {
		  while(PuertoSerie_SendBlock(&modo_ajuste_cabecera, sizeof(modo_ajuste_cabecera), &env)!= ERR_OK) {}
		  while(PuertoSerie_SendChar(10) != ERR_OK) {}
		  while(PuertoSerie_SendChar(13) != ERR_OK) {}
		  WAIT1_Waitms(500);
	  }
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
