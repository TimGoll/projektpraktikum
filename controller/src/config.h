#ifndef CONFIG_H
#define CONFIG_H

// In dieser Datei werden Hardgecodete Einstellungen vorgenommen

#define SERIAL_LABVIEW_BAUDRATE 115200
#define SERIAL_DEBUG_BAUDRATE 115200
#define SERIAL_UART_BAUDRATE 115200

#define SERIAL_READ_TIMEOUT 1000 //Zeit in ms, die eine Zeilenuebertragung maximal beanspruchen darf
#define SERIAL_READ_MAX_LINE_SIZE 512 //Maximale Laenge einer uebertragenenen Zeile
#define SERIAL_READ_MAX_BLOCK_SIZE 16 //Maximale Laenge eines Eintrages (Zerlegter Uebertragungsstring)
#define SERIAL_READ_MAX_BLOCK_AMOUNT 32 //Maximale Anzahl an Eintraegen pro Zeile

#define MAX_AMOUNT_MFC 16
#define MAX_AMOUNT_VALVE 16
#define MAX_AMOUNT_VALVE_PCB 8

#define MAX_SD_FILE_SIZE 52428800 //bytes, entspricht 50 MB
//obsolet, da bytesize eingeführt
// #define MAX_ROW_BYTES 4 // mit value = x_2*255^2 + x_1*255 + x_0    wobei 0 <= x_i <= 255 und 0 <= value <= (255^3 - 1) lässt sich value mit den ASCII-Zeichen ascii[x_2],ascii[x_1],ascii[x_0] darstellen
#define MAX_FLOAT_POINT_SHIFT 2
//#define SEPERATIONCHAR ";"

#define DISPLAY_SIZE_WIDTH 20
#define DISPLAY_SIZE_HEIGHT 4
#define DISPLAY_REDRAW_INTERVALL 50 //Bestimmt Häufigkeit der Bildaktualisierung, Errors ausgenommen

#define ERR_1000_TIME 1500
#define ERR_5000_TIME 604800000 //1 Woche

#define PIN_DEBUGSWITCH 5
#define PIN_STARTBUTTON 6

//ERORRORCODES
#define ERR_SERIAL_READ_MAX_STRING_SIZE 1000
#define ERR_SERIAL_READ_WRONG_LINE_BEGIN 1001
#define ERR_SERIAL_READ_WRONG_LINE_ENDING 1002
#define ERR_SERIAL_READ_TIMEOUT 1003

#define ERR_SERIAL_UNDEFINED_INDEX 5000

#endif
