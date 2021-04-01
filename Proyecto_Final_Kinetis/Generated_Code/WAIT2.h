/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : WAIT2.h
**     Project     : Proyecto_Final_Kinetis
**     Processor   : MK64FN1M0VLQ12
**     Component   : Wait
**     Version     : Component 01.082, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2021-04-01, 18:11, # CodeGen: 7
**     Abstract    :
**          Implements busy waiting routines.
**     Settings    :
**          Component name                                 : WAIT2
**          Manual Clock Values                            : Disabled
**          Delay100usFunction                             : Delay100US
**          RTOS                                           : Disabled
**          Watchdog                                       : Disabled
**     Contents    :
**         Wait10Cycles   - void WAIT2_Wait10Cycles(void);
**         Wait100Cycles  - void WAIT2_Wait100Cycles(void);
**         WaitCycles     - void WAIT2_WaitCycles(uint16_t cycles);
**         WaitLongCycles - void WAIT2_WaitLongCycles(uint32_t cycles);
**         Waitms         - void WAIT2_Waitms(uint16_t ms);
**         Waitus         - void WAIT2_Waitus(uint16_t us);
**         Waitns         - void WAIT2_Waitns(uint16_t ns);
**         WaitOSms       - void WAIT2_WaitOSms(void);
**         Init           - void WAIT2_Init(void);
**         DeInit         - void WAIT2_DeInit(void);
**
**Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**All Rights Reserved.
**
**Redistribution and use in source and binary forms, with or without modification,
**are permitted provided that the following conditions are met:
**
**o Redistributions of source code must retain the above copyright notice, this list
**  of conditions and the following disclaimer.
**
**o Redistributions in binary form must reproduce the above copyright notice, this
**  list of conditions and the following disclaimer in the documentation and/or
**  other materials provided with the distribution.
**
**o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**  contributors may be used to endorse or promote products derived from this
**  software without specific prior written permission.
**
**THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**http: www.freescale.com
**mail: support@freescale.com
** ###################################################################*/
/*!
** @file WAIT2.h
** @version 01.00
** @brief
**          Implements busy waiting routines.
*/         
/*!
**  @addtogroup WAIT2_module WAIT2 module documentation
**  @{
*/         

#ifndef __WAIT2_H
#define __WAIT2_H

/* MODULE WAIT2. */
#include "MCUC1.h" /* SDK and API used */
#include "WAIT2config.h" /* configuration */

/* other includes needed */
#if WAIT2_CONFIG_USE_RTOS_WAIT
  /* include RTOS header files */
  #include "McuRTOS.h"
  #include "FreeRTOS.h" /* for vTaskDelay() */
  #include "task.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#if MCUC1_CONFIG_SDK_VERSION_USED == MCUC1_CONFIG_SDK_PROCESSOR_EXPERT
  #define WAIT2_INSTR_CLOCK_HZ       CPU_CORE_CLK_HZ /* for Kinetis, use core clock as base for instruction execution */
#else
  extern uint32_t SystemCoreClock; /* clock frequency variable defined system_<device>.h of the SDK */
  #define WAIT2_INSTR_CLOCK_HZ       SystemCoreClock  /* core clock frequency in Hz */
#endif
#define WAIT2_NofCyclesMs(ms, hz)  ((ms)*((hz)/1000)) /* calculates the needed cycles based on bus clock frequency */
#define WAIT2_NofCyclesUs(us, hz)  ((us)*(((hz)/1000)/1000)) /* calculates the needed cycles based on bus clock frequency */
#define WAIT2_NofCyclesNs(ns, hz)  (((ns)*(((hz)/1000)/1000))/1000) /* calculates the needed cycles based on bus clock frequency */

#define WAIT2_WAIT_C(cycles) \
     ( (cycles)<=10 ? \
          WAIT2_Wait10Cycles() \
        : WAIT2_WaitCycles((uint16_t)cycles) \
      )                                      /*!< wait for some cycles */


