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
            mfc_continue_next_loop[i] = true;
        }
    }

    void MfcMain::setAdresses(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (int i = 0; i < this->amount_MFC; i++) {
            mfc_list[i]->setAdress(adresses[i]);
        }
    }

    void MfcMain::setTypes(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (int i = 0; i < this->amount_MFC; i++) {
            mfc_list[i]->setType(adresses[i]);
        }
    }

    void MfcMain::setEvent(int mfcID, int value, unsigned long time) {
        mfc_list[mfcID]->setEvent(value, time);
    }

    void MfcMain::start(unsigned long startTime) {
        for (int i = 0; i < this->amount_MFC; i++) {
            mfc_list[i]->start(startTime);
        }
    }

    bool MfcMain::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //TODO Aufrufen der MFC.compute() Funktionen. Kann immer getan werden, hat erst Wirkung nach demsie mit MFC.start() aktiviert werden.
        for (int i = 0; i < this->amount_MFC; i++) {
            if (mfc_continue_next_loop[i])
                mfc_continue_next_loop[i] = mfc_list[i]->compute();
        }

        //TODO: Ueberpruefe, ob alle MFC Objekte abgeschlossen sind

        return true;
    }
}
