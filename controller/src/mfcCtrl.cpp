#include "mfcCtrl.h"

namespace control {
    MfcCtrl::MfcCtrl() {

    }
    MfcCtrl::~MfcCtrl() {

    }

    bool MfcCtrl::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        Serial.println("loop called");
        return true;
    }
}
