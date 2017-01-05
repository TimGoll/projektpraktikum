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

#define DISPLAY_SIZE_WIDTH 20
#define DISPLAY_SIZE_HEIGHT 4
#define DISPLAY_REDRAW_INTERVALL 50 //Bestimmt Häufigkeit der Bildaktualisierung, Errors ausgenommen

#define ERR_1000_TIME 1500
#define ERR_5000_TIME 604800000 //1 Woche

//ERORRORCODES
#define ERR_SERIAL_READ_MAX_STRING_SIZE 1000
#define ERR_SERIAL_READ_WRONG_LINE_BEGIN 1001
#define ERR_SERIAL_READ_WRONG_LINE_ENDING 1002
#define ERR_SERIAL_READ_TIMEOUT 1003

#define ERR_SERIAL_UNDEFINED_INDEX 5000

#endif
