void controlUp(bool val) {
  digitalWrite(relay1, val);
}
void controlDown(bool val) {
  digitalWrite(relay2, val);
}
void controlPause(bool val) {
  digitalWrite(relay3, val);
}

void openGate() {
  Serial.println("SILAHKAN LEWAT");
  
  controlUp(true);
  delay(300);
  controlUp(false);
}

void closeGate() {
  Serial.println("JANGAN LEWAT");

  controlDown(true);
  delay(300);
  controlDown(false);
}

void pauseGate() {
  Serial.println("SETENGAH");

  controlPause(true);
  delay(300);
  controlPause(false);
}
