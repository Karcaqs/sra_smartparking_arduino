void initPin() {
  pinMode(UP_STATE_PIN, INPUT);
  pinMode(UP_PIN, OUTPUT);
  pinMode(DOWN_PIN, OUTPUT);
  pinMode(LOOP_ENTRY_PIN, INPUT);
  pinMode(LOOP_EXIT_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);


  pinMode(EXTRA_PIN, OUTPUT);
  digitalWrite(EXTRA_PIN, LOW);
  //  Serial.println("init pin ok");
}

void initDfPlayer() {
  FPSerial.begin(9600);
  if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    //    while (5) {
    //      delay(0); // Code to compatible with ESP8266 watch dog.
    //    }
  } else {
    Serial.println(F("DFPlayer Mini online."));
    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  }
}

void initPN532() {
  digitalWrite(EXTRA_PIN, HIGH);
  delay(1000);

  //  Wire.begin(PN532_SDA, PN532_SLC);
  nfc.begin();
  delay(500);

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println(F("Tidak menemukan chip PN53x"));
    myDFPlayer.play(8);
    // while (1); // stop di sini
  } else {
    // Tampilkan informasi versi firmware
    Serial.println(F("Ditemukan chip PN532"));
    //    Serial.println((versiondata >> 24) & 0xFF, HEX);
    //    Serial.print("Versi firmware: "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    //    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

    myDFPlayer.play(1);
  }

  delay(1000);

  // Atur PN532 untuk membaca kartu ISO14443A (Mifare, dll)
  nfc.SAMConfig();
}
