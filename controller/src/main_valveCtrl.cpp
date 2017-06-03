#include "main_valveCtrl.h"

namespace control {
    Main_ValveCtrl::Main_ValveCtrl() {
        this->amount_valve = -1;
        this->amount_of_finished_valves = 0;

        this->queueFinished = false;
    }
    Main_ValveCtrl::~Main_ValveCtrl() {

    }

    void Main_ValveCtrl::createValve(uint16_t amount) {
        this->amount_valve = amount;
        for (uint16_t i = 0; i < this->amount_valve; i++) {
            this->valve_list[i] = new control::ValveCtrl(i);
            this->valve_list[i]->setMainDisplayObjectPointer(this->main_display);
            this->valve_continue_next_loop[i] = true;
        }
    }

    void Main_ValveCtrl::setPcbAddresses(uint16_t amount, char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        srl->println('D', "Adressen der Ventilplatinen gesetzt, Kommunikationsobjekte erzeugt.");
        for (uint16_t i = 0; i < amount; i++) {
            this->comPointer[i] = new Pca9555((int)strtol(adresses[i], NULL, 0)); //strtoul() parsed unsigned Hex-Zahlen
            this->comPointer[i]->begin();
        }
    }

    void Main_ValveCtrl::setPins(char pins[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (uint16_t i = 0; i < this->amount_valve; i++) {
            char tmp_ids[2][16];
            cmn::split(tmp_ids, pins[i], ' ');

            uint16_t id_pcb   = atoi(tmp_ids[0]);
            uint16_t id_valve = atoi(tmp_ids[1]);

            this->valve_list[i]->setComPointer(this->comPointer[id_pcb]);
            this->valve_list[i]->setPin(id_valve);
        }
    }

    void Main_ValveCtrl::setEvent(uint16_t valveID, uint16_t value, uint32_t time) {
        this->valve_list[valveID]->setEvent(value, time);
    }

    void Main_ValveCtrl::start(uint32_t startTime) {
        for (int i = 0; i < this->amount_valve; i++) {
            this->valve_list[i]->start(startTime);
        }
    }

    void Main_ValveCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    void Main_ValveCtrl::getValveValueList(uint16_t valveValueList[]) {
        for (uint16_t i = 0; i < this->amount_valve; i++) {
            valveValueList[i] = this->valve_list[i]->getCurrentValue();
        }
    }

    uint16_t Main_ValveCtrl::getAmount_valve(){
      return this->amount_valve;
    }

    bool Main_ValveCtrl::getQueueFinished() {
        return this->queueFinished;
    }

    bool Main_ValveCtrl::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //Aufrufen der valveCtrl->compute() Funktionen. Kann immer getan werden, hat erst Wirkung nach demsie mit valveCtrl->start() aktiviert werden.
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
            this->queueFinished = true; //setze 'finished'
            return false;
        }

        return true;
    }
}
