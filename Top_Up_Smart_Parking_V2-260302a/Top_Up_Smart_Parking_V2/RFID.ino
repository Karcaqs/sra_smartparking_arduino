void initRFID() {
  nfc.begin();
  delay(500);

  uint32_t versiondata = nfc.getFirmwareVersion();
  while (!versiondata) {
    Serial.println("Didn't find PN53x board");
    delay(1000);
  }
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.SAMConfig();
}

String readUidCard() {
  String uidCard = "";
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 500)) {
    for (byte i = 0; i <= uidLength - 1; i++) {
      uidCard += (uid[i] < 0x10 ? "0" : "") + String(uid[i], HEX);
    }
    uidCard.toUpperCase();
    buzzerOn();
    //    Serial.println();
    //    Serial.print("UID CARD : ");
    //    Serial.println(uidCard);
    //    Serial.println();
  }
  return uidCard;
}
