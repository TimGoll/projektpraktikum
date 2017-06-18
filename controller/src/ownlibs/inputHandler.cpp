#include "InputHandler.h"

namespace io {
    InputHandler::InputHandler() {
        this->_stored_callbacks = 0;

        //initialisiere Arrays
        for (uint8_t i = 0; i < INPUTS_MAX_AMOUNT_BUTTONS; i++) {
            this->_pushed[i] = 0;
            this->_last_push_time[i] = 0;
            this->_last_increment_time[i] = 0;
        }
    }

    InputHandler::~InputHandler() {

    }

    void InputHandler::addInterrupt(uint8_t pin, void (*callback) (uint8_t), uint8_t type) {
        this->_push_mode[this->_stored_callbacks] = false;

        this->_pin[this->_stored_callbacks] = pin;
        this->_callbacks[this->_stored_callbacks] = callback;
        this->_type[this->_stored_callbacks] = type;
        this->_stored_callbacks++;
    }

    void InputHandler::addInterrupt(uint8_t pin, void (*callback) (uint8_t), uint8_t type, uint16_t push_start, uint16_t push_interval) {
        this->_push_mode[this->_stored_callbacks] = true;

        this->_pin[this->_stored_callbacks] = pin;
        this->_callbacks[this->_stored_callbacks] = callback;
        this->_type[this->_stored_callbacks] = type;

        this->_push_start_time[this->_stored_callbacks] = push_start;
        this->_push_interval_time[this->_stored_callbacks] = push_interval;

        this->_stored_callbacks++;
    }



    bool InputHandler::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        for (uint8_t i = 0; i < this->_stored_callbacks; i++) {
            if (millis() - this->_last_push_time[i] > 25) {
                if (digitalRead(this->_pin[i]) && !this->_pushed[i]) { //RISING
                    this->_last_push_time[i] = millis();
                    this->_pushed[i] = 1;
                    if (this->_type[i] == RISING || this->_type[i] == CHANGE)
                        this->_callbacks[i](HIGH);
                } else if (!digitalRead(this->_pin[i]) && this->_pushed[i]) { //FALLING
                    this->_last_push_time[i] = millis();
                    this->_pushed[i] = 0;
                    if (this->_type[i] == FALLING || this->_type[i] == CHANGE)
                        this->_callbacks[i](LOW);
                } else

                //push mode
                if (digitalRead(this->_pin[i]) && this->_pushed[i]) { //Wenn gedrueckt gehalten
                    if (this->_push_mode[i]) {
                        if (millis() - this->_last_push_time[i] > this->_push_start_time[i]) {
                            if (millis() - this->_last_increment_time[i] > this->_push_interval_time[i]) {
                                this->_last_increment_time[i] = millis();
                                this->_callbacks[i](HIGH);
                            }
                        }
                    }
                }

            }
        }

        return true;
    }
}
