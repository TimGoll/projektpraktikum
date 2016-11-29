#include "mfcCtrl.h"

namespace control {
    MfcCtrl::MfcCtrl(int id) {
        this->id = id;

        //setze defaultwerte für das "nextEvent"
        this->nextEvent.value = -1;
        this->nextEvent.time  = -1;

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
        Serial.print("MFC ");
        Serial.print(this->id);
        Serial.print(" Typ: ");
        Serial.println(type);

        strcpy(this->type, type);
    }

    void MfcCtrl::setAdress(char adress[]) {
        Serial.print("MFC ");
        Serial.print(this->id);
        Serial.print(" Adresse: ");
        Serial.println(adress);

        strcpy(this->adress, adress);
    }

    void MfcCtrl::setEvent(int value, unsigned long time) {
        eventElement newEvent; //Erstelle neue Datenstruktur (Pointer)
        newEvent.value = value;
        newEvent.time  = time;

        Serial.print("MFC ");
        Serial.print(this->id);
        Serial.print(" Neues Event: ");
        Serial.print(newEvent.value);
        Serial.print(", ");
        Serial.println(newEvent.time);

        this->eventList.push(newEvent); //Speichere Datenstruktur in einer Liste
    }

    void MfcCtrl::start(unsigned long startTime) {
        this->startTime = startTime;
        this->ready = true;
    }

    //HAUPTSCHLEIFE
    bool MfcCtrl::compute() {
        if (this->ready) {
            if (this->nextEvent.time == -1) //lade erstes Event in nextEvent
                nextEvent = eventList.pop();

            if (millis() >= this->startTime + this->nextEvent.time) {
                //TODO: set MFC to this->nextEvent->value

                Serial.print("[Zeit: ");
                Serial.print(millis());
                Serial.print("] MFC ");
                Serial.print(this->id);
                Serial.print(" gesetzt auf: ");
                Serial.println(this->nextEvent.value);

                if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }
        }

        return true;
    }
}
