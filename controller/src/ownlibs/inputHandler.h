#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>
#include <newdel.h> //fügt new und delete hinzu, wird für "mthread" benötigt
#include <mthread.h>

#include "../config.h"

namespace io {

class InputHandler : public Thread {
public:
    //Defaultconstructor
    InputHandler();
    //Destructor
    ~InputHandler();

    //adding pseudo interrupts
    //pin: PinNummer, callback: Funktion, die aufgerufen werden soll, type: RISING, FALLING, CHANGE
    //definiere zwei Funktionen mit selben Namen aber verschiedenen Parametern
    void addInterrupt(uint8_t pin, void (*callback) (uint8_t), uint8_t type);
    void addInterrupt(uint8_t pin, void (*callback) (uint8_t), uint8_t type, uint16_t push_start, uint16_t push_interval);
protected:
    //Die Loop wird kontinuierlich aufgerufen und vollstaendig ausgefuehrt
    bool loop();
private:
    //speichere Callbackfunktionen, Parameter: Wert nach Wechsel
    void (*_callbacks[INPUTS_MAX_AMOUNT_BUTTONS]) (uint8_t);
    uint8_t _pin[INPUTS_MAX_AMOUNT_BUTTONS];
    uint8_t _type[INPUTS_MAX_AMOUNT_BUTTONS];

    uint8_t _pushed[INPUTS_MAX_AMOUNT_BUTTONS];
    uint32_t _last_push_time[INPUTS_MAX_AMOUNT_BUTTONS];

    bool _push_mode[INPUTS_MAX_AMOUNT_BUTTONS];
    uint16_t _push_start_time[INPUTS_MAX_AMOUNT_BUTTONS];
    uint16_t _push_interval_time[INPUTS_MAX_AMOUNT_BUTTONS];
    uint32_t _last_increment_time[INPUTS_MAX_AMOUNT_BUTTONS];

    uint8_t _stored_callbacks;

};

}

#endif
