#include "mfc_main.h"

namespace control {
    MfcMain::MfcMain() {

    }
    MfcMain::~MfcMain() {

    }

    void MfcMain::createMFC(int amount) {
        this->amount_MFC = amount;
        for (int i = 0; i < this->amount_MFC; i++) {
            mfc_list[i] = new control::MfcCtrl(i);
        }
    }

    void MfcMain::setAdresses(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (int i = 0; i < this->amount_MFC; i++) {
            mfc_list[i]->setAdress(adresses[i]);
        }
    }

    void MfcMain::setEvent(int mfcID, int value, unsigned int time) {
        mfc_list[mfcID]->setEvent(value, time);
    }

    bool MfcMain::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //Serial.println("loop called");
        return true;
    }
}
