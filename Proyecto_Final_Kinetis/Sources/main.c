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
#include "Contador250ms.h"
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

#define MODO_TEMPERATURA 0
#define MODO_HORA 1
#define MODO_CONFIG 2

/**********************
 * variables goblales *
 **********************/
/**
 * variables para identificar los dígitos a mostrar en el display
 */
// byte maps para los números del 0 al 9
const byte digito[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};
// byte maps para seleccionar los dígitos del display de la multifunction shield
const byte segmento[] = {0xF1, 0xF2, 0xF4, 0xF8};
int digito1, digito2, digito3, digito4;
/**
 * variable para contabilizar segundos (necesario para el cambio de modo)
 */
int segundos = 0;
/**
 * variables para contabilizar: alarma y lectura de temperatura
 */
int contador_250ms = 0;
/**
 * variable con el valor leido de la temperatura
 */
word valor_temperatura;
/**
 * variable con los valores enviados por el usuario por el puerto serie
 */
char valor_usuario[5];
/**
 * variable que indica que se ha leido correctamente el contenido
 * del buffer del puerto serie.
 */
bool valor_leido = FALSE;
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
int modo_funcionamiento = MODO_TEMPERATURA;
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
 * 	- FALSE no se ha pulsado
 * 	- TRUE se ha pulsado
 */
bool estado_pulsador_ajuste = FALSE;
bool cambio_estado_pulsador_ajuste = FALSE; // esta variable solo indica cuando se cambia el estado del pulsador
/**
 * variable que indica si la alarma está o no activa
 */
bool alarma_activa = FALSE;
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
// función para visualizar los valores en el display
void display() {
	Display_ShiftByte(digito[digito1]);
	Display_ShiftByte(segmento[0]);
	Display_Latch();
	Display_ShiftByte(digito[digito2] & ~(1 << 7));
	Display_ShiftByte(segmento[1]);
	Display_Latch();
	Display_ShiftByte(digito[digito3]);
	Display_ShiftByte(segmento[2]);
	Display_Latch();
	Display_ShiftByte(digito[digito4]);
	Display_ShiftByte(segmento[3]);
	Display_Latch();
}
// función para desactivar la visualización por el display
void apagar_display() {
	Display_ShiftByte(0xFF);
	Display_ShiftByte(segmento[0]);
	Display_Latch();
	Display_ShiftByte(0xFF);
	Display_ShiftByte(segmento[1]);
	Display_Latch();
	Display_ShiftByte(0xFF);
	Display_ShiftByte(segmento[2]);
	Display_Latch();
	Display_ShiftByte(0xFF);
	Display_ShiftByte(segmento[3]);
	Display_Latch();
}

// función para comprobar si la alarma y la hora del sistema coinciden
bool comprobar_alarma () {
	return (hora.Hour == alarma.Hour && hora.Min == alarma.Min);
}

// función para comprobar el formato de la hora enviada por el puerto serie y obtener los valores de horas y minutos
bool comprobar_formato_hora (char *valor, int *hora, int *minuto) {
	return sscanf(valor, "%2d:%2d", hora, minuto) == 2;
}

// función que permite actualizar la hora del sistema
void actualizar_hora_sistema(int horas, int minutos) {
	if(horas >= 0 && minutos >= 0) {
		Tiempo_SetTime(horas, minutos, 0, 0);
	}
}

// función que permite actualizar la hora de la alarma
void actualizar_alarma_sistema(int horas, int minutos) {
	if(horas >= 0 && minutos >= 0) {
		alarma.Hour = horas;
		alarma.Min = minutos;
	}
}

// función para activar la alarma
void activar_alarma () {
	contador_250ms = 0;
	alarma_activa = TRUE;
}

// función para gestionar las acciones durante la alarma
void sonar_alarma() {
	if (contador_250ms % 2 == 0) {
		Zumbador_Enable();
		LedD1_PutVal(0);
		LedD2_PutVal(0);
		LedD3_PutVal(0);
		LedD4_PutVal(0);
	} else {
		apagar_alarma();
	}
}

// función para desactivar la alarma
void desactivar_alarma () {
	alarma_activa = FALSE;
	apagar_alarma();
}

