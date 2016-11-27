#include "mfcCtrl.h"

namespace control {
    MfcCtrl::MfcCtrl(int id) {
        this->id = id;

        //setze defaultwerte für das "nextEvent"
        this->nextEvent->value = -1;
        this->nextEvent->time  = -1;

        this->ready = false;

        Serial.print("MFC ");
        Serial.print(this->id);
        Serial.println(" erstellt.");
    }
    MfcCtrl::~MfcCtrl() {
        Serial.print("MFC ");
        Serial.print(this->id);
        Serial.println(" geloescht.");
    }

    void MfcCtrl::setType(char type[]) {
        strcpy(this->type, type);
    }

    void MfcCtrl::setAdress(char adress[]) {
        strcpy(this->adress, adress);
    }

    void MfcCtrl::setEvent(int value, unsigned int time) {
        eventElement *newEvent; //Erstelle neue Datenstruktur (Pointer)
        newEvent->value = value;
        newEvent->time  = time;

        this->eventList.push(newEvent); //Speichere Datenstruktur in einer Liste
    }

    void MfcCtrl::start() {
        this->ready = true;
    }

    //HAUPTSCHLEIFE
    bool MfcCtrl::compute() {
        Serial.print("MFC ");
        Serial.print(this->id);
        Serial.println(" wird ausgefuert.");

        if (this->ready) {
            if (this->nextEvent->time == -1) //lade erstes Event in nextEvent
                nextEvent = eventList.pop();

            if (this->nextEvent->time >= millis()) {
                //TODO: set MFC to this->nextEvent->value
                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }
        }

        return true;
    }
}
