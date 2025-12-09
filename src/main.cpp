#include <Arduino.h>
#include "globals.h"
#include "gnssHelper.h"
#include "sdHelper.h"
// PIN HIGH IS "OFF" or Access Point Mode, PIN LOW IS "ON" or Recording mode.
#define MODE_SELECTION_PIN 15 // GPIO pin for mode selection button
bool isRecordingActive = false;

void setup()
{
  // Always start serial first
  Serial.begin(500000);
// Before we do anything else we check if debugging is enabled. Just in case we write out that it's not
// this is helpful if you forget to set the flag and are looking but seeing no serial data.
#if DEBUG == 0
  // This will be the only line that gets printed to the serial port if debugging is disabled.
  Serial.println("Debugging disabled");
#else
  // If debugging is wait hold up starting until the serial port is connected.
  // Helpful to ensure you see all messages. Also helpful to ensure it gets turned off in production
  // environments since the app will not run without the connection.
  while (!Serial)
    ;
  ; // Intentiional holding of the application. Disable DEBUG in globals.h to run without serial connection.
#endif

  // Setup input buttons so that control for the startup sequence is possible.
  pinMode(MODE_SELECTION_PIN, INPUT_PULLUP);

  // Start by initializing the GNSS module.
  gnssHelper::gnssInitialize();
  while (gnssHelper::fixMode() < 3)
  {
    // Wait here until we have a 3D fix.
    debugln("Waiting for 3D fix...");
    delay(1000);
  }
  debugln("3D Fix acquired.");
  // Now that we have a fix, let's verify we have an SD Card available.
  // Start by initializing the SD Card helper.
  sdHelper::sdInitialize();
  if (!sdHelper::isSdAvailable())
  {
    debugln("SD Card not available. Halting execution.");
    while (1)
      ; // Halt execution if SD Card is not available.
  }
  else
  {
    debugln("SD Card is available.");
  }
  // Now that we have a fix and the SD Card is available we can set the current file name.
  gnssHelper::setCurrentFileName();
  sdHelper::createFileAndWriteHeader();
}

// The main loop handles two modes of operation: Recording mode and Access Point mode.
void loop()
{
  int modeButtonState = digitalRead(MODE_SELECTION_PIN);
  if (modeButtonState == LOW)
  {
    isRecordingActive = true;
    // Recording mode
    debugln("Recording mode selected");
    gnssHelper::originalLoop();
  }
  else
  {
    // Not recording so we need to check to see if recording was active before we do anything else.
    // If it was active we need to finalize the file before proceeding.
    if (isRecordingActive)
    {
      sdHelper::closeFile();
    }
    isRecordingActive = false;
    // Access Point mode
    debugln("Access Point mode selected");
  }
}