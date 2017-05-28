#include "main_stringBuilder.h"

namespace communication {
    Main_StringBuilder::Main_StringBuilder() {
        this->ready = false;

        this->storeD = new storage::StoreD(); //Erzeuge StoreD Objekt
    }
    Main_StringBuilder::~Main_StringBuilder() {

    }

    void Main_StringBuilder::setIntervall(uint16_t intervall) {
        this->intervall = intervall;

        srl->print('D', "StringBuilder: Intervall gesetzt auf: ");
        srl->println('D', this->intervall);
    }

    void Main_StringBuilder::start(uint32_t time) {
        this->ready = true;
        this->startTime = time;
        this->lastTime = time + this->intervall / 2; //addiere halbes Intervall um versetzt zur Messung zu speichern
    }

    void Main_StringBuilder::bothQueuesFinished() {
        //beide Queues sind abgearbeitet, sage Datei, dass sie nun geschlossen werden kann.
        this->storeD->closeFile();
    }

    void Main_StringBuilder::setDateString(char dateString[]) {
        strcpy(this->dateString, dateString);
    }


    void Main_StringBuilder::setMainValveObjectPointer(control::Main_ValveCtrl *main_valveCtrl) {
        this->main_valveCtrl = main_valveCtrl;
    }

    void Main_StringBuilder::setMainMfcObjectPointer(control::Main_MfcCtrl *main_mfcCtrl) {
        this->main_mfcCtrl = main_mfcCtrl;
    }

    void Main_StringBuilder::setMainBoschObjectPointer(communication::Main_BoschCom *main_boschCom) {
        this->main_boschCom = main_boschCom;
    }

    bool Main_StringBuilder::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        if (this->ready) {
            if (millis() >= this->lastTime) {
                //temporary
                char output[] = "    "; //4 Byte + \0 !! Wichtig für strcat()

                //HEADER nur zu Beginn einmal Funktion ausfuehren:
                //(Antwort von Markus): Header der txt-Datei wird beim Öffnen von dieser erstellt, siehe StoreD::openFile

                // Zeit in millis()
                // Passe currentTime so an, dass sie die relative Zeit zum Start anzeigt (rechne Schreibeverschiebnung wieder raus)
                uint32_t currentTime = this->lastTime - ( this->startTime +  this->intervall / 2);
                strcat(this->newLine, cmn::integerToByte(currentTime, 4, output));

                //MFC Werte
                this->main_mfcCtrl->getMfcValueList(mfcValueList);
                for (uint16_t i = 0; i < this->main_mfcCtrl->getAmount_MFC(); i++) {
                    strcat(this->newLine, cmn::integerToByte(mfcValueList[i], 3, output)); //TODO richtige Bytesize
                }

                //Ventilwerte
                this->main_valveCtrl->getValveValueList(valveValueList);
                for (uint16_t i = 0; i < this->main_valveCtrl->getAmount_valve(); i++) {
                    strcat(this->newLine, cmn::integerToByte(valveValueList[i], 3, output)); //TODO richtige Bytesize
                }

                //Boschsensor
                strcat(this->newLine, cmn::integerToByte(this->main_boschCom->getCurrentValue(), 3, output)); //TODO richtige Bytesize

                // Sende String an SD
                this->storeD->setNewLine(this->newLine);
                this->storeD->setDate(this->dateString);
                this->storeD->setIntervall(this->intervall);

                // sende String an LabCom

                // (Bei SD die Funktion aufrufen, die deine Loop ersetzt (this->storeD->...) // bei lab com this->main_labCom->setNewLine(string))
                // setze String zurueck fuer neuen String!

                //addiere intervall zur letzten Zeit und NICHT zur aktuellen Zeit, um
                //Zeitungenauigkeiten durch Verzoegerungen vorzubeugen
                this->lastTime += this->intervall;
            }
        }

        return true;
    }
}
