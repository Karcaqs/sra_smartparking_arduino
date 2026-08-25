// callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&responseBikeData, incomingData, sizeof(responseBikeData));

  String prefix = responseBikeData.prefix;
  String uidCard = responseBikeData.uidCard;
  int resCode = responseBikeData.resCode;
  bool state = responseBikeData.state;

  if (state) {
    openGate();
    cardId = "";
  } else {
//    Serial.println("DITOLAK MASUK, state: " + String(state));
//    Serial.println("resCode= " + String(resCode));

    if (uidCard.length() == 0 || uidCard == "CLOSE") {
//      Serial.println(F("Remote gate down"));
      closeGate();
    }

    switch (resCode) {
      case 101:
        // KARTU SUDAH digunakan
        myDFPlayer.play(5);
        break;
      case 102:
        // SALDO TIDAK CUKUP
        // myDFPlayer.play(7);
        break;
      case 103:
        // KARTU BELUM TERDAFTAR
        myDFPlayer.play(6);
        break;
      case 104:
        break;
      case 105:
        break;
      case 106:
        esp_restart();
        break;
      case 200:
        // "x." dispenser code
        // ".x" error code
        uint32_t versiondata = nfc.getFirmwareVersion();
        if (!versiondata) {
          float code = 4.001;
          sendingRemotData(code);
        } else {
          float code = 4.002;
          sendingRemotData(code);
        }
        break;
    }
    delay(1000);
    cardId = "";
    isEnableEntry = true;
  }

}
