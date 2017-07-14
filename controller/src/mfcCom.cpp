#include "mfcCom.h"

namespace communication {
    MfcCom::MfcCom() {
        //this->_mks      = new MfcCom_Mks();
        Mks::init();
        //this->_buerkert = new MfcCom_Buerkert();
    }

    MfcCom::~MfcCom() {

    }

    bool MfcCom::writeValue(char type[], char address[], float value, float *destination) {
        if (strcmp(type, "MKS") == 0)
            //return this->_mks->writeValue(address, value);
            return Mks::writeValue(address, value, destination);
        //else if (strcmp(type, "Buerkert") == 0)
            //return this->_buerkert->writeValue(address, value);
        //else
            //return 100; //TODO errorcode
        return false;
    }

    bool MfcCom::readValue(char type[], char address[], float *destination) {
        //if (strcmp(type, "MKS") == 0)
            //return this->_mks->readValue(address);
        //else if (strcmp(type, "Buerkert") == 0)
            //return this->_buerkert->readValue(address);
        //else

        return false;
    }

    // Initialisiere die Kommunikation mit den MFCs
    MfcCom *mfcCom = new MfcCom();
}
