#include "main_stringBuilder.h"

namespace communication {
    Main_StringBuilder::Main_StringBuilder() {
        this->ready = false;

        this->storeD = new storage::StoreD(); //Erzeuge StoreD Objekt
    }
    Main_StringBuilder::~Main_StringBuilder() {

    }

    void Main_StringBuilder::setIntervall(int intervall) {
        this->intervall = intervall;

        srl->print('D', "StringBuilder: Intervall gesetzt auf: ");
        srl->println('D', this->intervall);
    }

    void Main_StringBuilder::start(unsigned long time) {
        this->ready = true;
        this->lastTime = time + this->intervall / 2; //addiere halbes Intervall um versetzt zur Messung zu speichern
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
                //srl->print('D', millis());
                //srl->println('D', " Gebe String mit Daten aus...");

                // TODO
                // baue String (frage Werte von MFCs, Ventilen und Boschsensor ab, nutze
                // folgende Funktionen:

                //Zeit hinzufügen
                //strccat(newLine,

                // this->main_mfcCtrl->getMfcValueList(&mfcValueList); //int[]     ; du kennst die Anzahl der Eintraege und kannst durch das Array durchiterieren
                control::Main_MfcCtrl::getMfcValueList(&mfcValueList[MAX_AMOUNT_MFC]);
                control::Main_MfcCtrl::getAmount_MFC(&amount_MFC);
                for (int i = 0; i < amount_MFC; i++) {
                    sprintf(currentMfcValue, "%d", mfcValueList[i]);
                    strcat(newLine, currentMfcValue);
                    strcat(newLine, SEPERATIONCHAR);
                }

                // this->main_valveCtrl->getValveValueList(&valveValueList); //int[] ; Funktion muss Pointer des Zielarrays uebergeben bekommen
                control::Main_ValveCtrl::getValveValueList(&valveValueList[MAX_AMOUNT_VALVE]);
                control::Main_ValveCtrl::getAmount_valve(&amount_valve);
                for (int i = 0; i < amount_valve; i++) {
                    sprintf(currentValveValue, "%d", valveValueList[i]);
                    strcat(newLine, currentValveValue);
                    strcat(newLine, SEPERATIONCHAR);
                }

                // this->main_boschCom->getCurrentValue(); //int
                communication::Main_BoschCom::getCurrentValue(&currentIntBoschValue);
                sprintf(currentBoschValue, "%d", currentIntBoschValue);
                strcat(newLine, currentBoschValue);

                // Sende String an SD
                // sende String an LabCom
                // (Bei SD die Funktion aufrufen, die deine Loop ersetzt (this->storeD->...) // bei lab com this->main_labCom->setNewLine(string))

                //addiere intervall zur letzten Zeit und NICHT zur aktuellen Zeit, um
                //Zeitungenauigkeiten durch Verzoegerungen vorzubeugen
                this->lastTime += this->intervall;
            }
        }

        return true;
    }
}
