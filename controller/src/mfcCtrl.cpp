#include "mfcCtrl.h"

namespace control {
    MfcCtrl::MfcCtrl(uint16_t id) {
        this->id = id;

        //setze defaultwerte für das "nextEvent"
        this->nextEvent.value = -1;
        this->nextEvent.time  = 0;

        this->ready = false;

        this->currentValue = 0;

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

    void MfcCtrl::setAddress(char address[]) {
        strcpy(this->address, address);

        srl->print('D', "MFC ");
        srl->print('D', this->id);
        srl->print('D', " Adresse: ");
        srl->println('D', this->address);
    }

    void MfcCtrl::setEvent(uint16_t value, uint32_t time) {
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

    void MfcCtrl::start(uint32_t startTime) {
        this->startTime = startTime;
        this->ready = true;
    }

    void MfcCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    float MfcCtrl::getCurrentValue() {
        //waehrend man auf eine Variable zugreift, die in einem Interrupt bearbeitet werden kann
        //muss man dafuer sorgen, dass sie sich im Lesevorgang nicht aendern kann
        noInterrupts();
        float tmp_currentValue = this->currentValue;
        interrupts();
        return tmp_currentValue;
    }

    void MfcCtrl::getType(char type[]) {
        strcpy(type, this->type);
    }

    //HAUPTSCHLEIFE
    bool MfcCtrl::compute() {
        if (this->ready) {
            if (this->nextEvent.value == -1) { //lade erstes Event in nextEvent
                if (eventList.isEmpty()) //beende den Thread, wenn alle Events abgearbeitet sind
                    return false;
                nextEvent = eventList.pop();
            }

            if (millis() >= this->startTime + this->nextEvent.time) {
                //TODO Ist-Wert getaktet auslesen
                bool write_success = communication::mfcCom->writeValue(this->type, this->address, this->nextEvent.value, &this->currentValue);

                //Wenn Schreiben der Daten erfolgreich war, gebe Debugnachrichten aus und lade neues Event. Ansonsten soll es in der
                //Queue behalten werden, bis Uebertragung angenommen wird.
                //Fehlerhafte Uebertragungen fuehren nicht zu einem Neustart. Es wird nur darauf geachtet, ob die Leitung gerade frei ist
                if (write_success) {
                    uint32_t currentTime = millis();

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
                    this->currentValue = this->nextEvent.value;

                    if (eventList.isEmpty()) //beende den thread, wenn alle Events abgearbeitet sind
                        return false;
                    nextEvent = eventList.pop();
                } else {
                    srl->println('D', "Port belegt, versuche erneut...");
                }
            }
        }
        return true;
    }
}
