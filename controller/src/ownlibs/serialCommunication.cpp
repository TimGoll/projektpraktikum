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

HardwareSerial *SerialCommunication::getType(char type) {
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

void SerialCommunication::print(char type, String &input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, char input[]) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, char input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, unsigned char input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, int input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, unsigned int input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, long input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, unsigned long input) {
    this->getType(type)->print(input);
}

void SerialCommunication::print(char type, double input) {
    this->getType(type)->print(input);
}




void SerialCommunication::println(char type, String &input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, char input[]) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, char input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, unsigned char input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, int input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, unsigned int input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, long input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, unsigned long input) {
    this->getType(type)->println(input);
}

void SerialCommunication::println(char type, double input) {
    this->getType(type)->println(input);
}

// Initialisiere die Serielle Kommunikation
SerialCommunication *srl = new SerialCommunication();
