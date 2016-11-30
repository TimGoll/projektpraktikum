#include "main_StoreD.h"

namespace storage {
    Main_StoreD::Main_StoreD() {

    }
    Main_StoreD::~Main_StoreD() {

    }

    bool Main_StoreD::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //Serial.println("loop called");
        return true;
    }
}
