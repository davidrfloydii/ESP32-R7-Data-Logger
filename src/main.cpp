#include <Arduino.h>
#include "globals.h"
#include "gnssHelper.h"

#define MODE_SELECTION_PIN 32 // GPIO pin for mode selection button

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
    while (!Serial);
      ; // Intentiional holding of the application. Disable DEBUG in globals.h to run without serial connection.
  #endif

  // Setup input buttons so that control for the startup sequence is possible.
  pinMode(MODE_SELECTION_PIN, INPUT_PULLUP);

  // Start by initializing the GNSS module.
  gnssHelper::gnssInitialize();
}

void loop()
{
  gnssHelper::originalLoop();
}