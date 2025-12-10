#pragma once
#include "SD_MMC.h"
#define CAN_RX 16
#define CAN_TX 17


File logFile;
String currentFileName;

#define DEBUG 1
// Conditional definition for debugging if DEBUG is 1 then it will print to serial port.
// If DEBUG = 0 then the lines will be removed by the compiler.
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf("%f", x)
#define debugg(x, y, z) Serial.printf(x, y, z)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#define debugg(x, y, z)
#endif

unsigned long lastTime = 0;    // Simple local timer. Limits amount if I2C traffic to u-blox module.
unsigned long startTime = 0;   // Used to calc the actual update rate.
unsigned long updateCount = 0; // Used to calc the actual update rate.