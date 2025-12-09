#pragma once
#include "globals.h"

SdFat SD;
const int chipSelect = 5;

namespace sdHelper
{
    void createFileAndWriteHeader()
    {
        logFile.open(currentFileName.c_str(), O_RDWR | O_CREAT | O_AT_END);
        if (!logFile)
        {
            debugln("Error creating log file!");
            return;
        }
        else
        {
            debugln("Log file created successfully.");
            logFile.println("Latitude,Longitude,Altitude(m),GroundSpeed,DateTimeUTC");
        }
    }

    void sdInitialize()
    {
        debugln("Initializing SD card...");
        // Initialize SdFat or print a detailed error message and halt
        // Use half speed like the native library.
        // change to SPI_FULL_SPEED for more performance.
        if (!sd.begin(chipSelect, SPI_HALF_SPEED))
            sd.initErrorHalt();
        createFileAndWriteHeader();
        return;
    }

    bool isSdAvailable()
    {
        return true;
    }
    // Called to close the logging file. **NOTE: The design intent is that only one file is open at a time.
    void closeFile()
    {
        debugln("Closing log file if open.");
        if (logFile)
        {
            logFile.close();
            debugln("Log file closed.");
        }
    }

    void appendToFile(const String &dataLine)
    {
        logFile.println(dataLine);
    }
}