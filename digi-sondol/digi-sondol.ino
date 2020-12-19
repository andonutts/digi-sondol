// Digital Sample-based Sondol

// WAV files converted to code by wav2sketch
#include "AudioSampleClickw.h" // https://freesound.org/people/Druminfected/sounds/250551/
#include "AudioSampleClick_louderw.h"
#include "AudioSampleClick_even_louderw.h"

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
elapsedMillis debugTimer;
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
    playMem1.play(AudioSampleClick_even_louderw);
    audioTimer = 0;
  }
  
#ifndef TEST_MODE
  if (inputTimer > 10) {
    sgtl5000_1.volume(getVolume());
    audioInterval = getAudioInterval();
    inputTimer = 0;
  }
#endif

  if (debugTimer > 1000) {
    Serial.println("------------");
    Serial.print("Volume: ");
    Serial.println(getVolume());
    Serial.print("Audio interval (ms): ");
    Serial.println(audioInterval);
    Serial.println();
    Serial.println("Raw analog readings");
    Serial.print("volPin = ");
    Serial.println(analogRead(volPin));
    Serial.print("freqPin = ");
    Serial.println(analogRead(freqPin));
    debugTimer = 0;
  }
}
