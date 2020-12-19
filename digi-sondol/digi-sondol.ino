// Digital Sample-based Sondol

// WAV files converted to code by wav2sketch
#include "SondolClick.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// when defined, sets constant volume and frequency
//#define TEST_MODE

AudioPlayMemory          playMem1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(playMem1, 0, i2s1, 0);
AudioConnection          patchCord2(playMem1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

int freqPin = A0;
int volPin = A2;

elapsedMillis audioTimer;
elapsedMillis inputTimer;
uint16_t audioInterval;

float getVolume() {
  int volVal = analogRead(volPin);
  return (float)volVal / 1280.0;
}

int getAudioInterval() {
  int intervalVal = analogRead(freqPin);
  return 500 - (int)((float)intervalVal / 2.4);
}

void setup() {
  Serial.begin(9600);
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(getVolume());
  audioInterval = getAudioInterval();

#ifdef TEST_MODE
  sgtl5000_1.volume(0.2);
  audioInterval = 500;
#endif
}

void loop() {
  if (audioTimer > audioInterval) {
    playMem1.play(SondolClick);
    audioTimer = 0;
  }
  
#ifndef TEST_MODE
  if (inputTimer > 10) {
    sgtl5000_1.volume(getVolume());
    audioInterval = getAudioInterval();
    inputTimer = 0;
  }
#endif

}
