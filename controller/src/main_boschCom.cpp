#include "main_boschCom.h"

namespace communication {
    Main_BoschCom::Main_BoschCom() {
        this->ready = false;
        this->currentValue = 0;
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

    int Main_BoschCom::getCurrentValue() {
        return this->currentValue;
    }


    bool Main_BoschCom::loop() {
        //Gebe false zurueck um den Thread zu beenden. True bedeutet, dass der Thread weiter läuft
        if (kill_flag)
            return false;

        if (this->ready) {
            if (millis() >= this->lastTime) {
                //Lese Sensor aus, speichere in this->currentValue;
                //srl->print('D', millis());
                //srl->println('D', " Messe Boschsensor ...");


                this->lastTime += this->intervall;
            }
        }

        return true;
    }
}
