#include "Storage.h"

namespace Storage {

Preferences preferences;

void init() {
  preferences.begin("virtual_pet", false);
}

void save(const char* key, uint8_t value) {
  preferences.putUInt(key, value);
}

uint8_t load(const char* key) {
  return preferences.getUInt(key, 100);
}

void saveCredentials(const char* key, String value) {
  preferences.putString(key, value);
}

String loadCredentials(const char* key) {
  return preferences.getString(key);
}

}  // namespace Storage