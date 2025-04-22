#ifndef DICT_UTILS_H
#define DICT_UTILS_H

struct DictionaryEntry {
  const char* key;
  float value;
};

int getNoteFreq(const char* key);
extern DictionaryEntry myDict[]; // Expose the dictionary

#endif
