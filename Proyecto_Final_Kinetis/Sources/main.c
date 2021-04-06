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
#include "ContadorAlarma.h"
#include "TimerIntLdd2.h"
#include "TU3.h"
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
 * variables para contabilizar los ciclos cuando está activada la alarma
 */
int contador_alarma = 0;
/**
 * variable con el valor leido de la temperatura
 */
word valor_temperatura;
/**
 * variable con los valores enviados por el usuario por el puerto serie
 */
char *valor_usuario;
/**
 * variables para gestionar los valores recibidos/eviados por el puerto serie
 */
word env, rec;
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
 * variable para controlar los pasos en el modo configuración
 * 	0.- preguntar hora
 * 	1.- esperar respuesta
 *  2.- preguntar alarma
 * 	3.- esperar respuesta
 */
int modo_configuracion = 0;
/**
 * variable para detectar la pulsación del S3A3 (modo configuración)
 * 	- es necesario detectar que mantiene pulsado el pulsador sin soltarlo
 */
bool cambio_pulsador_s3a3 = FALSE;
/**
 * variables para gestionar la hora GenericTimeDate
 * 	- hora del sistema
 * 	- hora de la alarma
 */
TIMEREC hora;
TIMEREC alarma;


/**********************
 * funciones globales *
 **********************/
// función para comprobar si la alarma y la hora del sistema coinciden
bool comprobar_alarma () {
	return (hora.Hour == alarma.Hour && hora.Min == alarma.Min);
}

// función que permite actualizar la hora del sistema
void actualizar_hora_sistema(int horas, int minutos) {
	Tiempo_SetTime(horas, minutos, 0, 0);
}

// función que permite actualizar la hora de la alarma
void actualizar_alarma_sistema(int horas, int minutos) {
	alarma.Hour = horas;
	alarma.Min = minutos;
}

// función para gestionar las acciones durante la alarma
void sonar_alarma() {
	contador_alarma %= 3;
	if (contador_alarma % 2 == 0) {
		Zumbador_Enable();
		LedD1_PutVal(0);
		LedD2_PutVal(0);
		LedD3_PutVal(0);
		LedD4_PutVal(0);
	} else {
		desactivar_alarma();
	}
}

// función que permite desactivar las acciones de la alarma
void desactivar_alarma() {
	Zumbador_Disable();
	LedD1_PutVal(1);
	LedD2_PutVal(1);
	LedD3_PutVal(1);
	LedD4_PutVal(1);
}


/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
  // byte maps para los números del 0 al 9
  const byte digito[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};
  // byte maps para seleccionar los dígitos del display de la multifunction shield
  const byte segmento[] = {0xF1, 0xF2, 0xF4, 0xF8};
  // variables auxiliares para la hora
  int value_hora, value_minutos, valor_hora_aux;
  // variable auxiliar para la temperatura
  float temp;
  // variable para saber que se ha detectado la pulsación de S3A3
  bool activando_configuracion = FALSE, alarma_activa = FALSE;
  // mensajes puerto serie
  char modo_ajuste_cabecera[] = "MODO AJUSTE HORA/ALARMA";
  char modo_ajuste_cabecera_fin[] = "Se ha configurado la hora y la alarma correctamente";
  char modo_ajuste_hora[] = "Introduzca hora en formato HH:MM: ";
  char modo_ajuste_alarma[] = "Introduzca la hora de la alarma (HH:MM): ";

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  // habilitamos el display
  Display_Enable();
  Display_Init();
  // deshabilitamos el zumbido
  Zumbador_SetRatio16(0xFFFF);
  Zumbador_Disable();
  // TODO: poner hora del sistema

  // alarma por defecto
  alarma.Hour = 6;
  alarma.Min = 30;

  /* For example: for(;;) { } */
  for(;;) {
	  // gestión de la alarma
	  if (comprobar_alarma()) {
	  	if (!alarma_activa) {
	  		alarma_activa = TRUE;
	  		contador_alarma = 0;
	  	}
	  	sonar_alarma();
	  } else {
		// solo desactivamos si previamente estaba activado
	    if (alarma_activa) {
	    	alarma_activa = FALSE;
			desactivar_alarma();
		}
	  }

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
			  valor_usuario = '\0';
		  }
	  }

	  // gestión de la configuracion
	  if (modo_funcionamiento == 2) {
		  if (modo_configuracion == 0) {
			  while (PuertoSerie_SendBlock(&modo_ajuste_cabecera, sizeof(modo_ajuste_cabecera), &env) != ERR_OK){}
			  while (PuertoSerie_SendChar(10) != ERR_OK){}
			  while (PuertoSerie_SendChar(13) != ERR_OK){}
			  while (PuertoSerie_SendBlock(&modo_ajuste_hora, sizeof(modo_ajuste_hora), &env) != ERR_OK){}
			  modo_configuracion = 1; // esperando respuesta del usuario
		  } else if (modo_configuracion == 1) {
			  // comprobamos el valor leido por el usuario
			  if (valor_usuario) {
				  while (PuertoSerie_SendBlock(&valor_usuario, sizeof(valor_usuario), &env) != ERR_OK){}
				  while (PuertoSerie_SendChar(10) != ERR_OK){}
				  while (PuertoSerie_SendChar(13) != ERR_OK){}
				  valor_hora_aux = atoi(&valor_usuario);
				  actualizar_hora_sistema(valor_hora_aux / 100, valor_hora_aux % 100);
				  valor_usuario = '\0';
				  modo_configuracion = 2; // preguntar por la alarma
			  }
		  } else if (modo_configuracion == 2) {
			  while (PuertoSerie_SendBlock(&modo_ajuste_alarma, sizeof(modo_ajuste_alarma), &env) != ERR_OK){}
			  modo_configuracion = 3; // esperando respuesta del usuario
		  } else if (modo_configuracion == 3) {
			  // comprobamos el valor leido por el usuario
			  if (valor_usuario) {
				  while (PuertoSerie_SendBlock(&valor_usuario, sizeof(valor_usuario), &env) != ERR_OK){}
				  while (PuertoSerie_SendChar(10) != ERR_OK){}
				  while (PuertoSerie_SendChar(13) != ERR_OK){}
				  valor_hora_aux = atoi(&valor_usuario);
				  actualizar_alarma_sistema(valor_hora_aux / 100, valor_hora_aux % 100);
				  valor_usuario = '\0';
				  modo_configuracion = 4; // finalizar configuracion
			  }
		  } else if (modo_configuracion == 4) {
			  while (PuertoSerie_SendBlock(&modo_ajuste_cabecera_fin, sizeof(modo_ajuste_cabecera_fin), &env) != ERR_OK){}
			  modo_funcionamiento = 0;
			  modo_configuracion = 0;
		  }
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
