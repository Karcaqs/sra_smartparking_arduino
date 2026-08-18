void sendingBikeEntryResponse(String jsonString) {
  //  Serial.println(jsonString);
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print("deserializeJson() bike entry failed: ");
    Serial.println(error.c_str());
    return;
  }
  String prefix = doc["prefix"];
  String uidCard = doc["uidCard"];
  bool state = doc["state"];
  int resCode = doc["resCode"];

  strcpy(responseBikeData.uidCard, uidCard.c_str());
  responseBikeData.state = state;
  responseBikeData.resCode = resCode;

  //  Serial.println("PREFIX: " + prefix);
  //  Serial.println("UID CARD: " + uidCard);
  //  Serial.println("STATE: " + String(state));
  //  Serial.println("RESCODE: " + String(resCode));

  esp_err_t result = esp_now_send(portalMasukAddress, (uint8_t *) &responseBikeData, sizeof(responseBikeData));
  
  responseBikeData.resCode = 201;
  esp_err_t control = esp_now_send(controlGateAddress, (uint8_t *) &responseBikeData, sizeof(responseBikeData)); // buka palang
}

void sendingBikeExitResponse(String jsonString) {
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (error) {
    Serial.print("deserializeJson() bike exit failed: ");
    Serial.println(error.c_str());
    return;
  }
  String prefix = doc["prefix"];
  String uidCard = doc["uidCard"];
  bool state = doc["state"];
  int resCode = doc["resCode"];

  strcpy(responseBikeData.uidCard, uidCard.c_str());
  responseBikeData.state = state;
  responseBikeData.resCode = resCode;

  //  Serial.println("PREFIX: " + prefix);
  //  Serial.println("UID CARD: " + uidCard);
  //  Serial.println("STATE: " + String(state));
  //  Serial.println("RESCODE: " + String(resCode));

  esp_err_t result = esp_now_send(portalKeluarAddress, (uint8_t *) &responseBikeData, sizeof(responseBikeData));
  
  responseBikeData.resCode = 202;
  esp_err_t control = esp_now_send(controlGateAddress, (uint8_t *) &responseBikeData, sizeof(responseBikeData)); // buka palang
}

void sendingControlGate() {
  strcpy(responseBikeData.uidCard, "CLOSE");
  responseBikeData.state = false;
  responseBikeData.resCode = 100;

  esp_err_t enter = esp_now_send(portalMasukAddress, (uint8_t *) &responseBikeData, sizeof(responseBikeData));
  esp_err_t exit = esp_now_send(portalKeluarAddress, (uint8_t *) &responseBikeData, sizeof(responseBikeData));
}
