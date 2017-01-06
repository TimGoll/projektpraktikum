#include "main_boschCom.h"

namespace communication {
    Main_BoschCom::Main_BoschCom() {
        this->ready = false;
    }
    Main_BoschCom::~Main_BoschCom() {

    }

    void Main_BoschCom::setIntervall(int intervall) {
        this->intervall = intervall;

        srl->print('D', "BoschCom: Intervall gesetzt auf: ");
        srl->println('D', this->intervall);
    }

    void Main_BoschCom::start(unsigned long time) {
        this->ready = true;
        this->lastTime = time;
    }


    bool Main_BoschCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        if (millis() >= this->lastTime) {
            //Lese Sensor aus
            //srl->print('D', millis());
            //srl->println('D', " Messe Boschsensor ...");


            this->lastTime += this->intervall;
        }

        return true;
    }
}
