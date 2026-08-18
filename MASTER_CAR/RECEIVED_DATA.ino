// callback function that will be executed when data is received
void onDataReceive(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  
  if (len == sizeof(struct_bike_entry_exit)) {
    memcpy(&bikeEntryExitData, incomingData, sizeof(bikeEntryExitData));
    StaticJsonDocument<512> json;

    String prefix = bikeEntryExitData.prefix;
    if (prefix == prefixEntry) {
      String uidCard = bikeEntryExitData.uidCard;
      float temp = bikeEntryExitData.temp;
      json["uidCard"] = uidCard;
      json["temp"] = temp;
      String jsonString;
      serializeJson(json, jsonString);
      String messageToSend = prefix + jsonString;
      Serial.println(messageToSend);

    } else if (prefix == prefixExit) {
      String uidCard = bikeEntryExitData.uidCard;
      float temp = bikeEntryExitData.temp;
      json["uidCard"] = uidCard;
      json["temp"] = temp;
      String jsonString;
      serializeJson(json, jsonString);
      String messageToSend = prefix + jsonString;
      Serial.println(messageToSend);

    }

  } else if (len == sizeof(struct_remote_bike)) {
    
    memcpy(&remoteBikeData, incomingData, sizeof(remoteBikeData));
    StaticJsonDocument<512> json;
    String prefix = remoteBikeData.prefix;

    if (prefix == prefixRemotEntry) {
      float temp = remoteBikeData.temp;
      json["temp"] = temp;
      String jsonString;
      serializeJson(json, jsonString);
      String messageToSend = prefix + jsonString;
      Serial.println(messageToSend);

    } else if (prefix == prefixRemotExit) {
      float temp = remoteBikeData.temp;
      json["temp"] = temp;
      String jsonString;
      serializeJson(json, jsonString);
      String messageToSend = prefix + jsonString;
      Serial.println(messageToSend);
    }

  } else if (len == sizeof(struct_control_gate)) {
    memcpy(&controlGateData, incomingData, sizeof(controlGateData));
    sendingControlGate();
  }

}
