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

    int available(char type); //returns number of bytes in buffer
    int read(char type); //Returns the first byte of incoming serial data available (or -1 if no data is available)

    //Serial_print Funktionen fuer alle Datentypen
    void print(const char type, String &input);
    void print(const char type, const char input[]);
    void print(const char type, const char input);
    void print(const char type, unsigned char input);
    void print(const char type, int input);
    void print(const char type, unsigned int input);
    void print(const char type, long input);
    void print(const char type, unsigned long input);
    void print(const char type, double input);

    void println(const char type, String &input);
    void println(const char type, const char input[]);
    void println(const char type, const char input);
    void println(const char type, unsigned char input);
    void println(const char type, int input);
    void println(const char type, unsigned int input);
    void println(const char type, long input);
    void println(const char type, unsigned long input);
    void println(const char type, double input);

private:
    HardwareSerial *getType(const char type);
    HardwareSerial *serial_labView;
    HardwareSerial *serial_debug;
    HardwareSerial *serial_uart;
};

extern SerialCommunication *srl;

#endif
