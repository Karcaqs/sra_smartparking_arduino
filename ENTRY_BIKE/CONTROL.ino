void controlUp(bool val) {
  digitalWrite(UP_PIN, val);
}
void controlDown(bool val) {
  digitalWrite(DOWN_PIN, val);
}

void controlFan(bool val) {
  digitalWrite(FAN_PIN, val);
}

void openGate() {
  isEnableEntry = false;
  myDFPlayer.play(4);
  //  Serial.println("SILAHKAN MASUK");
  controlUp(true);
  delay(300);
  controlUp(false);
}

void closeGate() {
  controlDown(true);
  delay(300);
  controlDown(false);
  delay(500);
  isEnableEntry = true;
  playOnce = true;
}
