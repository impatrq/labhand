/*
 * File:   main.c
 * Author: Mati
 *
 * Created on October 21, 2020, 3:39 PM
 */


#include <xc.h>
#include "Configuracion.h"
#include "USART_libreria.h"

void main(void) {
    USART_Init(9600);
    TRISCbits.RC7 = 0; //Salida RD7
    while(1){
        char valor = USART_Rx();
        if(valor == '1'){
            LATCbits.LC7 = 1;
        }
        else if(valor == '2'){
            LATCbits.LC7 = 0;
        }
    }
}
