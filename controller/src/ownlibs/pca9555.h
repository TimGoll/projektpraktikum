#ifndef PCA9555_H
#define PCA9555_H

#include <Arduino.h>
#include <Wire.h>

//definiere Register (hier nur 2(+1) und 6(+1) genutzt)
//Quelle: http://www.ti.com/lit/ds/symlink/pca9555.pdf
#define PCA9555_INPUT      0
#define PCA9555_OUTPUT     2
#define PCA9555_INVERT     4
#define PCA9555_CONFIG     6

class Pca9555 {
public:
    //Defaultconstructor
    Pca9555(uint8_t address);
    //Destructor
    ~Pca9555();
    //Setzt alle 16 Pins des IO Expanders auf OUTPUT mit Level 0
    void begin();
    //globale Pin-Funktion, laesst sich benutzen wie Standard digitalWrite
    void digitalWrite(uint8_t pin, uint8_t value);
private:
    //schreibe 8-bit Wert ueber I2C
    void writeI2CValue(uint8_t reg, uint8_t value);

    //Die 'configurationRegister' Variable (16-bit) ist unterteilt in zwei 8-bit Variablen,
    //da jede Uebertragung nur 8 Bit umfassen kann. Das Gleiche gilt fuer 'valueRegister'
    //low und high steht hier fuer die Position im 16-bit Bereich
    union {
        struct {
            uint8_t _configurationRegister_low;
            uint8_t _configurationRegister_high;
        };
        uint16_t _configurationRegister;
    };
    union {
        struct {
            uint8_t _valueRegister_low;
            uint8_t _valueRegister_high;
        };
        uint16_t _valueRegister;
    };

    uint8_t _address;
};

#endif
