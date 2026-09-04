void sendingBikeData() {
  if (cardId != "") {
    //    Serial.println("MASUK PAK EKO");
    bikeEntryExitData.prefix = prefixCard;
    strcpy(bikeEntryExitData.uidCard, cardId.c_str());
    bikeEntryExitData.temp = readTemp();
    esp_err_t result = esp_now_send(masterAddr, (uint8_t *) &bikeEntryExitData, sizeof(bikeEntryExitData));

  }
}

void sendingRemotData(float code) {
  //  Serial.println("REMOTE");
  remoteBikeData.prefix = prefixRemote;
  remoteBikeData.temp = code;
  Serial.println(  remoteBikeData.prefix);
  Serial.println(  remoteBikeData.temp);
  esp_err_t result = esp_now_send(masterAddr, (uint8_t *) &remoteBikeData, sizeof(remoteBikeData));
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == ESP_NOW_SEND_SUCCESS) {
    cardId = "";
  }
  else {
    cardId = "";
    isEnableEntry = false;
    myDFPlayer.play(8); // coba lagi
    delay(2000);
    isEnableEntry = true;
  }
}
