#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>

#include "pulsantes.h"
#include "pulsantes.c"




void setup(){
// Inicializacion del pulsador
init_MEF();
Serial.begin(115200);
}

void loop() {
    int ret = act_MEF();

    switch (ret)
    {
    case 1:
        Serial.println("estado inicial");
        break;
    case 2:
        Serial.println("estado down");
        break;
    case 3:
        Serial.println("estado inicial");
        break;
    default:
        Serial.println("estado inicial");
        break;
    }
}