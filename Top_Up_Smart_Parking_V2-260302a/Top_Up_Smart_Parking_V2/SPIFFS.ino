void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println(F("SPIFFS initialisation failed!"));
    while (1) yield();
  }
  float totalMB = SPIFFS.totalBytes() / 1048576.0;
  float usedMB  = SPIFFS.usedBytes() / 1048576.0;
  float freeMB  = totalMB - usedMB;

  Serial.printf("Total: %.2f MB\n", totalMB);
  Serial.printf("Used : %.2f MB\n", usedMB);
  Serial.printf("Free : %.2f MB\n", freeMB);
}

void initLittleFS() {
  bool fsOK = LittleFS.begin(true);
  if (!fsOK) {
    Serial.println(F("[FS] LittleFS mount failed"));
  }
}
