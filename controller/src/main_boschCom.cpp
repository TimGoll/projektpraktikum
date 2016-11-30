#include "main_boschCom.h"

namespace communication {
    Main_BoschCom::Main_BoschCom() {

    }
    Main_BoschCom::~Main_BoschCom() {

    }

    bool Main_BoschCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //Serial.println("loop called");
        return true;
    }
}
