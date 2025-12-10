#pragma once
#include "globals.h"
// Define a large buffer size for characters (e.g., 4KB)
const size_t BUFFER_SIZE_CHARS = 4096;
char writeBuffer[BUFFER_SIZE_CHARS];
// Keep track of how much space is left in the buffer
size_t bufferOccupied = 0;
unsigned long logEntryCount = 0;
char tempLine[128]; // Small buffer for formatting a single line

namespace sdHelper
{
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

        // Initialize SdFat or print a detailed error message and halt
        // Use half speed like the native library.
        // change to SPI_FULL_SPEED for more performance.
        if (!SD_MMC.begin("/sdcard", false, false, 40000, 5))
        {
            debugln("SD Card Mount Failed");
            return;
        }
        uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
        debugf("SD_MMC mounted, size: %llu MB\n", cardSize);
        if (cardSize == 0)
        {
            debugln("Card size is 0 MB. SD Card not available.");
            return;
        }
        else
        {
            debugln("SD Card is available.");
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