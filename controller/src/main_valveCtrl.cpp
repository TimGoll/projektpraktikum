#include "main_valveCtrl.h"

namespace control {
    Main_ValveCtrl::Main_ValveCtrl() {
        this->amount_valve = -1;
        this->amount_of_finished_valves = 0;
    }
    Main_ValveCtrl::~Main_ValveCtrl() {

    }

    void Main_ValveCtrl::createValve(int amount) {
        this->amount_valve = amount;
        for (int i = 0; i < this->amount_valve; i++) {
            this->valve_list[i] = new control::ValveCtrl(i);
            this->valve_list[i]->setMainDisplayObjectPointer(this->main_display);
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
        this->main_display = main_display;
    }

    void Main_ValveCtrl::getValveValueList(int valveValueList[]) {
        for (int i = 0; i < this->amount_valve; i++) {
            valveValueList[i] = this->valve_list[i]->getCurrentValue();
        }
    }

    int Main_ValveCtrl::getAmount_valve(){
      return this->amount_valve;
    }

    bool Main_ValveCtrl::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //Aufrufen der Valve.compute() Funktionen. Kann immer getan werden, hat erst Wirkung nach demsie mit Valve.start() aktiviert werden.
        for (int i = 0; i < this->amount_valve; i++) {
            if (this->valve_continue_next_loop[i]) {
                this->valve_continue_next_loop[i] = this->valve_list[i]->compute();

                if (!this->valve_continue_next_loop[i]) {
                    this->amount_of_finished_valves++;
                    srl->print('D', "Eventliste von Ventil ");
                    srl->print('D', i);
                    srl->println('D', " abgearbeitet.");
                }
            }
        }

        //Beenden des Threads, wenn alle Events abgearbeitet sind
        if (this->amount_valve != -1 && this->amount_of_finished_valves >= this->amount_valve) {
            srl->println('D', "Alle Ventile abgearbeitet.");
            this->main_display->queueFinished();
            return false;
        }

        return true;
    }
}
