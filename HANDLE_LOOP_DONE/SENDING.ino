void sendingDisReset() {

  commandMaster.resCode = 204;

  esp_err_t resultCar = esp_now_send(masterCar, (uint8_t *) &commandMaster, sizeof(commandMaster));
  esp_err_t resultBike = esp_now_send(masterBike, (uint8_t *) &commandMaster, sizeof(commandMaster));
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
