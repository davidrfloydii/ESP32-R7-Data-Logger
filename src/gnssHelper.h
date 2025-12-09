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

    int fixMode()
    {
        return myGNSS.getFixType();
    }

    void setCurrentFileName()
    {
        // Create a filename based on the current date and time from GNSS
        int year = myGNSS.getYear();
        int month = myGNSS.getMonth();
        int day = myGNSS.getDay();
        int hour = myGNSS.getHour();
        int minute = myGNSS.getMinute();
        int second = myGNSS.getSecond();

        // Format: LOG_YYYYMMDD_HHMMSS.CSV
        currentFileName = "LOG_";
        if (year < 2000)
        {
            currentFileName += "20";
        }
        else
        {
            currentFileName += String(year);
        }
        if (month < 10)
        {
            currentFileName += "0";
        }
        currentFileName += String(month);
        if (day < 10)
        {
            currentFileName += "0";
        }
        currentFileName += String(day);
        currentFileName += "_";
        if (hour < 10)
        {
            currentFileName += "0";
        }
        currentFileName += String(hour);
        if (minute < 10)
        {
            currentFileName += "0";
        }
        currentFileName += String(minute);
        if (second < 10)
        {
            currentFileName += "0";
        }
        currentFileName += String(second);
        currentFileName += ".CSV";
        debug("Log file name set to: ");
        debugln(currentFileName);
    }

    String getGnssDataString()
    {
        String myGpString = "";
        lastTime = millis(); // Update the timer
        float latitude = (myGNSS.getLatitude() / 10000000.0);
        float longitude = (myGNSS.getLongitude() / 10000000.0);
        uint32_t altitude = myGNSS.getAltitude();
        uint32_t epoch = myGNSS.getUnixEpoch();
        uint32_t groundSpeed = (myGNSS.getGroundSpeed() * 0.00223694); // Convert from mm/s to mph
        char bufferLat[15];
        char bufferLon[15];
        dtostrf(latitude, 10, 7, bufferLat);
        dtostrf(longitude, 11, 7, bufferLon);
        myGpString += String(bufferLat);
        myGpString += ",";
        myGpString += String(bufferLon);
        myGpString += ",";
        myGpString += String(altitude);
        myGpString += ",";
        myGpString += String(groundSpeed);
        myGpString += ",";
        // Format the date and time into a single string
        uint16_t year = myGNSS.getYear();
        uint8_t month = myGNSS.getMonth();
        uint8_t day = myGNSS.getDay();
        uint8_t hour = myGNSS.getHour();
        uint8_t minute = myGNSS.getMinute();
        uint8_t second = myGNSS.getSecond();
        myGpString += String(year) + "-" + String(month) + "-" + String(day) + +" " + String(hour) + ":" + String(minute) + ":" + String(second) + "." + String(epoch * 1000000);
        return myGpString;
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