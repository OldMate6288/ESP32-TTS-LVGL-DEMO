// Example created by OldMate6288

#include <Arduino.h>
#include <ESP8266SAM.h>
#include <AudioOutputI2S.h>

AudioOutputI2S *out = NULL;

void setup()
{
  Serial.begin(115200);
  out = new AudioOutputI2S(0, 2, 8, -1);
  out->begin();
}

void loop()
{
  
  // Wait for user input
  while (!Serial.available()) {
    delay(10);
  }

  // Read the text from the Serial Monitor
  String userMessage = Serial.readString();
  userMessage.trim(); // Remove leading and trailing whitespaces

  // Check if the user entered text and create SAM instance
  if (userMessage.length() > 0) {
    ESP8266SAM *sam = new ESP8266SAM;
    
    // Convert text to string and speak
    sam->Say(out, userMessage.c_str());
    delay(500);
    delete sam;
  }
}