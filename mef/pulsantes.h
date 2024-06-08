#ifndef pulsantes_H
#define pulsantes_H

#include <Arduino.h>
#include <stdbool.h>

#define SW1 3 // Pin del boton
#define SW2 4 // Pin del boton
#define DEBOUNCE_DELAY_MS 40 // Delay de debounce para los botones
#define NUM_PUL 2
#define GOOD_INIT_BUTTONS "Inicio botones OK"
#define BAD_INIT_BUTTONS "Inicio botones fail"
#define GOOD_INIT_TIMER "Inicio timer1 OK"
#define BAD_INIT_TIMER "Inicio timer1 fail"



typedef enum {
    ESTADO_UP,
    ESTADO_FALLING,
    ESTADO_DOWN,
    ESTADO_RISING
} estado_t;

typedef struct {
    int* pins[NUM_PUL];
    estado_t * estadoActual[NUM_PUL];
    uint16_t * debounceCounter[NUM_PUL];
    int delay;
    int numero_pulsadores;
}pulsantes;


void init_MEF(pulsantes* pulsadores);
int act_MEF();

#endif //pulsantes.H