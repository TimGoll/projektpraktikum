#include "main_stringBuilder.h"

namespace communication {
    Main_StringBuilder::Main_StringBuilder() {
        this->ready = false;
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

        this->storeD->setIntervall(this->intervall);
        this->storeD->setAmountMFC(this->main_mfcCtrl->getAmount_MFC());
        this->storeD->setAmountValve(this->main_valveCtrl->getAmount_valve());
        this->storeD->openFile();
    }

    void Main_StringBuilder::bothQueuesFinished() {
        //beende Messung
        this->ready = false;
        //beide Queues sind abgearbeitet, sage Datei, dass sie nun geschlossen werden kann.
        this->storeD->closeFile();
    }

    void Main_StringBuilder::setDateString(char dateString[]) {
        strcpy(this->dateString, dateString);
        this->storeD->setDate(this->dateString);
    }

    void Main_StringBuilder::setTypes(char types[][16]) {
        this->storeD->setTypes(types);
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

    void Main_StringBuilder::setStoreDObjectPointer(storage::StoreD *storeD) {
        this->storeD = storeD;
    }

    bool Main_StringBuilder::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        if (this->ready) {
            if (millis() >= this->lastTime) {
                // temporary
                char output[16]; //4 Byte + \0 !! Wichtig für strcat()

                this->newLine[0] = '\0';

                // Zeit in millis()
                // Passe currentTime so an, dass sie die relative Zeit zum Start anzeigt (rechne Schreibeverschiebnung wieder raus)
                uint32_t currentTime = this->lastTime - ( this->startTime +  this->intervall / 2);
                //cmn::integerToByte(currentTime, 4, output);
                sprintf(output, "%lu", currentTime);
                strcat(this->newLine, output);
                strcat(this->newLine, " ");

                //MFC Werte
                this->main_mfcCtrl->getMfcValueList(this->mfcValueList);
                for (uint8_t i = 0; i < this->main_mfcCtrl->getAmount_MFC(); i++) {
                    //cmn::integerToByte(this->mfcValueList[i], 4, output);
                    sprintf(output, "%d", this->mfcValueList[i]);
                    strcat(this->newLine, output);
                    strcat(this->newLine, " ");
                }

                //TODO MFC IST WERTE
                this->main_mfcCtrl->getMfcValueList(this->mfcValueList);
                for (uint8_t i = 0; i < this->main_mfcCtrl->getAmount_MFC(); i++) {
                    //cmn::integerToByte(this->mfcValueList[i], 4, output);
                    sprintf(output, "%d", this->mfcValueList[i]);
                    strcat(this->newLine, output);
                    strcat(this->newLine, " ");
                }

                //Ventilwerte
                this->main_valveCtrl->getValveValueList(valveValueList);
                for (uint8_t i = 0; i < this->main_valveCtrl->getAmount_valve(); i++) {
                    //cmn::integerToByte(valveValueList[i], 1, output);
                    sprintf(output, "%d", this->valveValueList[i]);
                    strcat(this->newLine, output);
                    strcat(this->newLine, " ");
                }

                // Boschsensor
                //cmn::integerToByte(this->main_boschCom->getCurrentValue(), 2, output);
                this->boschValues = this->main_boschCom->getCurrentValues();
                for (uint8_t i = 0; i < 3; i++) {
                    sprintf(output, "%f", this->boschValues[i]);
                    strcat(this->newLine, output);
                    strcat(this->newLine, " ");
                }


                // Sende String an SD
                this->storeD->writeNewLine(this->newLine);


                // sende String an LabView
                srl->println('L', this->newLine);

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
