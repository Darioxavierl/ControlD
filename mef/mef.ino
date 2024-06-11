#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "pulsantes.h"
#include "pulsantes.c"

//Declaracion de botones

pulsantes botones;

uint8_t initBotones(){
    botones.delay = DEBOUNCE_DELAY_MS;
    botones.numero_pulsadores = NUM_PUL;

    for (int i = 0; i < NUM_PUL; ++i) {
        // Definiendo espacio de memoria para el arreglo de pines
        botones.pins[i] = (int*)malloc(sizeof(int));
        // Definiendo espacio de memoria para el arreglo de estado del MEF de cada boton y asignando estado inicial
        botones.estadoActual[i] = (estado_t*)malloc(sizeof(estado_t));
        *(botones.estadoActual[i]) = ESTADO_UP;
        // Definiendo espacio de memoria para el arreglo de contadores para cada pin y asignando el valor del contador
        botones.debounceCounter[i] = (uint16_t*)malloc(sizeof(uint16_t));
        *(botones.debounceCounter[i]) = botones.delay;
        // Definiendo espacio de memoria para el arreglo de las flag de presionado para cada pin y asignando el valor por defecto 0 
        botones.presionado[i] = (uint8_t*)malloc(sizeof(uint8_t));
        *(botones.presionado[i]) = 0;
        // Definiendo espacio de memoria para el arreglo de las flag de liberado para cada pin y asignando el valor por defecto 0
        botones.liberado[i] = (uint8_t*)malloc(sizeof(uint8_t));
        *(botones.liberado[i]) = 0;
        // Definiendo espacio de memoria para el arreglo que guarda las lecturas de los pines digitales y asignando por defecto 0 
        botones.val[i] = (uint8_t*)malloc(sizeof(uint8_t));
        *(botones.val[i]) = 0;

        // Manejo de errores en la asignacion de memoria 
        if (botones.pins[i] == NULL || botones.estadoActual[i] == NULL || botones.debounceCounter[i] == NULL) {
            // Error: No se pudo asignar memoria
            return 0;
        }
    }

    *(botones.pins[0]) = SW1;
    *(botones.pins[1]) = SW2;

    return 1;
}

uint8_t setupTimer1() {
    // Deshabilitar interrupciones globales mientras se configura el timer
    cli(); // Se limpia (deshabilita) la interrupción global
    // Configurar el Timer1 en modo CTC (Clear Timer on Compare Match)
    TCCR1A = 0; // Borra todos los bits de control de salida
    TCCR1B = 0; // Borra todos los bits de control de timer

    // Configurar el prescaler a 64
    TCCR1B |= (1 << CS11) | (1 << CS10); // Configurar el prescaler a 64

    // Configurar el modo CTC
    TCCR1B |= (1 << WGM12); // Modo CTC, OCR1A como tope

    // Establecer el valor de comparación (OCR1A) para generar la frecuencia deseada
    OCR1A = 249; // (16MHz / (prescaler * frecuencia_deseada)) - 1
                 // Para una frecuencia de interrupción de 1000 Hz, se utiliza un prescaler de 64
                 // Entonces, el valor de OCR1A se calcula como (16000000 / (64 * 1000)) - 1 = 249

    // Habilitar la interrupción de comparación de salida A (OCIE1A)
    TIMSK1 |= (1 << OCIE1A); // Habilitar la interrupción de comparación de salida A

    // Habilitar nuevamente las interrupciones globales
    sei(); // Se establece (habilita) la interrupción global
    return 1;
}

ISR(TIMER1_COMPA_vect) {
    // Cambiar el estado del LED en el pin 13
    digitalWrite(13, !digitalRead(13));

    for (uint8_t i = 0; i < botones.numero_pulsadores ; i++)
    {
        if ((*(botones.estadoActual[i]) == ESTADO_FALLING) || (*(botones.estadoActual[i]) == ESTADO_RISING))
        {
            *(botones.debounceCounter[i]) =  (*(botones.debounceCounter[i]) > 0) ? (*(botones.debounceCounter[i])-1) : botones.delay;
        } else{
            *(botones.debounceCounter[i]) = botones.delay;
        }
        
    }
    


}

void setup(){
    Serial.begin(115200);
    delay(50);
    //Inicializacion de botones
    uint8_t bt = initBotones() ? Serial.println(GOOD_INIT_BUTTONS) : Serial.println(BAD_INIT_BUTTONS) ;
    delay(100);
    // Inicializacion del timer

    uint8_t tm = setupTimer1() ? Serial.println(GOOD_INIT_TIMER) : Serial.println(BAD_INIT_TIMER);
    delay(100);

    // Inicio de los pines de los pulsadores
    for (int i = 0; i < botones.numero_pulsadores; i++) {
        pinMode(*(botones.pins[i]), INPUT_PULLUP);
    }
    
    
}

void loop() {
    for (int i = 0; i < NUM_PUL; ++i) {
        int value = digitalRead(*(botones.pins[i]));
        if(!value){
            Serial.println(value);
        }
    }
}