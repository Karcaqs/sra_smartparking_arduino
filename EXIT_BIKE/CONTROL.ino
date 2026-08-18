void controlUp(bool val) {
  digitalWrite(UP_PIN, val);
}
void controlDown(bool val) {
  digitalWrite(DOWN_PIN, val);
}

void controlStop(bool val) {
  digitalWrite(STOP_PIN, val);
}

void controlRemote(bool val) {

}

void controlFan(bool val) {
  digitalWrite(FAN_PIN, val);
}

void openGate() {
  isEnableEntry = false;
  myDFPlayer.play(4);
  Serial.println("SILAHKAN KELUAR");
  controlUp(true);
  delay(300);
  controlUp(false);
}

void closeGate() {
  controlDown(true);
  delay(300);
  controlDown(false);
  delay(1000);
  isEnableEntry = true;
  playOnce = true;
  cek = 0;

}

void stopGate() {
  controlStop(true);
  delay(100);
  controlStop(false);
  delay(100);
}
