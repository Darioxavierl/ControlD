#include <stdio.h>
#include <stdbool.h>
#include "pulsantes.h"
#include <Arduino.h>



void init_MEF(pulsantes* pulsadores){
    for (int i = 0; i < pulsadores->numero_pulsadores; i++)
    {
        *(pulsadores->estadoActual[i]) = ESTADO_UP;
    }
      
}

void act_MEF(pulsantes* pulsadores){

     for (uint8_t i = 0; i < pulsadores->numero_pulsadores; i++)
    {   
        switch (*(pulsadores->estadoActual[i]))
        {
        case ESTADO_UP:
            // inicializar hardware
            *(pulsadores->val[i]) = digitalRead(*(pulsadores->pins[i]));
            *(pulsadores->estadoActual[i]) = *(pulsadores->val[i]) ? ESTADO_FALLING : ESTADO_UP;
            *(pulsadores->presionado[i]) = *(pulsadores->val[i]) ? 1:0;
            //return 1;
            break;
        case ESTADO_FALLING:
            //Serial.println("Estado up");
            if (*(pulsadores->debounceCounter[i])== 0)
            {
                *(pulsadores->val[i]) = digitalRead(*(pulsadores->pins[i]));
                *(pulsadores->estadoActual[i]) = *(pulsadores->presionado[i]) ^ *(pulsadores->val[i]) ? ESTADO_UP : ESTADO_DOWN;
            }
            //return 2;
            break;
        case ESTADO_DOWN:
            *(pulsadores->val[i]) = digitalRead(*(pulsadores->pins[i]));
            *(pulsadores->estadoActual[i]) = *(pulsadores->val[i]) == 0 ? ESTADO_RISING : ESTADO_DOWN;
            *(pulsadores->liberado[i]) = *(pulsadores->val[i]) == 0 ? 1:0;
            //return 3;
            break;
        case ESTADO_RISING:
            if (*(pulsadores->debounceCounter[i])== 0)
            {
                *(pulsadores->val[i]) = digitalRead(*(pulsadores->pins[i]));
                *(pulsadores->estadoActual[i]) = *(pulsadores->presionado[i]) ^ *(pulsadores->val[i]) ? ESTADO_UP : ESTADO_DOWN;   
            }
        
            //return 4;
            break;
        default:
            //init_MEF();
            break;
        }
        
    }
    
}