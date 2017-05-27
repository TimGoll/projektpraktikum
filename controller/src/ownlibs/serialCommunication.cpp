#include "serialCommunication.h"

SerialCommunication::SerialCommunication() {

}
SerialCommunication::~SerialCommunication() {

}

void SerialCommunication::setSerial(HardwareSerial *serial_labView, HardwareSerial *serial_debug, HardwareSerial* serial_uart) {
    this->serial_labView = serial_labView;
    this->serial_debug   = serial_debug;
    this->serial_uart    = serial_uart;

    this->serial_labView->begin(SERIAL_LABVIEW_BAUDRATE);
    this->serial_debug->begin(SERIAL_DEBUG_BAUDRATE);
    this->serial_uart->begin(SERIAL_UART_BAUDRATE);
}

HardwareSerial *SerialCommunication::getType(const char type) {
    HardwareSerial *this_serial;

    if (type == 'L') {
        this_serial = this->serial_labView;
    } else if (type == 'D') {
        this_serial = this->serial_debug;
    } else if (type == 'U') {
        this_serial = this->serial_uart;
    } else {
        this_serial = this->serial_debug;
        this->serial_debug->println("ERROR - Falscher Typ gewaehlt");
    }

    return this_serial;
}

int SerialCommunication::available(const char type) {
    return this->getType(type)->available();
}

int SerialCommunication::read(const char type) {
    return this->getType(type)->read();
}

void SerialCommunication::print(const char type, String &input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, const char input[]) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, const char input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, unsigned char input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, int input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, unsigned int input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, long input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, unsigned long input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(const char type, double input) {
    this->getType(type)->print(input);
}




void SerialCommunication::println(const char type, String &input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, const char input[]) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, const char input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, unsigned char input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, int input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, unsigned int input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, long input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, unsigned long input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(const char type, double input) {
    this->getType(type)->println(input);
}

// Initialisiere die Serielle Kommunikation
SerialCommunication *srl = new SerialCommunication();
