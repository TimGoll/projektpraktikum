#include "mfcCtrl.h"

namespace control {
    MfcCtrl::MfcCtrl(int id) {
        this->id = id;

        //setze defaultwerte für das "nextEvent"
        this->nextEvent.value = -1;
        this->nextEvent.time  = -1;

        this->ready = false;

        srl->print('D', "MFC ");
        srl->print('D', this->id);
        srl->println('D', " erstellt.");
    }
    MfcCtrl::~MfcCtrl() {
        srl->print('D', "MFC ");
        srl->print('D', this->id);
        srl->println('D', " geloescht.");
    }

    void MfcCtrl::setType(char type[]) {
        strcpy(this->type, type);

        srl->print('D', "MFC ");
        srl->print('D', this->id);
        srl->print('D', " Typ: ");
        srl->println('D', this->type);
    }

    void MfcCtrl::setAdress(char adress[]) {
        strcpy(this->adress, adress);

        srl->print('D', "MFC ");
        srl->print('D', this->id);
        srl->print('D', " Adresse: ");
        srl->println('D', this->adress);
    }

    void MfcCtrl::setEvent(int value, unsigned long time) {
        eventElement newEvent; //Erstelle neue Datenstruktur
        newEvent.value = value;
        newEvent.time  = time;

        srl->print('D', "MFC ");
        srl->print('D', this->id);
        srl->print('D', " Neues Event: ");
        srl->print('D', newEvent.value);
        srl->print('D', ", ");
        srl->println('D', newEvent.time);

        this->eventList.push(newEvent); //Speichere Datenstruktur in einer Liste
    }

    void MfcCtrl::start(unsigned long startTime) {
        this->startTime = startTime;
        this->ready = true;
    }

    void MfcCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    //HAUPTSCHLEIFE
    bool MfcCtrl::compute() {
        if (this->ready) {
            if (this->nextEvent.time == -1) { //lade erstes Event in nextEvent
                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }

            if (millis() >= this->startTime + this->nextEvent.time) {
                //TODO: set MFC to this->nextEvent->value

                unsigned long currentTime = millis();

                srl->print('D', "MFC\t");
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

                this->main_display->setLastEvent('M', this->id, this->nextEvent.value, this->nextEvent.time);

                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }
        }

        return true;
    }
}
