#include <Arduino.h>
#include <string.h>  // for strcmp
#include "music_dict.h"

// Define your dictionary entries here
DictionaryEntry myDict[] = {
  {"C", 523.25},
  {"D", 587.33},
  {"E", 659.25},
  {"F", 698.46},
  {"G", 783.99},
  {"A", 880},
  {"B",987.77}
};

// Lookup function
int getNoteFreq(const char* key) {
  int dictSize = sizeof(myDict) / sizeof(myDict[0]);
  for (int i = 0; i < dictSize; i++) {
    if (strcmp(myDict[i].key, key) == 0) {
      return myDict[i].value;
    }
  }
  return -1;  // or another sentinel for "not found"
}
