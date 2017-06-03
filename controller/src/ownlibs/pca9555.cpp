#include "pca9555.h"

Pca9555::Pca9555(uint8_t address) {
    this->_address = address;
    this->_valueRegister         = 0b0000000000000000; //alle Pins auf LOW
    this->_configurationRegister = 0b0000000000000000; //alle PINS als OUTPUT

    Wire.begin();
}

Pca9555::~Pca9555() {

}

void Pca9555::begin() {
    //setze alle 16 Pins auf OUTPUT
    this->writeI2CValue(PCA9555_CONFIG    , _configurationRegister_low);
    this->writeI2CValue(PCA9555_CONFIG + 1, _configurationRegister_high);

    //setzte alle 16 Pins auf low
    this->writeI2CValue(PCA9555_OUTPUT    , _valueRegister_low);
    this->writeI2CValue(PCA9555_OUTPUT + 1, _valueRegister_high);
}

void Pca9555::digitalWrite(uint8_t pin, uint8_t value) {
    if (pin > 15) { //falsche Pinnummer!
        return;
    }

    //BITSHIFTING + MASKING
    //Folgeder Code bearbeitet das ValueRegister so, dass sich nur
    //das fuer uns interessante Bit aendert.
    //mittels ~(1 << bit) wird eine Maske erstellt:
    //Wenn das dritte bit 1 sein soll, dann sind alle anderen 0 und umgekehrt
    //mittels bitweise logischen Verknuepfungen (| &) wird diese Maske
    //auf das ValueRegister aufgetragen und nur das fuer uns interessante Bit
    //aender sich
    if (value == 0) {
        //interessantes Bit hat Wert 0, Maske 1, daher &
        //(da beliebiger Wert &1 den beliebigen vorherigen Wert ergibt)
        _valueRegister = _valueRegister & ~(1 << pin);
    } else {
        //interessantes Bit hat Wert 1, Maske 0, daher |
        //(da beliebiger Wert |0 den beliebigen vorherigen Wert ergibt)
        _valueRegister = _valueRegister | (1 << pin);
    }

    //schreibe nun den 16-bit Wert in zwei Schritten (je 8 Bit) ueber I2C
    this->writeI2CValue(PCA9555_OUTPUT    , _valueRegister_low);
    this->writeI2CValue(PCA9555_OUTPUT + 1, _valueRegister_high);
}

void Pca9555::writeI2CValue(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(this->_address);   //Beginne Uebertragung mit Adresse des Empfaengers
    Wire.write(reg);                          //setze das Zielregister
    Wire.write(value);                        //setze den Wert
    Wire.endTransmission();                   //beende die Uebertragung
}
