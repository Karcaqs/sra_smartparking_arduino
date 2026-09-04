// void readNFC() {
//   if (nfc.tagPresent()) {
//     entryByCard = true;
//     NfcTag tag = nfc.read();

//     tag.print();
//     cardId = tag.getUidString();
//     cardId.replace(" ", "");
//     Serial.println("Tag id");
//     Serial.println(cardId);
//     sendingBikeData();

//     delay(1000);
//   } else {
//     Serial.println("TEMPELKAN KARTU!!!");
//   }
// }

void readNFC() {
  String uidString = "";
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  //  Serial.println("Menunggu kartu ISO14443A ...");

  if (nfc.inListPassiveTarget()) {

    // Baca kartu ISO14443A
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 500); //timeout 100ms
    //    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (success) {
      //      Serial.println("Ditemukan kartu ISO14443A");
      //      Serial.print("  Panjang UID: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
      //      Serial.print("  Nilai UID: ");
      nfc.PrintHex(uid, uidLength);

      if (uidLength == 4) {
        // Kemungkinan besar Mifare Classic
        uint32_t cardid = uid[0];
        cardid <<= 8;
        cardid |= uid[1];
        cardid <<= 8;
        cardid |= uid[2];
        cardid <<= 8;
        cardid |= uid[3];
        //        Serial.print("Terdeteksi Mifare Classic, ID: ");
        //        Serial.println(cardid);
      } else {
        Serial.println(F("Kartu bukan Mifare Classic (UID > 4 byte)"));
      }

      for (uint8_t i = 0; i < uidLength; i++) {
        if (uid[i] < 0x10) uidString += "0";  // tambahkan 0 di depan jika < 0x10 (biar jadi 2 digit)
        uidString += String(uid[i], HEX);
      }
      uidString.toUpperCase();  // biar hurufnya kapital semua (opsional)

      //      Serial.print("UID sebagai string: ");
      //      Serial.println(uidString);

      cardId = uidString;
      sendingBikeData();
      delay(1000);
    }
  }
  //  else {
  //    Serial.println("TEMPELKAN KARTU!!!");
  //  }
}
