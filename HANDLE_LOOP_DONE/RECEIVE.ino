// callback function that will be executed when data is received
void onDataReceive(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&responseBikeData, incomingData, sizeof(responseBikeData));

  String prefix = responseBikeData.prefix;
  String uidCard = responseBikeData.uidCard;
  int resCode = responseBikeData.resCode;
  bool state = responseBikeData.state;

  if (state) {
    wsb = millis();
    openGate();
    // 201 = car_enter, 202 = car_exit, 203 = bike_exit
    if (resCode == 202 || resCode == 203) {
      enableButton3 = true;
      enableButton2 = false;
      if (resCode == 203) {
        delay(400);
        pauseGate();
      }
    } else {
      enableButton3 = false;
      enableButton2 = true;
    }
  } else {
    Serial.println("DITOLAK MASUK");
    Serial.println("resCode= " + String(resCode));

    if (uidCard.length() == 0 || uidCard == "CLOSE") {
      Serial.println("Remote gate down");
      enableButton3 = true;
      enableButton2 = false;
      closeGate();
    }

  }

}