void WAIT2_Wait10Cycles(void);
/*
** ===================================================================
**     Method      :  WAIT2_Wait10Cycles (component Wait)
**     Description :
**         Wait for 10 CPU cycles.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void WAIT2_Wait100Cycles(void);
/*
** ===================================================================
**     Method      :  WAIT2_Wait100Cycles (component Wait)
**     Description :
**         Wait for 100 CPU cycles.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void WAIT2_WaitCycles(uint16_t cycles);
/*
** ===================================================================
**     Method      :  WAIT2_WaitCycles (component Wait)
**     Description :
**         Wait for a specified number of CPU cycles (16bit data type).
**     Parameters  :
**         NAME            - DESCRIPTION
**         cycles          - The number of cycles to wait.
**     Returns     : Nothing
** ===================================================================
*/

void WAIT2_Waitms(uint16_t ms);
/*
** ===================================================================
**     Method      :  WAIT2_Waitms (component Wait)
**     Description :
**         Wait for a specified time in milliseconds.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ms              - How many milliseconds the function has to
**                           wait
**     Returns     : Nothing
** ===================================================================
*/

/* we are having a static clock configuration: implement as macro/inlined version */
#define WAIT2_Waitus(us)  \
        /*lint -save -e(505,506,522) Constant value Boolean, Redundant left argument to comma. */\
       (  ((WAIT2_NofCyclesUs((us),WAIT2_INSTR_CLOCK_HZ)==0)||(us)==0) ? \
          (void)0 : \
          ( ((us)/1000)==0 ? (void)0 : WAIT2_Waitms((uint16_t)((us)/1000))) \
          , (WAIT2_NofCyclesUs(((us)%1000), WAIT2_INSTR_CLOCK_HZ)==0) ? (void)0 : \
            WAIT2_WAIT_C(WAIT2_NofCyclesUs(((us)%1000), WAIT2_INSTR_CLOCK_HZ)) \
       /*lint -restore */\
       )
/*
** ===================================================================
**     Method      :  WAIT2_Waitus (component Wait)
**     Description :
**         Wait for a specified time in microseconds.
**     Parameters  :
**         NAME            - DESCRIPTION
**         us              - How many microseconds the function has to
**                           wait
**     Returns     : Nothing
** ===================================================================
*/

/* we are having a static clock configuration: implement as macro/inlined version */
#define WAIT2_Waitns(ns)  \
        /*lint -save -e(505,506,522) Constant value Boolean, Redundant left argument to comma. */\
       (  ((WAIT2_NofCyclesNs((ns), WAIT2_INSTR_CLOCK_HZ)==0)||(ns)==0) ? \
          (void)0 : \
          WAIT2_Waitus((ns)/1000) \
          , (WAIT2_NofCyclesNs((ns)%1000, WAIT2_INSTR_CLOCK_HZ)==0) ? \
              (void)0 : \
              WAIT2_WAIT_C(WAIT2_NofCyclesNs(((ns)%1000), WAIT2_INSTR_CLOCK_HZ)) \
       /*lint -restore */\
       )
/*
** ===================================================================
**     Method      :  WAIT2_Waitns (component Wait)
**     Description :
**         Wait for a specified time in nano seconds.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ns              - How many ns the function has to wait
**     Returns     : Nothing
** ===================================================================
*/

#if WAIT2_CONFIG_USE_RTOS_WAIT
  #define WAIT2_WaitOSms(ms) vTaskDelay(pdMS_TO_TICKS(ms)) /* use FreeRTOS API */
#else
  #define WAIT2_WaitOSms(ms)  WAIT2_Waitms(ms) /* use normal wait */
#endif
/*
** ===================================================================
**     Method      :  WAIT2_WaitOSms (component Wait)
**     Description :
**         If an RTOS is enabled, this routine will use a non-blocking
**         wait method. Otherwise it will do a busy/blocking wait.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void WAIT2_WaitLongCycles(uint32_t cycles);
/*
** ===================================================================
**     Method      :  WAIT2_WaitLongCycles (component Wait)
**     Description :
**         Wait for a specified number of CPU cycles (32bit data type).
**     Parameters  :
**         NAME            - DESCRIPTION
**         cycles          - The number of cycles to wait.
**     Returns     : Nothing
** ===================================================================
*/

void WAIT2_Init(void);
/*
** ===================================================================
**     Method      :  WAIT2_Init (component Wait)
**     Description :
**         Driver initialization routine.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void WAIT2_DeInit(void);
/*
** ===================================================================
**     Method      :  WAIT2_DeInit (component Wait)
**     Description :
**         Driver de-initialization routine
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END WAIT2. */

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif
/* ifndef __WAIT2_H */
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