// función que permite apagar las acciones de la alarma
void apagar_alarma() {
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
  // variables auxiliares para la hora
  int value_hora, value_minutos;
  int *valor_horas_aux, *valor_minutos_aux;
  // variable auxiliar para la temperatura
  float temp;
  // variable para saber que se ha detectado la pulsación de S3A3 TRUE => pulsado FALSE => no pulsado
  bool pulsado_s3a3 = FALSE;
  // variable para indicar que se empiezan a contabilizar los 2 segundos
  bool activando_configuracion = FALSE;
  // variable para solo leer una vez la temperatura cada cierto tiempo
  bool lectura_temperatura = FALSE;
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
	  		activar_alarma();
	  	}
	  	sonar_alarma();
	  } else {
		// solo desactivamos si previamente estaba activado
	    if (alarma_activa) {
	    	desactivar_alarma();
		}
	  }

	  // recogida de datos y temperatura
	  Tiempo_GetTime(&hora);
	  // leemos la temperatura cada 500 ms SOLO UNA VEZ
	  if(contador_250ms % 2 == 0) {
		  if (!lectura_temperatura) {
			  lectura_temperatura = TRUE;
			  Temperatura_Measure(FALSE);
		  }
	  } else {
		  lectura_temperatura = FALSE;
	  }


	  // gestión de la hora
	  if (modo_funcionamiento == MODO_HORA) {
		  // obtenemos qué dígitos mostrar
		  value_hora = hora.Hour;
		  value_minutos = hora.Min;
		  digito1 = (((int)value_hora % 100) / 10);
		  digito2 = (((int)value_hora % 10));
		  digito3 = (((int)value_minutos % 100) / 10);
		  digito4 = (((int)value_minutos % 10));
		  // función para mostrar los valores en el display
		  display();
	  }

	  // gestión de la temperatura
	  if (modo_funcionamiento == MODO_TEMPERATURA) {
		  if (valor_temperatura_disponible) {
			  // reiniciamos la variable bandera
			  valor_temperatura_disponible = FALSE;
			  // formateamos el valor de temperatura
			  temp = (float) valor_temperatura*250/65535;
			  digito1 = (((int)temp % 100) / 10);
			  digito2 = (((int)temp % 10));
			  digito3 = (((int)(temp * 10) % 10));
			  digito4 = (((int)(temp * 100) % 10));
		  }
		  // función para mostrar los valores en el display
		  display();
	  }

	  // gestión de la pulsación del S3A3
	  if (cambio_estado_pulsador_ajuste) {
		  cambio_estado_pulsador_ajuste = FALSE;
		  activando_configuracion = estado_pulsador_ajuste;
		  if (estado_pulsador_ajuste) {
			  segundos = 0;
		  }
	  }

	  // estamos manteniendo pulsado el S3A3
	  if(activando_configuracion) {
		  if (segundos == 2) {
			  activando_configuracion = FALSE;
			  // inicializamos estado
			  modo_funcionamiento = MODO_CONFIG;
			  modo_configuracion = 0;
			  valor_leido = FALSE;
			  desactivar_alarma();
		  }
	  }

	  // gestión de la configuracion
	  if (modo_funcionamiento == MODO_CONFIG) {
		  apagar_display(); // apagamos el display
		  if (modo_configuracion == 0) {
			  while (PuertoSerie_SendBlock(&modo_ajuste_cabecera, sizeof(modo_ajuste_cabecera), &env) != ERR_OK){}
			  while (PuertoSerie_SendChar(10) != ERR_OK){}
			  while (PuertoSerie_SendChar(13) != ERR_OK){}
			  while (PuertoSerie_SendBlock(&modo_ajuste_hora, sizeof(modo_ajuste_hora), &env) != ERR_OK){}
			  modo_configuracion = 1; // esperando respuesta del usuario
		  } else if (modo_configuracion == 1) {
			  // comprobamos el valor leido por el usuario
			  if (valor_leido) {
				  while (PuertoSerie_SendBlock(&valor_usuario, sizeof(valor_usuario), &env) != ERR_OK){}
				  while (PuertoSerie_SendChar(10) != ERR_OK){}
				  while (PuertoSerie_SendChar(13) != ERR_OK){}
				  // comprobamos el formato del valor recibido por puerto serie
				  if (comprobar_formato_hora(valor_usuario, &value_hora, &value_minutos)) {
					  actualizar_hora_sistema(value_hora, value_minutos);
					  modo_configuracion = 2; // preguntar por la alarma
				  } else {
					  modo_configuracion = 0;
				  }
				  valor_leido = FALSE;
			  }
		  } else if (modo_configuracion == 2) {
			  while (PuertoSerie_SendBlock(&modo_ajuste_alarma, sizeof(modo_ajuste_alarma), &env) != ERR_OK){}
			  modo_configuracion = 3; // esperando respuesta del usuario
		  } else if (modo_configuracion == 3) {
			  // comprobamos el valor leido por el usuario
			  if (valor_leido) {
				  while (PuertoSerie_SendBlock(&valor_usuario, sizeof(valor_usuario), &env) != ERR_OK){}
				  while (PuertoSerie_SendChar(10) != ERR_OK){}
				  while (PuertoSerie_SendChar(13) != ERR_OK){}
				  // comprobamos el formato del valor recibido por puerto serie
				  if (comprobar_formato_hora(valor_usuario, &value_hora, &value_minutos)) {
					 actualizar_alarma_sistema(value_hora, value_minutos);
					  modo_configuracion = 4; // finalizar configuracions
				  } else {
					  modo_configuracion = 2;
				  }
				  valor_leido = FALSE;
			  }
		  } else if (modo_configuracion == 4) {
			  while (PuertoSerie_SendBlock(&modo_ajuste_cabecera_fin, sizeof(modo_ajuste_cabecera_fin), &env) != ERR_OK){}
			  while (PuertoSerie_SendChar(10) != ERR_OK){}
			  while (PuertoSerie_SendChar(13) != ERR_OK){}
			  modo_funcionamiento = MODO_HORA;
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
