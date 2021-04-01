/*
 * UART.c
 *
 *  Created on: 17/12/2014
 *      Author: Alejandro
 */
#include "PuertoSerie.h"
#include "UART.h"


void UART_Write_Numero_Int(unsigned int numero)
{
	while(PuertoSerie_SendChar((char)(48+(numero/10000)))!= ERR_OK) {};
	while(PuertoSerie_SendChar((char)(48+(numero%10000)/1000))!= ERR_OK) {};
	while(PuertoSerie_SendChar((char)(48+(numero%1000)/100))!= ERR_OK) {};
	while(PuertoSerie_SendChar((char)(48+(numero%100)/10))!= ERR_OK) {};
	while(PuertoSerie_SendChar((char)(48+(numero%10)))!= ERR_OK) {};
}

