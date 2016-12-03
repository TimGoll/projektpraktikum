#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <Arduino.h>
#include "../config.h"

class SerialCommunication {
public:
    //Defaultconstructor
    SerialCommunication();
    //Destructor
    ~SerialCommunication();

    void setSerial(HardwareSerial *serial_labView, HardwareSerial *serial_debug, HardwareSerial* serial_uart);

    //Serial_print Funktionen fuer alle Datentypen
    void print(char type, String &input);
    void print(char type, char input[]);
    void print(char type, char input);
    void print(char type, unsigned char input);
    void print(char type, int input);
    void print(char type, unsigned int input);
    void print(char type, long input);
    void print(char type, unsigned long input);
    void print(char type, double input);

    void println(char type, String &input);
    void println(char type, char input[]);
    void println(char type, char input);
    void println(char type, unsigned char input);
    void println(char type, int input);
    void println(char type, unsigned int input);
    void println(char type, long input);
    void println(char type, unsigned long input);
    void println(char type, double input);

private:
    HardwareSerial *getType(char type);
    HardwareSerial *serial_labView;
    HardwareSerial *serial_debug;
    HardwareSerial *serial_uart;
};

extern SerialCommunication *srl;

#endif
