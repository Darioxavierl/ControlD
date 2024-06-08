#include <stdio.h>
#include <stdbool.h>
#include "pulsantes.h"
#include <Arduino.h>


estado_t estadoActuale;


void init_MEF(pulsantes* pulsadores){
    for (int i = 0; i < pulsadores->numero_pulsadores; i++)
    {
        *(pulsadores->estadoActual[i]) = ESTADO_UP;
    }
      
}

int act_MEF(){

    int val = digitalRead(SW1);
    static int prevVal = 0;
    bool cambio = val ^ prevVal;
    switch (estadoActuale)
    {
    case ESTADO_UP:
        // inicializar hardware
        estadoActuale = val ? ESTADO_UP : ESTADO_DOWN;
        return 1;
        break;
    case ESTADO_FALLING:
        //Serial.println("Estado up");
        return 2;
        break;
    case ESTADO_DOWN:

        return 3;
        break;
    case ESTADO_RISING:
        return 4;
        break;
    default:
        //init_MEF();
        break;
    }
}