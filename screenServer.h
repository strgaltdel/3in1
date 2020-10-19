// Reads a screen image off the TFT and send it to a processing client sketch
// over the serial port. The TFT screen must be readable

// It is assumed that the Due Native USB Port will be used

// This sketch has been created to work with an Arduino Due and the library here:
// https://github.com/prenticedavid/MCUFRIEND_kbv

// Created by: Bodmer 5/3/17

// MIT licence applies, all text above must be included in derivative works

#define DUMP_BAUD_RATE 115200 // Rate used for screen dumps (don't care for Native Port)

#define PIXEL_TIMEOUT 100    // 100ms Time-out between pixel requests
#define START_TIMEOUT 20000  // 20s Maximum time to wait for start of transfer

#define NPIXELS 4 // Number of pixels to send in a burst (minimum of 1) must be integer division of tft width



// Screenshot serial port server (Processing sketch acts as client)
boolean serialScreenServer(void)
{

  // Precautionary receive buffer garbage flush for 50ms
  uint32_t clearTime = millis() + 50;
  while ( millis() < clearTime ) {
        Serial.read();
  }

  boolean wait = true;
  uint32_t lastCmdTime = millis();     // Initialise start of command time-out

  // Wait for the starting flag with a start time-out
  while (wait)
  {
    // Check serial buffer
    if (Serial.available() > 0) {
      // Read the command byte
      uint8_t cmd = Serial.read();
      // If it is 'S' (start command) then clear the serial buffer for 50ms and stop waiting
      if ( cmd == 'S' ) {
        // Precautionary receive buffer garbage flush for 50ms
        clearTime = millis() + 50;
        while ( millis() < clearTime ) {
          Serial.read();
        }
        wait = false;           // No need to wait anymore
        lastCmdTime = millis(); // Set last received command time

        // Send screen size
        Serial.write('W');
        Serial.write(tft.width()  >> 8);
        Serial.write(tft.width()  & 0xFF);
        Serial.write('H');
        Serial.write(tft.height() >> 8);
        Serial.write(tft.height() & 0xFF);
        Serial.write('Y');
      }
    }
    else
    {
      // Check for time-out
      if ( millis() > lastCmdTime + START_TIMEOUT){
		  Serial.println("no dump success");
	   return false;
	   }
    }
  }

  uint16_t color[NPIXELS]; // 565 color buffer for N pixels

  // Send all the pixels on the whole screen
  for ( uint32_t y = 0; y < tft.height(); y++)
  {
    // Increment x by NPIXELS as we send NPIXELS for every byte received
    for ( uint32_t x = 0; x < tft.width(); x += NPIXELS)
    {
      // Wait here for serial data to arrive or a time-out elapses
      while ( Serial.available() == 0 )
      {
        if ( millis() > lastCmdTime + PIXEL_TIMEOUT) return false;
      }

      // Serial data must be available to get here, read 1 byte and
      // respond with N pixels
      if ( Serial.read() == 'X' ) {
        // X command byte means abort, so clear the buffer and return
        clearTime = millis() + 50;
        while ( millis() < clearTime ) Serial.read();
        return false;
      }

      // Save arrival time of the read command (for later time-out check)
      lastCmdTime = millis();

      // Fetch N pixels from x,y
      tft.readGRAM(x, y, color, NPIXELS, 1);

      // Convert 565 colour values to 3 RGB bytes in a buffer
      //uint8_t rgbBuffer[NPIXELS * 3];
      //for (int i = 0; i < NPIXELS; i++)
      //{
      //  rgbBuffer[i * 3 + 0] = (uint8_t)((color[i] & 0xF800) >> 8); // Pixel red
      //  rgbBuffer[i * 3 + 1] = (uint8_t)((color[i] & 0x07E0) >> 3); // Pixel green
      //  rgbBuffer[i * 3 + 2] = (uint8_t)((color[i] & 0x001F) << 3); // Pixel blue
      //}

      // Send 565 colour buffer to client
      Serial.write((uint8_t*)color, NPIXELS * 2);
    }
  }

  // Receive buffer excess command flush for 50ms
  clearTime = millis() + 50;
  while ( millis() < clearTime ) Serial.read();

  return true;
}

// Start a screen dump server (serial or network)
boolean screenServer(void)
{
  Serial.begin(DUMP_BAUD_RATE); // Set baud rate

  boolean result = serialScreenServer(); // Screenshot serial port server

  //Serial.println();
  //if (result) Serial.println(F("Screen dump passed :-)"));
  //else        Serial.println(F("Screen dump failed :-("));

  return result;
}

