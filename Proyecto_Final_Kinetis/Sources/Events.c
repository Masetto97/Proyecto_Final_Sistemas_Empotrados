/* ###################################################################
**     Filename    : Events.c
**     Project     : Proyecto_Final_Kinetis
**     Processor   : MK64FN1M0VLQ12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2021-04-01, 17:13, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMI - void Cpu_OnNMI(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MK64FN1M0LQ12]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PulsadorAjuste_OnInterrupt (module Events)
**
**     Component   :  PulsadorAjuste [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
extern int modo_funcionamiento;
extern bool cambio_pulsador_s3a3;
void PulsadorAjuste_OnInterrupt(void)
{
  /* Write your code here ... */
  cambio_pulsador_s3a3 = TRUE;

}

/*
** ===================================================================
**     Event       :  PulsadorTemperatura_OnInterrupt (module Events)
**
**     Component   :  PulsadorTemperatura [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PulsadorTemperatura_OnInterrupt(void)
{
  /* Write your code here ... */
  modo_funcionamiento = 1;
}

/*
** ===================================================================
**     Event       :  PulsadorHora_OnInterrupt (module Events)
**
**     Component   :  PulsadorHora [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PulsadorHora_OnInterrupt(void)
{
  /* Write your code here ... */
  modo_funcionamiento = 0;
}

/*
** ===================================================================
**     Event       :  PuertoSerie_OnError (module Events)
**
**     Component   :  PuertoSerie [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PuertoSerie_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PuertoSerie_OnRxChar (module Events)
**
**     Component   :  PuertoSerie [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PuertoSerie_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PuertoSerie_OnTxChar (module Events)
**
**     Component   :  PuertoSerie [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PuertoSerie_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PuertoSerie_OnFullRxBuf (module Events)
**
**     Component   :  PuertoSerie [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
extern char *valor_usuario;
extern word rec;
void PuertoSerie_OnFullRxBuf(void)
{
    /* Write your code here ... */
	char *aux; // variable auxiliar de lectura para comprobar errores
	if(PuertoSerie_RecvBlock(&aux, sizeof(aux), &rec) == ERR_OK) {
		// en caso de no haber ningún error, lo guardamos en la variable compartida
		valor_usuario = aux;
	}
}

/*
** ===================================================================
**     Event       :  PuertoSerie_OnFreeTxBuf (module Events)
**
**     Component   :  PuertoSerie [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PuertoSerie_OnFreeTxBuf(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  Contador_OnInterrupt (module Events)
**
**     Component   :  Contador [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
extern int segundos;
void Contador_OnInterrupt(void)
{
  /* Write your code here ... */
  Tiempo_AddTick();
  segundos ++;
}

/*
** ===================================================================
**     Event       :  Temperatura_OnEnd (module Events)
**
**     Component   :  Temperatura [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
extern word valor_temperatura;
extern bool valor_temperatura_disponible;
void Temperatura_OnEnd(void)
{
  /* Write your code here ... */
  // lectura del puerto analógico
  Temperatura_GetValue16(&valor_temperatura);
  // indicamos al programa principal que está listo el valor
  valor_temperatura_disponible = TRUE;
}

/*
** ===================================================================
**     Event       :  Temperatura_OnCalibrationEnd (module Events)
**
**     Component   :  Temperatura [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Temperatura_OnCalibrationEnd(void)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
