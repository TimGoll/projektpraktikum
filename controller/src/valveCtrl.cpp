#include "valveCtrl.h"

namespace control {
    ValveCtrl::ValveCtrl(int id) {
        this->id = id;

        //setze defaultwerte für das "nextEvent"
        this->nextEvent.value = -1;
        this->nextEvent.time  = -1;

        this->ready = false;

        srl->print('D', "Ventil ");
        srl->print('D', this->id);
        srl->println('D', " erstellt.");
    }
    ValveCtrl::~ValveCtrl() {

    }

    void ValveCtrl::setPin(int pin) {
        this->pin = pin;

        srl->print('D', "Ventil ");
        srl->print('D', this->id);
        srl->print('D', " Pin: ");
        srl->println('D', this->pin);
    }

    void ValveCtrl::setEvent(int value, unsigned long time) {
        eventElement newEvent; //Erstelle neue Datenstruktur
        newEvent.value = value;
        newEvent.time  = time;

        srl->print('D', "Ventil ");
        srl->print('D', this->id);
        srl->print('D', " Neues Event: ");
        srl->print('D', newEvent.value);
        srl->print('D', ", ");
        srl->println('D', newEvent.time);

        this->eventList.push(newEvent); //Speichere Datenstruktur in einer Liste
    }

    void ValveCtrl::start(unsigned long startTime) {
        this->startTime = startTime;
        this->ready = true;
    }

    bool ValveCtrl::compute() {
        if (this->ready) {
            if (this->nextEvent.time == -1) { //lade erstes Event in nextEvent
                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }

            if (millis() >= this->startTime + this->nextEvent.time) {
                //TODO: set Valve to this->nextEvent->value

                srl->print('D', "[Zeit: ");
                srl->print('D', millis());
                srl->print('D', "] Ventil ");
                srl->print('D', this->id);
                srl->print('D', " gesetzt auf: ");
                srl->println('D', this->nextEvent.value);

                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }
        }

        return true;
    }
}
