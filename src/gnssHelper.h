#pragma once
#include "globals.h"
#include <Wire.h> //Needed for I2C to GNSS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

SFE_UBLOX_GNSS myGNSS;

namespace gnssHelper
{
    void gnssInitialize()
    {
        // Start I2C communication
        Wire.begin();
        // Increase I2C clock speed to 400kHz to cope with the high navigation rate
        // (We normally recommend running the bus at 100kHz)
        Wire.setClock(400000);

        if (myGNSS.begin() == false) // Connect to the u-blox module using Wire port
        {
            debugln(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
            while (1)
                ;
        }
        myGNSS.setI2COutput(COM_TYPE_UBX); // Set the I2C port to output UBX only (turn off NMEA noise)
        myGNSS.setNavigationFrequency(25); // Set output to 5 times a second

        uint8_t rate = myGNSS.getNavigationFrequency(); // Get the update rate of this module
        debug("Current update rate: ");
        debugln(rate);

        startTime = millis();
    }

    void originalLoop()
    {
        // Query module every 25 ms. Doing it more often will just cause I2C traffic.
        // The module only responds when a new position is available. This is defined
        // by the update freq.
        if (millis() - lastTime > 25)
        {
            lastTime = millis(); // Update the timer

            long latitude = myGNSS.getLatitude();
            Serial.print(F("Lat: "));
            Serial.print(latitude);

            long longitude = myGNSS.getLongitude();
            Serial.print(F(" Long: "));
            Serial.print(longitude);

            updateCount++;

            // Calculate the actual update rate based on the sketch start time and the
            // number of updates we've received.
            Serial.print(F(" Rate: "));
            Serial.print(updateCount / ((millis() - startTime) / 1000.0), 2);
            Serial.print(F("Hz"));

            Serial.println();
        }
    }
} // namespace gnssHelper