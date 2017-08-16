#ifndef CONFIG_H
#define CONFIG_H

// In dieser Datei werden Hardgecodete Einstellungen vorgenommen

//SERIAL BAUDRATES
#define SERIAL_LABVIEW_BAUDRATE 115200
#define SERIAL_DEBUG_BAUDRATE 115200
#define SERIAL_MKS_BAUDRATE 115200
#define SERIAL_BUERKERT_BAUDRATE 9600

//SERIAL READ CONFIGS
#define SERIAL_READ_TIMEOUT 1000 //Zeit in ms, die eine Zeilenuebertragung maximal beanspruchen darf
#define SERIAL_READ_MAX_LINE_SIZE 512 //Maximale Laenge einer uebertragenenen Zeile
#define SERIAL_READ_MAX_BLOCK_SIZE 16 //Maximale Laenge eines Eintrages (Zerlegter Uebertragungsstring)
#define SERIAL_READ_MAX_BLOCK_AMOUNT 64 //Maximale Anzahl an Eintraegen pro Zeile

//SD CONFIGS
#define SDCARD_READ_TIMEOUT 250 //Zeit in ms, die eine Zeile maximal zum Einlesen brauchen darf

//OBJECTSIZE
#define MAX_AMOUNT_MFC 32
#define MAX_AMOUNT_VALVE 32
#define MAX_AMOUNT_VALVE_PCB 8

//DISPLAY CONFIGS
#define DISPLAY_SIZE_WIDTH 20
#define DISPLAY_SIZE_HEIGHT 4
#define DISPLAY_REDRAW_INTERVALL 50 //ms, Bestimmt Häufigkeit der Bildaktualisierung, Errors ausgenommen
#define PIN_DISPLAY_RED 37 //pwm
#define PIN_DISPLAY_GREEN 36 //pwm
#define PIN_DISPLAY_BLUE 35 //pwm

//MENU AND BUTTONS
#define MENU_MAX_AMOUNT_ENTRIES 32
#define INPUTS_MAX_AMOUNT_BUTTONS 8

//INPUT OUTUT
#define PIN_DEBUGSWITCH 20 //input
#define PIN_OKBUTTON 16 //input
#define PIN_DOWNBUTTON 15 //input
#define PIN_UPBUTTON 14 //input
#define PIN_DEBUGLED 6 //output
#define PIN_FINISHLED 13 //output

#define PIN_ENABLE_MKS 26 //output
#define PIN_ENABLE_BUERKERT 27 //output

#define PIN_SOFTWARE_RESET 23 //output

//MFC COMMUNICATION TIMINGS
#define MKS_READ_START_TIME 1000 //microseconds
#define MKS_ANSWER_TIMEOUT 5 //milliseconds
#define MKS_READ_TIMEOUT 5 //milliseconds

#define BUERKERT_READ_START_TIME 1000 //microseconds
#define BUERKERT_ANSWER_TIMEOUT 25 //milliseconds
#define BUERKERT_READ_TIMEOUT 25 //milliseconds

//ADDRESSES
#define ADDRESS_I2C_BOSCH 0x76
#define ADDRESS_I2C_DISPLAY 0x38

//ERORRORCODES
#define ERR_SERIAL_READ_MAX_STRING_SIZE 1000
#define ERR_SERIAL_READ_WRONG_LINE_BEGIN 1001
#define ERR_SERIAL_READ_WRONG_LINE_ENDING 1002
#define ERR_SERIAL_READ_TIMEOUT 1003
#define ERR_SERIAL_COMMANDS_WRONG_AMOUNT 1004
#define ERR_SERIAL_UNDEFINED_INDEX 5000

#define ERR_1000_TIME 1500
#define ERR_5000_TIME 604800000 //1 Woche

#endif
