#include "main_mfcCtrl.h"

namespace control {
    Main_MfcCtrl::Main_MfcCtrl() {
        this->amount_MFC = 0;
    }
    Main_MfcCtrl::~Main_MfcCtrl() {

    }

    void Main_MfcCtrl::createMFC(int amount) {
        this->amount_MFC = amount;
        for (int i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i] = new control::MfcCtrl(i);
            this->mfc_list[i]->setMainDisplayObjectPointer(main_display);
            this->mfc_continue_next_loop[i] = true;
        }
    }

    void Main_MfcCtrl::setAdresses(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (int i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i]->setAdress(adresses[i]);
        }
    }

    void Main_MfcCtrl::setTypes(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (int i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i]->setType(adresses[i]);
        }
    }

    void Main_MfcCtrl::setEvent(int mfcID, int value, unsigned long time) {
        this->mfc_list[mfcID]->setEvent(value, time);
    }

    void Main_MfcCtrl::start(unsigned long startTime) {
        for (int i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i]->start(startTime);
        }
    }

    void Main_MfcCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    bool Main_MfcCtrl::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //TODO Aufrufen der MFC.compute() Funktionen. Kann immer getan werden, hat erst Wirkung nach demsie mit MFC.start() aktiviert werden.
        for (int i = 0; i < this->amount_MFC; i++) {
            if (this->mfc_continue_next_loop[i])
                this->mfc_continue_next_loop[i] = this->mfc_list[i]->compute();
        }

        //TODO: Ueberpruefe, ob alle MFC Objekte abgeschlossen sind

        return true;
    }
}
