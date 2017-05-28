#include "main_boschCom.h"

namespace communication {
    Main_BoschCom::Main_BoschCom() {
        this->ready = false;
        this->currentValue = 0;

        this->bme280 = new Adafruit_BME280(0x76);
        if (!this->bme280->begin())
            srl->println('D', "BME280 nicht gefunden.");
        else
            srl->println('D', "BME280 gefunden.");
    }
    Main_BoschCom::~Main_BoschCom() {

    }

    void Main_BoschCom::setIntervall(uint16_t intervall) {
        this->intervall = intervall;

        srl->print('D', "BoschCom: Intervall gesetzt auf: ");
        srl->println('D', this->intervall);
    }

    void Main_BoschCom::start(uint32_t time) {
        this->ready = true;
        this->lastTime = time;
    }

    uint16_t Main_BoschCom::getCurrentValue() {
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

                /*
                 * Read BME280 with...
                 * this->bme280->readTemperature()
                 * this->bme280->readPressure()
                 * this->bme280->readHumidity()
                 */


                this->lastTime += this->intervall;
            }
        }

        return true;
    }
}
