#pragma once
#include "globals.h"
#include <SPI.h>

SdFat SD;
#define SD_FAT_TYPE 32 // Use 32-bit SD FAT type
#define SD_CS_PIN 5   // GPIO pin for SD card chip select


namespace sdHelper
{
    void sdInitialize()
    {
        debugln("Initializing SD card...");
        if (!SD.begin()) {
            debugln("SD Card initialization failed!");
            return;
        }
        debugln("SD Card initialized successfully.");
        debugln("SD Card Type: FAT" + String(SD_FAT_TYPE));
        debugln("SD Card CS Pin: " + String(SD_CS_PIN));
        debugln("SD Card is ready for use.");
        debugln("SD Card Volume Size (MB): " + String(SD.vol()->clusterCount() * SD.vol()->bytesPerCluster() / (1024 * 1024))); 
        delay(5000);
        return;
    }

    bool isSdAvailable()
    {
        return true;
    }
    // Called to close the logging file. **NOTE: The design intent is that only one file is open at a time.
    void closeFile() {
        debugln("Closing log file if open.");
        if (logFile) {
            logFile.close();
            debugln("Log file closed.");
        }
    }

    void createFileAndWriteHeader() {
        logFile = SD.open(currentFileName.c_str(),FILE_WRITE);
        if (!logFile) {
            debugln("Error creating log file!");
            return;
        }
        else {
            debugln("Log file created successfully.");
            logFile.println("Time(ms),Latitude,Longitude,Altitude(m),Fix Type,Satellites");
            logFile.flush(); // Ensure header is written to the SD card
        }
    }
}