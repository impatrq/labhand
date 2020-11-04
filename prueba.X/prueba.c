/*
 * File:   prueba.c
 * Author: Mati
 *
 * Created on October 24, 2020, 5:35 PM
 */


#include <xc.h>
#include "Configuracion.h"

void Timer0_Init(void);
float  por_adc;
unsigned int value1 = 0;
int tmr,DUTY_CALCULADO;

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
        PORTB = value1;
        por_adc = (100*value1)/1023;
        tmr = (por_adc*625)/100;
        LATCbits.LATC0 = 1;
        DUTY_CALCULADO = 54285 - tmr;
        DUTY_CALCULADO = 700;
        TMR0L = DUTY_CALCULADO;
        TMR0H = DUTY_CALCULADO>>8;
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
        if(PORTDbits.RD4){
            LATDbits.LATD1 = 1;
            __delay_ms(2);
            LATDbits.LATD1 = 0;
            TMR0L = 0x0D;
            TMR0H = 0xD4;
            INTCONbits.TMR0IF = 0;
        }
        else{
            LATDbits.LATD1 = 1;
            __delay_ms(1);
            LATDbits.LATD1 = 0;
            TMR0L = 0x9C;
            TMR0H = 0xD1;
            INTCONbits.TMR0IF = 0;
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