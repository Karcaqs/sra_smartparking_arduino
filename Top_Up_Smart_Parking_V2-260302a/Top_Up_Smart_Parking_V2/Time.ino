void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void initTime() {
  sntp_set_time_sync_notification_cb(timeavailable);
  
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  lcdInitTime();

  buzzerOn();
  struct tm timeinfo;
  int count = 0;
  while (!getLocalTime(&timeinfo)) {
    Serial.println(F("Failed to obtain time"));
    if (count >= 3) {
      delay(3000);
      ESP.restart();
    }
    count++;
  }
  buzzerOn();
  rtc.setTimeStruct(timeinfo);
  rtc1.setTimeStruct(timeinfo);
  rtc2.setTimeStruct(timeinfo);
}

void timeCheck() {
  timestamp = rtc.getLocalEpoch();
}

void timeOrder() {
  timestamp = rtc.getLocalEpoch();
  timesExpired = rtc1.getLocalEpoch();

  expired = rtc1.getTime("%FT%TZ");
  Serial.println(expired);

  referenceId = String(idDevice) + "_" + String(uidCard) + "_" + String(timestamp);
}
