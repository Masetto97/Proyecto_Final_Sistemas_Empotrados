/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : Tiempo.h
**     Project     : Proyecto_Final_Kinetis
**     Processor   : MK64FN1M0VLQ12
**     Component   : GenericTimeDate
**     Version     : Component 01.061, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2021-04-01, 17:54, # CodeGen: 6
**     Abstract    :
**         Software date/time module.
**     Settings    :
**          Component name                                 : Tiempo
**          Software RTC                                   : Enabled
**            Tick Time (ms)                               : 1000
**            RTOS                                         : Disabled
**          Hardware RTC                                   : Disabled
**          Set Time and Date                              : 
**            Software RTC                                 : yes
**            Internal RTC                                 : yes
**            External RTC                                 : yes
**          Get Time and Date                              : Software RTC
**          Init()                                         : 
**            Defaults                                     : 
**              Time                                       : 17:51:31
**              Date                                       : 2012-07-22
**            Call Init() in startup                       : yes
**            Software RTC Initialization                  : Init from Defaults
**          System                                         : 
**            Critical Section                             : CS1
**            SDK                                          : MCUC1
**            Shell                                        : Disabled
**     Contents    :
**         AddTick                     - void Tiempo_AddTick(void);
**         AddTicks                    - void Tiempo_AddTicks(uint16_t nofTicks);
**         TicksToTime                 - uint8_t Tiempo_TicksToTime(uint32_t ticks, TIMEREC *time);
**         TimeToTicks                 - uint8_t Tiempo_TimeToTicks(TIMEREC *time, uint32_t *ticks);
**         CalculateDayOfWeek          - uint8_t Tiempo_CalculateDayOfWeek(uint16_t Year, uint8_t Month, uint8_t Day);
**         SetTime                     - uint8_t Tiempo_SetTime(uint8_t Hour, uint8_t Min, uint8_t Sec, uint8_t Sec100);
**         GetTime                     - uint8_t Tiempo_GetTime(TIMEREC *time);
**         SetDate                     - uint8_t Tiempo_SetDate(uint16_t Year, uint8_t Month, uint8_t Day);
**         GetDate                     - uint8_t Tiempo_GetDate(DATEREC *date);
**         SetTimeDate                 - uint8_t Tiempo_SetTimeDate(TIMEREC *time, DATEREC *date);
**         GetTimeDate                 - uint8_t Tiempo_GetTimeDate(TIMEREC *time, DATEREC *date);
**         SetSWTimeDate               - uint8_t Tiempo_SetSWTimeDate(TIMEREC *time, DATEREC *date);
**         GetSWTimeDate               - uint8_t Tiempo_GetSWTimeDate(TIMEREC *time, DATEREC *date);
**         SetInternalRTCTimeDate      - uint8_t Tiempo_SetInternalRTCTimeDate(TIMEREC *time, DATEREC *date);
**         GetInternalRTCTimeDate      - uint8_t Tiempo_GetInternalRTCTimeDate(TIMEREC *time, DATEREC *date);
**         SyncWithInternalRTC         - uint8_t Tiempo_SyncWithInternalRTC(void);
**         SyncSWtimeToInternalRTCsec  - uint8_t Tiempo_SyncSWtimeToInternalRTCsec(void);
**         SetExternalRTCTimeDate      - uint8_t Tiempo_SetExternalRTCTimeDate(TIMEREC *time, DATEREC *date);
**         GetExternalRTCTimeDate      - uint8_t Tiempo_GetExternalRTCTimeDate(TIMEREC *time, DATEREC *date);
**         SyncWithExternalRTC         - uint8_t Tiempo_SyncWithExternalRTC(void);
**         UnixSecondsToTimeDateCustom - void Tiempo_UnixSecondsToTimeDateCustom(int32_t seconds, int8_t offset_hours,...
**         UnixSecondsToTimeDate       - void Tiempo_UnixSecondsToTimeDate(int32_t seconds, int8_t offset_hours,...
**         TimeDateToUnixSecondsCustom - int32_t Tiempo_TimeDateToUnixSecondsCustom(TIMEREC *time, DATEREC *date,...
**         TimeDateToUnixSeconds       - int32_t Tiempo_TimeDateToUnixSeconds(TIMEREC *time, DATEREC *date, int8_t...
**         AddDateString               - uint8_t Tiempo_AddDateString(uint8_t *buf, size_t bufSize, DATEREC *date,...
**         AddTimeString               - uint8_t Tiempo_AddTimeString(uint8_t *buf, size_t bufSize, TIMEREC *time,...
**         DeInit                      - void Tiempo_DeInit(void);
**         Init                        - uint8_t Tiempo_Init(void);
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
** @file Tiempo.h
** @version 01.00
** @brief
**         Software date/time module.
*/         
/*!
**  @addtogroup Tiempo_module Tiempo module documentation
**  @{
*/         

#ifndef __Tiempo_H
#define __Tiempo_H

/* MODULE Tiempo. */
#include "MCUC1.h" /* SDK and API used */
#include "Tiempoconfig.h" /* configuration */

/* Include inherited components */
#include "CS1.h"
#include "UTIL1.h"
#include "MCUC1.h"


/* if command line parser is enabled */
#define Tiempo_PARSE_COMMAND_ENABLED  0 /* set to 1 if method ParseCommand() is present, 0 otherwise */

/* if Init() is called durint startup */
#define Tiempo_INIT_IN_STARTUP                              1 /* 1: call Init() during startup, 0: application needs to call Init() */

/* RTC Initialization options during Init() */
#define Tiempo_INIT_SOFTWARE_RTC_FROM_DEFAULTS              0  /* init software RTC from default values */
#define Tiempo_INIT_SOFTWARE_RTC_FROM_INTERNAL_RTC          1  /* init software RTC from internal RTC values */
#define Tiempo_INIT_SOFTWARE_RTC_FROM_EXTERNAL_RTC          2  /* init software RTC from external RTC values */

/* settings for software RTC */
#define Tiempo_USE_SOFTWARE_RTC                             1  /* set to 1 if using software RTC, 0 otherwise */
#define Tiempo_INIT_SOFTWARE_RTC_METHOD                     Tiempo_INIT_SOFTWARE_RTC_FROM_DEFAULTS /* which method to use during Init() */

/* settings for internal hardware RTC */
#define Tiempo_USE_INTERNAL_HW_RTC                          0  /* set to 1 if using internal HW RTC, 0 otherwise */
#define Tiempo_USE_INTERNAL_HW_RTC_BEAN                     0  /* set to 1 if using HW RTC using normal bean driver, 0 otherwise */
#define Tiempo_USE_INTERNAL_HW_RTC_LDD                      0  /* set to 1 if using HW RTC using LDD driver, 0 otherwise */

#define Tiempo_USE_EXTERNAL_HW_RTC                          0  /* set to 1 if using external HW RTC driver, 0 otherwise */

#if Tiempo_USE_INTERNAL_HW_RTC_BEAN
  #define Tiempo_HAS_SEC100_IN_TIMEREC                      0 /* Bean version have no Sec100!  */
#else
  #define Tiempo_HAS_SEC100_IN_TIMEREC                      1 /* non-Bean version have Sec100  */
#endif

/* SetTime() and SetDate() configuration */
#define Tiempo_SET_TIME_DATE_METHOD_USES_SOFTWARE_RTC       1 /* 1: Enable setting software RTC time/date */
#define Tiempo_SET_TIME_DATE_METHOD_USES_INTERNAL_RTC       1 /* 1: Enable setting internal RTC time/date */
#define Tiempo_SET_TIME_DATE_METHOD_USES_EXTERNAL_RTC       1 /* 1: Enable setting external RTC time/date */

/* GetTime() and GetDate() configuration */
#define Tiempo_GET_TIME_DATE_METHOD_SOFTWARE_RTC            1 /* use software RTC */
#define Tiempo_GET_TIME_DATE_METHOD_INTERNAL_RTC            2 /* use internal RTC */
#define Tiempo_GET_TIME_DATE_METHOD_EXTERNAL_RTC            3 /* use external RTC */
#define Tiempo_USE_GET_TIME_DATE_METHOD                     Tiempo_GET_TIME_DATE_METHOD_SOFTWARE_RTC /* specifies method to get time and date */

/* default time and date format strings */
#define Tiempo_DEFAULT_TIME_FORMAT_STR  "hh:mm:ss,cc"
#define Tiempo_DEFAULT_DATE_FORMAT_STR  "dd.mm.yyyy"

/* user events */
#define Tiempo_ON_DATE_GET_EVENT                            0 /* 1: enabled user event */
#define Tiempo_ON_DATE_GET_EVENT_NAME                       Tiempo_OnDateGet /* name of user event*/
#define Tiempo_ON_TIME_GET_EVENT                            0 /* 1: enabled user event */
#define Tiempo_ON_TIME_GET_EVENT_NAME                       Tiempo_OnTimeGet /* name of user event*/
#define Tiempo_ON_DATE_SET_EVENT                            0 /* 1: enabled user event */
#define Tiempo_ON_DATE_SET_EVENT_NAME                       Tiempo_OnDateSet /* name of user event*/
#define Tiempo_ON_TIME_SET_EVENT                            0 /* 1: enabled user event */
#define Tiempo_ON_TIME_SET_EVENT_NAME                       Tiempo_OnTimeSet /* name of user event*/


#ifndef __BWUserType_TIMEREC
#define __BWUserType_TIMEREC
  typedef struct {                     /* It contains actual number of hours, minutes, seconds and hundreth of seconds. */
    uint8_t Hour;                      /* hours (0 - 23) */
    uint8_t Min;                       /* minutes (0 - 59) */
    uint8_t Sec;                       /* seconds (0 - 59) */
  #if Tiempo_HAS_SEC100_IN_TIMEREC /* does not exist for Bean version */
    uint8_t Sec100;                    /* hundredth of seconds (0 - 99) */
  #endif
  } TIMEREC;
#endif
#ifndef __BWUserType_DATEREC
#define __BWUserType_DATEREC
  typedef struct {                     /* It contains actual year, month, and day description. */
    uint16_t Year;                     /* years (1998 - 2099) */
    uint8_t Month;                     /* months (1 - 12) */
    uint8_t Day;                       /* days (1 - 31) */
  } DATEREC;
#endif

/* default time/date values */
static const TIMEREC Tiempo_DefaultTime = {
  17, /* hour */
  51,  /* minute */
  31,  /* second */
#if Tiempo_HAS_SEC100_IN_TIMEREC
  0 /* h-second */
#endif
};
static const DATEREC Tiempo_DefaultDate = {
  2012, /* year */
  7,  /* month */
  22 /* day */
};

#define Tiempo_TICK_TIME_MS \
  1000                                  /* Period in milliseconds as defined in component properties, at which Tiempo._AddTick() is called */





uint8_t Tiempo_SetTime(uint8_t Hour, uint8_t Min, uint8_t Sec, uint8_t Sec100);
/*
** ===================================================================
**     Method      :  Tiempo_SetTime (component GenericTimeDate)
**     Description :
**         This method sets a new actual time of the software RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Hour            - Hours (0 - 23)
**         Min             - Minutes (0 - 59)
**         Sec             - Seconds (0 - 59)
**         Sec100          - Hundredth of seconds (0 - 99)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void Tiempo_AddTick(void);
/*
** ===================================================================
**     Method      :  Tiempo_AddTick (component GenericTimeDate)
**     Description :
**         Increments the tick counter of the software RTC. Needs to be
**         called periodically by the application to increase the time
**         tick count.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

uint8_t Tiempo_GetTime(TIMEREC *time);
/*
** ===================================================================
**     Method      :  Tiempo_GetTime (component GenericTimeDate)
**     Description :
**         This method returns current time from the software RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to the structure TIMEREC. It
**                           contains actual number of hours, minutes,
**                           seconds and hundredth of seconds.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_SetDate(uint16_t Year, uint8_t Month, uint8_t Day);
/*
** ===================================================================
**     Method      :  Tiempo_SetDate (component GenericTimeDate)
**     Description :
**         This method sets a new actual date of the software RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Year            - Years (16-bit unsigned integer)
**         Month           - Months (8-bit unsigned integer)
**         Day             - Days (8-bit unsigned integer)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_GetDate(DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_GetDate (component GenericTimeDate)
**     Description :
**         This method returns current date from the software RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * date            - Pointer to DATEREC
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_Init(void);
/*
** ===================================================================
**     Method      :  Tiempo_Init (component GenericTimeDate)
**     Description :
**         Initialization method
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void Tiempo_DeInit(void);
/*
** ===================================================================
**     Method      :  Tiempo_DeInit (component GenericTimeDate)
**     Description :
**         Deinitializes the driver.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Tiempo_AddTicks(uint16_t nofTicks);
/*
** ===================================================================
**     Method      :  Tiempo_AddTicks (component GenericTimeDate)
**     Description :
**         Same as AddTick(), but multiple ticks can be added in one
**         step.
**     Parameters  :
**         NAME            - DESCRIPTION
**         nofTicks        - Number of ticks to be added
**     Returns     : Nothing
** ===================================================================
*/

uint8_t Tiempo_TicksToTime(uint32_t ticks, TIMEREC *time);
/*
** ===================================================================
**     Method      :  Tiempo_TicksToTime (component GenericTimeDate)
**     Description :
**         Transforms ticks into time information
**     Parameters  :
**         NAME            - DESCRIPTION
**         ticks           - 
**       * time            - Pointer where to store the time
**                           information
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_TimeToTicks(TIMEREC *time, uint32_t *ticks);
/*
** ===================================================================
**     Method      :  Tiempo_TimeToTicks (component GenericTimeDate)
**     Description :
**         Transforms time information into ticks
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer where to time information
**       * ticks           - Pointer to where to store the ticks
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_SetInternalRTCTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_SetInternalRTCTimeDate (component GenericTimeDate)
**     Description :
**         This method sets a new actual time and date of the internal
**         hardware RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to struct which contains the
**                           time to be set. Can be NULL if time shall
**                           not be set.
**       * date            - Pointer to struct which contains the
**                           date information to be set. Can be NULL if
**                           date shall not be set.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_GetInternalRTCTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_GetInternalRTCTimeDate (component GenericTimeDate)
**     Description :
**         This method returns current time and date from the hardware
**         RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to the structure to return the
**                           time. Can be NULL.
**       * date            - Pointer to structure which returns the
**                           date information. Can be NULL
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_CalculateDayOfWeek(uint16_t Year, uint8_t Month, uint8_t Day);
/*
** ===================================================================
**     Method      :  Tiempo_CalculateDayOfWeek (component GenericTimeDate)
**     Description :
**         Returns the day of the week, Sunday starting as zero. Monday
**         is 1, ...
**     Parameters  :
**         NAME            - DESCRIPTION
**         Year            - Years (16-bit unsigned integer)
**         Month           - Months (8-bit unsigned integer)
**         Day             - Days (8-bit unsigned integer)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_SetSWTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_SetSWTimeDate (component GenericTimeDate)
**     Description :
**         This method sets a new actual time and date of the software
**         RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to struct which contains the
**                           time to be set. Can be NULL if time shall
**                           not be set.
**       * date            - Pointer to struct which contains the
**                           date information to be set. Can be NULL if
**                           date shall not be set.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_GetSWTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_GetSWTimeDate (component GenericTimeDate)
**     Description :
**         This method returns current time and date from the software
**         RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to the structure to return the
**                           time. Can be NULL.
**       * date            - Pointer to structure which returns the
**                           date information. Can be NULL
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_SyncWithInternalRTC(void);
/*
** ===================================================================
**     Method      :  Tiempo_SyncWithInternalRTC (component GenericTimeDate)
**     Description :
**         Synchronizes the software RTC with date and time from the
**         internal hardware RTC
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_SyncWithExternalRTC(void);
/*
** ===================================================================
**     Method      :  Tiempo_SyncWithExternalRTC (component GenericTimeDate)
**     Description :
**         Synchronizes the software RTC with date and time from the
**         hardware RTC. Note that if that RTC interface requires
**         interrupts, this function should be called only when
**         interrupts are enabled.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_SetExternalRTCTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_SetExternalRTCTimeDate (component GenericTimeDate)
**     Description :
**         This method sets a new actual time and date of the external
**         hardware RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to struct which contains the
**                           time to be set. Can be NULL if time shall
**                           not be set.
**       * date            - Pointer to struct which contains the
**                           date information to be set. Can be NULL if
**                           date shall not be set.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_GetExternalRTCTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_GetExternalRTCTimeDate (component GenericTimeDate)
**     Description :
**         This method returns current time and date from the external
**         hardware RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to the structure to return the
**                           time. Can be NULL.
**       * date            - Pointer to structure which returns the
**                           date information. Can be NULL
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_SetTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_SetTimeDate (component GenericTimeDate)
**     Description :
**         This method sets a new actual time and date of the RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to struct which contains the
**                           time to be set. Can be NULL if time shall
**                           not be set.
**       * date            - Pointer to struct which contains the
**                           date information to be set. Can be NULL if
**                           date shall not be set.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t Tiempo_GetTimeDate(TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_GetTimeDate (component GenericTimeDate)
**     Description :
**         This method returns current time and date from the RTC.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to the structure to return the
**                           time. Can be NULL.
**       * date            - Pointer to structure which returns the
**                           date information. Can be NULL
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void Tiempo_UnixSecondsToTimeDateCustom(int32_t seconds, int8_t offset_hours, TIMEREC *time, DATEREC *date, uint16_t baseYear);
/*
** ===================================================================
**     Method      :  Tiempo_UnixSecondsToTimeDateCustom (component GenericTimeDate)
**     Description :
**         Transforms a given time/date into the Unix time stamp, with
**         the number of seconds from a starting date
**     Parameters  :
**         NAME            - DESCRIPTION
**         seconds         - Unix time stamp in seconds after
**                           the given base year and base month
**         offset_hours    - Optional time zone
**                           offset, use 0 for no offset
**       * time            - Pointer to TIMEREC struct where the
**                           result is stored. Can be NULL.
**       * date            - Pointer to DATEREC struct where the
**                           result is stored. Can be NULL.
**         baseYear        - Base year from which is used as
**                           base to count the seconds, in YYYY format.
**     Returns     : Nothing
** ===================================================================
*/

int32_t Tiempo_TimeDateToUnixSecondsCustom(TIMEREC *time, DATEREC *date, int8_t offset_hours, uint16_t baseYear);
/*
** ===================================================================
**     Method      :  Tiempo_TimeDateToUnixSecondsCustom (component GenericTimeDate)
**     Description :
**         Returns for a given time/date the corresponding UNIX time
**         stamp with a custom base date.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to TIMEREC which holds the time
**                           to convert
**       * date            - Pointer to DATEREC with the date to
**                           convert
**         offset_hours    - Optional number of
**                           offset hours to adjust for the time zone.
**                           Use 0 for no adjustment.
**         baseYear        - Base year from which is used as
**                           base to count the seconds, in YYYY format
**     Returns     :
**         ---             - number of seconds after given base
**                           month/year
** ===================================================================
*/

void Tiempo_UnixSecondsToTimeDate(int32_t seconds, int8_t offset_hours, TIMEREC *time, DATEREC *date);
/*
** ===================================================================
**     Method      :  Tiempo_UnixSecondsToTimeDate (component GenericTimeDate)
**     Description :
**         Transforms a given time/date into the Unix time stamp, with
**         the number of seconds after 1-Jan-1970
**     Parameters  :
**         NAME            - DESCRIPTION
**         seconds         - Unix time stamp in seconds after
**                           the given base year and base month
**         offset_hours    - Optional time zone
**                           offset, use 0 for no offset
**       * time            - Pointer to TIMEREC struct where the
**                           result is stored. Can be NULL.
**       * date            - Pointer to DATEREC struct where the
**                           result is stored. Can be NULL.
**     Returns     : Nothing
** ===================================================================
*/

int32_t Tiempo_TimeDateToUnixSeconds(TIMEREC *time, DATEREC *date, int8_t offset_hours);
/*
** ===================================================================
**     Method      :  Tiempo_TimeDateToUnixSeconds (component GenericTimeDate)
**     Description :
**         Returns for a given time/date the corresponding UNIX time
**         stamp, starting at 1-Jan-1970.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * time            - Pointer to TIMEREC which holds the time
**                           to convert
**       * date            - Pointer to DATEREC with the date to
**                           convert
**         offset_hours    - Optional number of
**                           offset hours to adjust for the time zone.
**                           Use 0 for no adjustment.
**     Returns     :
**         ---             - number of seconds after given base
**                           month/year
** ===================================================================
*/

uint8_t Tiempo_AddDateString(uint8_t *buf, size_t bufSize, DATEREC *date, uint8_t *format);
/*
** ===================================================================
**     Method      :  Tiempo_AddDateString (component GenericTimeDate)
**     Description :
**         Adds a formatted date string to a buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**       * buf             - Pointer to zero terminated buffer where to
**                           append the string
**         bufSize         - Size of the buffer in bytes
**       * date            - Pointer to date information
**       * format          - Pointer to the format string.
**                           Supported formats: "dd.mm.yyyy"
**     Returns     :
**         ---             - Error code, ERR_OK for no error
** ===================================================================
*/

uint8_t Tiempo_AddTimeString(uint8_t *buf, size_t bufSize, TIMEREC *time, uint8_t *format);
/*
** ===================================================================
**     Method      :  Tiempo_AddTimeString (component GenericTimeDate)
**     Description :
**         Adds a formatted time string to a buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**       * buf             - Pointer to zero terminated buffer where to
**                           append the string
**         bufSize         - Size of the buffer in bytes
**       * time            - Pointer to time information
**       * format          - Pointer to the format string.
**                           Supported formats: "hh:mm.ss,cc"
**     Returns     :
**         ---             - Error code, ERR_OK for no error
** ===================================================================
*/

uint8_t Tiempo_SyncSWtimeToInternalRTCsec(void);
/*
** ===================================================================
**     Method      :  Tiempo_SyncSWtimeToInternalRTCsec (component GenericTimeDate)
**     Description :
**         This method synchronizes the software RTC with the internal
**         HW RTC. Because the internal RTC only counts seconds, we
**         sync on a second change.
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

/* END Tiempo. */

#endif
/* ifndef __Tiempo_H */
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
