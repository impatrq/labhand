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

/*#include <xc.h>
#include "Configuracion.h"

void Timer0_Init(void);
float  por_adc;
long int value1 = 0;
static long int DUTY_CALCULADO;
long int tmr;
void main(void) {
    ADCON0bits.ADON = 1;
    ADCON0bits.CHS = 1;
    ADCON2bits.ACQT = 2;
    ADCON2bits.ADFM = 1;
    TRISD = 0xf0;
    TRISB = 0x00;
    TRISA = 0xff;
    LATD = 0;
    TMR0L = 0x2B;
    TMR0H = 0xCF;
    Timer0_Init();
    while(1){
        ADCON0bits.GO_DONE = 1; 
        while (ADCON0bits.GO_DONE);
        value1 = ADRESH;
        value1 = (value1 << 8) | ADRESL;

        por_adc = (100*value1)/1023;
 
        tmr = (por_adc*625)/100;
        
        DUTY_CALCULADO = 54285 - tmr;
       
        
    }
}

void Timer0_Init(void){
    INTCONbits.GIE = 0;
    T0CONbits.T0CS = 0; // Temporizador
    T0CONbits.PSA = 0; // Habilitamos Pre escaler
    T0CONbits.T0PS = 0b010; //8 Pre escaler
    T0CONbits.T08BIT = 0; // 16 bits de cuenta 
    INTCONbits.TMR0IF = 0; //Flag del Timer 0 apagado
    INTCONbits.TMR0IE = 1; // Permitir el desbordamiento del Timer0
    INTCONbits.PEIE = 1; // Interrupciones de perifericos
    INTCONbits.GIE = 1;
    T0CONbits.TMR0ON = 1; // Tmr0 empieza
}

void __interrupt() Timer_INT(void){
    
    if(INTCONbits.TMR0IF){
        LATDbits.LATD1 = 1;
        if(DUTY_CALCULADO != 0){
           
            __delay_ms(2);
            LATDbits.LATD1 = 0;
            TMR0L = DUTY_CALCULADO;
            TMR0H = DUTY_CALCULADO>>8;
            INTCONbits.TMR0IF = 0;
            PORTB = DUTY_CALCULADO;
        }
    }
}


/* ADCON0bits.ADON = 1;
    ADCON0bits.CHS = 1;
    ADCON2bits.ACQT = 2;
    ADCON2bits.ADFM = 1;
    while(1){
        ADCON0bits.GO_DONE = 1; 
        while (ADCON0bits.GO_DONE);
        value1 = ADRESH;
        value1 = (value1 << 8) | ADRESL;
        por_adc = (100*value1)/1023;
        tmr = (por_adc*625)/100;
        LATBbits.LATB0 = 1;
        DUTY_CALCULADO = 54285 - tmr;
        DUTY_CALCULADO = 700;
        TMR0L = DUTY_CALCULADO;
        TMR0H = DUTY_CALCULADO>>8;*/