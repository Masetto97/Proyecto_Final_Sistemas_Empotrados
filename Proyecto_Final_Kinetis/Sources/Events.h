/* ###################################################################
**     Filename    : Events.h
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
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
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

#ifdef __cplusplus
extern "C" {
#endif 

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
void Cpu_OnNMI(void);


void PulsadorAjuste_OnInterrupt(void);
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

void PulsadorTemperatura_OnInterrupt(void);
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

void PulsadorHora_OnInterrupt(void);
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
void PuertoSerie_OnError(void);

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
void PuertoSerie_OnRxChar(void);

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
void PuertoSerie_OnTxChar(void);

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
void PuertoSerie_OnFullRxBuf(void);

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
void PuertoSerie_OnFreeTxBuf(void);

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
void Contador_OnInterrupt(void);

void Temperatura_OnEnd(void);
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

void Temperatura_OnCalibrationEnd(void);
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

/*
** ===================================================================
**     Event       :  ContadorAlarma_OnInterrupt (module Events)
**
**     Component   :  ContadorAlarma [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ContadorAlarma_OnInterrupt(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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
