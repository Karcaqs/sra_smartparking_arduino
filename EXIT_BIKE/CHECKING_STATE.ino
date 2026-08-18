unsigned long loopEntryMillis = 0;
void checkLoopDetector() {
  if (digitalRead(LOOP_ENTRY_PIN) == LOW) {
    isLoopEntryOn = true;
    loopEntryMillis = 0;

  } else {
    isLoopEntryOn = false;
    // Serial.println("loop entry False");
    // Mulai menghitung waktu saat pertama kali LOOP_ENTRY_PIN HIGH
    if (loopEntryMillis == 0) {
      loopEntryMillis = millis();
    }

    // Jika 3 detik telah berlalu saat LOOP_ENTRY_PIN HIGH, set isEnableEntry menjadi true
    if (millis() - loopEntryMillis >= 3000) {
      isEnableEntry = true;
    }
  }

}
