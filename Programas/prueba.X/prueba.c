/*
 * File:   prueba.c
 * Author: Mati
 *
 * Created on October 24, 2020, 5:35 PM
 */

#include <xc.h>
#pragma config FOSC = INTOSCIO_EC
#pragma config FCMEN = OFF
#pragma config IESO = ON
#define _XTAL_FREQ 20000000
short servo;

short getCAD0 (){
	ADCON1 = 0b00001110;
	ADCON0 = 0b00000000;
	ADCON2 = 0b00100101;
	ADCON0bits.ADON = 1;
	ADCON0bits.GO_DONE = 1;
	while (ADCON0bits.GO_DONE==1){
	}
	return ADRESH;
	}

void T0_servoOn(){
	INTCON = 0b10100000;
	T0CON = 0b11000100;
    servo = servo / 4;
	TMR0L = 210 - servo;
	}

void T0_servoOff(){
	INTCON = 0b10100000;
	T0CON = 0b11000111;
	servo = servo * 9; // investigar
	//TMR0 = 18036 + servo;
	//TMR0L = 6;
    TMR0L = 108; //1ms
    TMR0H = 100;
	}

void servocontrol (){
	if(PORTCbits.RC2==1){
		PORTCbits.RC2 = 0;
		T0_servoOff();
	}
	else{
		PORTCbits.RC2 = 1;	
		T0_servoOn();
	}
}

void __interrupt() Interrupciones (void){
	INTCONbits.GIE = 0;
	INTCONbits.T0IF = 0;
	servocontrol();
}

void main (void) {
	OSCCON = 0b01110000;
	TRISAbits.RA0=1;
	TRISCbits.RC2=0;
	servo=128;
	servocontrol();
	while(1){
		servo=getCAD0();
		
	}
	return;
	}
