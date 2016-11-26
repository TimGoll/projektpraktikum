#include "boschCom.h"

namespace communication {
    BoschCom::BoschCom() {

    }
    BoschCom::~BoschCom() {

    }

    bool BoschCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        //Serial.println("loop called");
        return true;
    }
}
