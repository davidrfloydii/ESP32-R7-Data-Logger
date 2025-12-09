#pragma once
#include "globals.h"

namespace sdHelper
{
    void sdInitialize()
    {
        return;
    }

    bool isSdAvailable()
    {
        // TODO: Implement actual SD card availability check
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
}