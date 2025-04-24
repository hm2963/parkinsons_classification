#include "sound.h"
#include <Arduino.h>
#include "music_dict.h"

const int speaker = 5;
const char* dk_notes[3] = {"E", "F", "E"};
const char* tm_notes[3] = {"C", "A#", "F#"}; 
const char* n_notes[3] = {"C", "E", "G"}; //major chord for happy sound
int numNotes = 3;

unsigned long lastNoteTime = 0;
int noteDuration = 200;         // ms
int pauseBetweenNotes = 50;     // ms
int intervalBetweenSequences = 1000; // ms between *sequences*

int currentNoteIndex = 0;
bool isPlayingSequence = false;
bool waitingForNextSequence = false;
unsigned long sequenceEndTime = 0;

void initSound() {
  pinMode(speaker, OUTPUT);
}

void sequencing(const char* notes[]){
  unsigned long now = millis();

  // Start a new sequence if we're not playing and not waiting
  if (!isPlayingSequence && !waitingForNextSequence) {
    isPlayingSequence = true;
    currentNoteIndex = 0;
    lastNoteTime = now;
    int freq = getNoteFreq(notes[currentNoteIndex]);
    tone(speaker, freq, noteDuration);
  }

  // Continue sequence
  if (isPlayingSequence && now - lastNoteTime >= noteDuration + pauseBetweenNotes) {
    currentNoteIndex++;
    if (currentNoteIndex < numNotes) {
      lastNoteTime = now;
      int freq = getNoteFreq(notes[currentNoteIndex]);
      tone(speaker, freq, noteDuration);
    } else {
      isPlayingSequence = false;
      waitingForNextSequence = true;
      sequenceEndTime = now;
    }
  }

  // Wait between sequences
  if (waitingForNextSequence && now - sequenceEndTime >= intervalBetweenSequences) {
    waitingForNextSequence = false;
  }
}

void handleSoundDK(float intensity) {
  sequencing(dk_notes);
}



void handleSoundTM(float intensity) { //replace function name and logic
  sequencing(tm_notes);
}

void handleSoundN(float intensity) { //replace function name and logic
  sequencing(n_notes);
}
