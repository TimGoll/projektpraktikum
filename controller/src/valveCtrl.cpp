#include "valveCtrl.h"

namespace control {
    ValveCtrl::ValveCtrl(uint16_t id) {
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

    void ValveCtrl::setComPointer(Pca9555 *pca9555) {
        this->pca9555 = pca9555;
    }

    void ValveCtrl::setPin(uint16_t pin) {
        this->pin = pin;

        srl->print('D', "Ventil ");
        srl->print('D', this->id);
        srl->print('D', " Pin: ");
        srl->println('D', this->pin);
    }

    void ValveCtrl::setEvent(uint16_t value, uint32_t time) {
        eventElement_valve newEvent; //Erstelle neue Datenstruktur
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

    void ValveCtrl::start(uint32_t startTime) {
        this->startTime = startTime;
        this->ready = true;
    }

    void ValveCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    uint8_t ValveCtrl::getCurrentValue() {
        if (eventList.isEmpty())
            return 0;
        return this->currentValue;
    }

    bool ValveCtrl::compute() {
        if (this->ready) {
            if (this->nextEvent.value == -1) { //lade erstes Event in nextEvent
                if (eventList.isEmpty()) //beende den Thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }


            if (millis() >= this->startTime + this->nextEvent.time) {
                //setze Ventil auf this->nextEvent.value
                this->pca9555->digitalWrite(this->pin, this->nextEvent.value);

                uint32_t currentTime = millis();

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
