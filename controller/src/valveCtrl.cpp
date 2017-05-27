#include "valveCtrl.h"

namespace control {
    ValveCtrl::ValveCtrl(int id) {
        this->id = id;

        //setze defaultwerte für das "nextEvent"
        this->nextEvent.value = -1;
        this->nextEvent.time  = 0;

        this->ready = false;

        this->currentValue = 0;

        srl->print('D', "Ventil ");
        srl->print('D', this->id);
        srl->println('D', " erstellt.");
    }
    ValveCtrl::~ValveCtrl() {

    }

    void ValveCtrl::setPin(int pin) {
        this->pin = pin;

        pinMode(this->pin, OUTPUT);

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

    void ValveCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    int ValveCtrl::getCurrentValue() {
        return this->currentValue;
    }

    bool ValveCtrl::compute() {
        if (this->ready) {
            if (this->nextEvent.value == -1) { //lade erstes Event in nextEvent
                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }


            if (millis() >= this->startTime + this->nextEvent.time) {
                //setze Ventil auf this->nextEvent.value
                digitalWrite(this->pin, this->nextEvent.value);

                unsigned long currentTime = millis();

                srl->print('D', "Ventil\t");
                srl->print('D', this->id);
                srl->print('D', " gesetzt auf: ");
                srl->print('D', this->nextEvent.value);
                srl->print('D', "\t\tSchaltzeit:\t");
                srl->print('D', currentTime);
                srl->print('D', "\terwartet:\t");
                srl->print('D', this->startTime + this->nextEvent.time);
                srl->print('D', "\t( Rel.Zeit: ");
                srl->print('D', this->nextEvent.time);
                srl->print('D', " )\t( ");
                srl->print('D', currentTime - (this->startTime + this->nextEvent.time));
                srl->println('D', "\tms Verzoegerung )");

                this->main_display->setLastEvent('V', this->id, this->nextEvent.value, this->nextEvent.time);
                this->currentValue = this->nextEvent.value;

                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }
        }
        return true;
    }
}
