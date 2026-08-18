void logActivity(String message, String level = "INFO") {
  String timestamp = rtc2.getTime("%Y-%m-%d %H:%M:%S");
  String logMsg = "[" + timestamp + "] [" + level + "] " + message;
  Serial.println(logMsg);
  
  // Optional: Send to Telegram if level is CRITICAL or WARN
  if (level == "CRITICAL" || level == "ERROR") {
    myBot.sendTo(CHAT_ID, "🚨 *" + level + "*\n" + escapeMarkdown(message));
  }
}

void reportSystemHealth() {
  String uptimeStr = String(millis() / 1000) + "s";
  int rssi = WiFi.RSSI();
  uint32_t freeHeap = ESP.getFreeHeap();

  String msg = "📊 *System Health Report*\n";
  msg += "⏱️ Uptime: `" + uptimeStr + "`\n";
  msg += "📶 WiFi RSSI: `" + String(rssi) + " dBm`\n";
  msg += "💾 Free Heap: `" + String(freeHeap) + " bytes`\n";
  msg += "🕒 Time: `" + rtc2.getTime("%Y-%m-%d %H:%M:%S") + "`";

  logActivity("Health Report: RSSI " + String(rssi) + ", Heap " + String(freeHeap));
  myBot.sendTo(CHAT_ID, escapeMarkdown(msg));
}

void handleStateChange(AppState newState) {
  if (currentState != newState) {
    logActivity("State Change: " + String(appStateToString(currentState)) + " -> " + String(appStateToString(newState)));
  }
}
