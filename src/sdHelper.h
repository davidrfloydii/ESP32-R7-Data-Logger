#pragma once
#include "globals.h"
#define ONE_BIT_MODE false
// Define a large buffer size for characters (e.g., 4KB)
const size_t BUFFER_SIZE_CHARS = 4096;
char writeBuffer[BUFFER_SIZE_CHARS];
// Keep track of how much space is left in the buffer
size_t bufferOccupied = 0;
unsigned long logEntryCount = 0;
char tempLine[128]; // Small buffer for formatting a single line

namespace sdHelper
{

    // These are the pins reserved for ESP32 WROOM modules SD_MMC
    // *NOTE: A button to pull IO2 to ground is required to flash.
    int clk = 14;
    int cmd = 15;
    int d0 = 2;
    int d1 = 4;
    int d2 = 12;
    int d3 = 13;

    void createFileAndWriteHeader()
    {
        logFile = SD_MMC.open(currentFileName.c_str(), FILE_WRITE);
        if (!logFile)
        {
            debugln("Failed to open log file");
            while (1)
                ;
        }
        logFile.print("Latitude,Longitude,Altitude,GroundSpeed,DateTimeUTC,MicroSeconds,RefreshRate");
        logFile.print("\n");
    }

    void sdInitialize()
    {
        debugln("Initializing SD card...");
        pinMode(clk, INPUT_PULLUP);
        pinMode(cmd, INPUT_PULLUP);
        pinMode(d1, INPUT_PULLUP);
        pinMode(d2, INPUT_PULLUP);
        pinMode(d3, INPUT_PULLUP);
        // Initialize SdFat or print a detailed error message and halt
        // Use half speed like the native library.
        // change to SPI_FULL_SPEED for more performance.
        if (!SD_MMC.begin("/sdcard", ONE_BIT_MODE, false, SDMMC_FREQ_HIGHSPEED, 1))
        {
            debugln("SD Card Mount Failed");
            return;
        }
        return;
    }

    bool isSdAvailable()
    {
        bool isCardTypeNone = SD_MMC.cardType() != CARD_NONE;
        return isCardTypeNone;
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

    // Function to add a formatted string (a single line) to the buffer
    void addLineToBufferAndFlush(const char *line)
    {
        size_t lineLen = strlen(line);

        // If the new line won't fit entirely in the remaining buffer space
        if (bufferOccupied + lineLen >= BUFFER_SIZE_CHARS)
        {
            // Flush the current buffer content to the SD card first
            logFile.write((uint8_t *)writeBuffer, bufferOccupied);
            bufferOccupied = 0; // Reset the buffer position
        }

        // Append the new line to the buffer
        memcpy(&writeBuffer[bufferOccupied], line, lineLen);
        bufferOccupied += lineLen;
    }

    // Function to ensure all remaining data is written
    void flushRemaining()
    {
        if (bufferOccupied > 0)
        {
            logFile.write((uint8_t *)writeBuffer, bufferOccupied);
            bufferOccupied = 0;
        }
        logFile.flush();
    }

    void appendToFile(const String &dataLine)
    {
        addLineToBufferAndFlush(dataLine.c_str());
        logEntryCount++;
        if (logEntryCount % 100 == 0)
        {
            flushRemaining();
        }
    }
}