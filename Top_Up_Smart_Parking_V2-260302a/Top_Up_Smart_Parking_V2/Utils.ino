void initPin() {
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

String formatRupiah(long angka) {
  String hasil = String(angka);
  int len = hasil.length();

  for (int i = len - 3; i > 0; i -= 3) {
    hasil = hasil.substring(0, i) + "." + hasil.substring(i);
  }
  return hasil;
}

void tDownComplete() {
  tDown.stop();
}

void buzzerOn() {
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
}
