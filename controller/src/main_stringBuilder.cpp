#include "main_stringBuilder.h"

namespace communication {
    Main_StringBuilder::Main_BoschCom() {
        this->ready = false;
    }
    Main_StringBuilder::~Main_BoschCom() {

    }

    void Main_StringBuilder::setIntervall(int intervall) {
        this->intervall = intervall;
    }

    void Main_StringBuilder::start(unsigned long time) {
        this->ready = true;
        this->lastTime = time + this->intervall / 2; //addiere halbes Intervall um versetzt zur Messung zu speichern
    }

    bool Main_StringBuilder::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        if (this->ready) {
            if (millis() >= this->lastTime) {
                // TODO
                // baue String (frage Werte von MFCs, Ventilen und Boschsensor ab, nutze
                // folgende noch nocht existierenden Funktionen:
                // this->main_mfcCtrl->getMfcValueList(); //int[]
                // this->main_valveCtrl->getValveValueList(); //int[]
                // this->main_bischCom->getSensorValue(); //int
                // )
                // Sende String an SD
                // sende String an LabCom
                // (Bei SD die Funktion aufrufen, die deine Loop ersetzt // bei lab com this->main_labCom->setNewLine(string))

                //addiere intervall zur letzten Zeit und NICHT zur aktuellen Zeit, um
                //Zeitungenauigkeiten durch Verzoegerungen vorzubeugen
                this->lastTime += this->intervall;
            }
        }

        return true;
    }
}
