// Digital Sample-based Sondol

// WAV files converted to code by wav2sketch
#include "AudioSampleClick.h" // https://freesound.org/people/Druminfected/sounds/250551/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

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
int audioInterval;

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
}

void loop() {
  if (audioTimer > audioInterval) {
    playMem1.play(AudioSampleClick);
    audioTimer = 0;
  }
  if (inputTimer > 100) {
    sgtl5000_1.volume(getVolume());
    audioInterval = getAudioInterval();
    inputTimer = 0;
  }
  if (debugTimer > 1000) {
    Serial.println("input info");
    Serial.println(getVolume());
    Serial.println(audioInterval);
    Serial.println();
    debugTimer = 0;
  }
}
