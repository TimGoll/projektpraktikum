#include "main_valveCtrl.h"

namespace control {
    Main_ValveCtrl::Main_ValveCtrl() {
        this->amount_valve = 0;
    }
    Main_ValveCtrl::~Main_ValveCtrl() {

    }

    void Main_ValveCtrl::createValve(int amount) {
        this->amount_valve = amount;
        for (int i = 0; i < this->amount_valve; i++) {
            this->valve_list[i] = new control::ValveCtrl(i);
            this->valve_continue_next_loop[i] = true;
        }
    }

    void Main_ValveCtrl::setPins(char pins[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (int i = 0; i < this->amount_valve; i++) {
            this->valve_list[i]->setPin(atoi(pins[i])); //Uebergebe Pin-Nummer als Integer
        }
    }

    void Main_ValveCtrl::setEvent(int valveID, int value, unsigned long time) {
        this->valve_list[valveID]->setEvent(value, time);
    }

    void Main_ValveCtrl::start(unsigned long startTime) {
        for (int i = 0; i < this->amount_valve; i++) {
            this->valve_list[i]->start(startTime);
        }
    }

    void Main_ValveCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        for (int i = 0; i < this->amount_valve; i++) {
            this->valve_list[i]->setMainDisplayObjectPointer(main_display);
        }
    }

    bool Main_ValveCtrl::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //TODO Aufrufen der MFC.compute() Funktionen. Kann immer getan werden, hat erst Wirkung nach demsie mit MFC.start() aktiviert werden.
        for (int i = 0; i < this->amount_valve; i++) {
            if (this->valve_continue_next_loop[i])
                this->valve_continue_next_loop[i] = this->valve_list[i]->compute();
        }

        //TODO: Ueberpruefe, ob alle MFC Objekte abgeschlossen sind

        return true;
    }
}
