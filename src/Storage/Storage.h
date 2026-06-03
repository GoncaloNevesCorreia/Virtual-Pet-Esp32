#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <Preferences.h>

namespace Storage {

void init();

void save(const char* key, uint8_t value);

uint8_t load(const char* key);

void saveCredentials(const char* key, String value);

String loadCredentials(const char* key);

}  // namespace Storage

#endif