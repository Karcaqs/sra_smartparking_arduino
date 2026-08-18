void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setSleep(false);
  WiFi.onEvent(WiFiEvent);

  mac = getMacID();
  idDevice += "-" + mac;
}

void initAPWiFi() {
  IPAddress ap_ip(192, 168, 4, 1);
  IPAddress ap_mask(255, 255, 255, 0);
  IPAddress ap_leaseStart(192, 168, 4, 2);
  IPAddress ap_dns(8, 8, 4, 4);

  const char* SSID_AP = idDevice.c_str();

  WiFi.AP.begin();
  WiFi.AP.config(ap_ip, ap_ip, ap_mask, ap_leaseStart, ap_dns);
  WiFi.AP.create(SSID_AP, PASSWORD_AP);
  if (!WiFi.AP.waitStatusBits(ESP_NETIF_STARTED_BIT, 1000)) {
    Serial.println(F("Failed to start AP!"));
    return;
  }
}

void connectToWiFi() {
  buzzerOn();
  lcdConnectingToWiFi();
  
  Serial.println(SSID_STA);
  WiFi.begin(SSID_STA, PASSWORD_STA);
  
  int count = 0;
  while (WiFi.status() != WL_CONNECTED || count >= 50) {
    count++;
    delay(100);
  }

  lcdConnectedToWiFi();
}

String getMacID() {
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);

  char id[7];  // 6 karakter + null terminator
  sprintf(id, "%02X%02X%02X", mac[3], mac[4], mac[5]);
  return String(id);
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {

    case ARDUINO_EVENT_WIFI_READY:
      Serial.println(F("WiFi ready"));
      break;

    case ARDUINO_EVENT_WIFI_STA_START:
      Serial.println(F("WiFi STA starting"));
      break;

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.println(F("WiFi STA connected"));
      break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println(F("WiFi connected"));
      Serial.print(F("IP address: "));
      Serial.println(WiFi.localIP());
      break;

    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      Serial.println(F("WiFi lost IP"));
      WiFi.reconnect();
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println(F("WiFi Disconnected"));
      break;

    default:
      break;
  }
}
