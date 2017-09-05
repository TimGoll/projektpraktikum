#include "main_mfcCtrl.h"

namespace control {
    Main_MfcCtrl::Main_MfcCtrl() {
        this->amount_MFC = -1;
        this->amount_of_finished_mfcs = 0;

        this->queueFinished = false;

        this->lastReadEventSuccess = true;
        this->nextReadEvent_time = 0;
        this->nextMfcId = 0;

        this->ready = false;
    }
    Main_MfcCtrl::~Main_MfcCtrl() {

    }

    void Main_MfcCtrl::createMFC(uint16_t amount) {
        this->amount_MFC = amount;
        for (uint16_t i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i] = new control::MfcCtrl(i);
            this->mfc_list[i]->setMainDisplayObjectPointer(this->main_display);
            this->mfc_continue_next_loop[i] = true;
        }
    }

    void Main_MfcCtrl::setAddresses(char addresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (uint16_t i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i]->setAddress(addresses[i]);
        }
    }

    void Main_MfcCtrl::setTypes(char adresses[][SERIAL_READ_MAX_BLOCK_SIZE]) {
        for (uint16_t i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i]->setType(adresses[i]);
        }
    }

    void Main_MfcCtrl::setEvent(uint16_t mfcID, float value, uint32_t time) {
        this->mfc_list[mfcID]->setEvent(value, time);
    }

    void Main_MfcCtrl::start(uint32_t startTime) {
        this->ready = true;
        this->nextReadEvent_time = millis() + MFC_READ_TIMING;

        for (uint16_t i = 0; i < this->amount_MFC; i++) {
            this->mfc_list[i]->start(startTime);
        }
    }

    void Main_MfcCtrl::setMainDisplayObjectPointer(io::Main_Display *main_display) {
        this->main_display = main_display;
    }

    void Main_MfcCtrl::getMfcValueList(float mfcValueList[]) {
        for (uint16_t i = 0; i < this->amount_MFC; i++) {
            mfcValueList[i] = this->mfc_list[i]->getCurrentValue();
        }
    }

    void Main_MfcCtrl::getMfcDestinationList(float mfcDestinationList[]) {
        for (uint16_t i = 0; i < this->amount_MFC; i++) {
            mfcDestinationList[i] = this->mfc_list[i]->getDestinationValue();
        }
    }

    uint16_t Main_MfcCtrl::getAmount_MFC(){
      return this->amount_MFC;
    }

    bool Main_MfcCtrl::getQueueFinished() {
        return this->queueFinished;
    }

    void Main_MfcCtrl::getMfcType(uint16_t id, char type[]) {
        if (id < this->amount_MFC) {
            char type_tmp[16];
            this->mfc_list[id]->getType(type_tmp);
            strcpy(type, type_tmp);
        }
    }


    bool Main_MfcCtrl::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //Aufrufen der mfcCtrl->compute() Funktionen. Kann immer getan werden, hat erst Wirkung nachdem sie mit mfcCtrl->start() aktiviert werden.
        for (uint16_t i = 0; i < this->amount_MFC; i++) {
            if (this->mfc_continue_next_loop[i]) {
                this->mfc_continue_next_loop[i] = this->mfc_list[i]->compute();

                if (!this->mfc_continue_next_loop[i]) {
                    this->amount_of_finished_mfcs++;
                    srl->print('D', "Eventliste von MFC ");
                    srl->print('D', i);
                    srl->println('D', " abgearbeitet.");
                }
            }
        }


        if (this->ready && (millis() >= this->nextReadEvent_time || !this->lastReadEventSuccess)) {
            this->lastReadEventSuccess = this->mfc_list[this->nextMfcId]->readCurrentValue();

            if (this->lastReadEventSuccess) {
                this->nextReadEvent_time += MFC_READ_TIMING;
                this->nextMfcId = ++this->nextMfcId % this->amount_MFC;
            }
        }

        //Beenden des Threads, wenn alle Events abgearbeitet sind
        if (this->amount_MFC != -1 && this->amount_of_finished_mfcs >= this->amount_MFC) {
            srl->println('D', "Alle MFCs abgearbeitet.");
            this->queueFinished = true; //setze 'finished'
            return false;
        }


        return true;
    }
}
