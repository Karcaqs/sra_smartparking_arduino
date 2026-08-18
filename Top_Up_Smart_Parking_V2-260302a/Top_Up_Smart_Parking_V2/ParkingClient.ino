const char* baseUrl = "http://192.168.8.100:3000/api/";
const char* checkCard = "checkCard";

void httpClientParking(HTTPClient& _http, const String& _url) {
  _http.begin(_url);
  _http.addHeader("Content-Type", "application/json");
  _http.addHeader("Accept", "application/json");
}

String sendHttpRequest(HTTPClient& _http, const String& _requestBody = "") {
  int httpResponseCode;
  if (_requestBody != "") {
    httpResponseCode = _http.POST(_requestBody);
  } else {
    httpResponseCode = _http.GET();
  }

  if (httpResponseCode > 0) {
    String response = _http.getString();
    return response;
  } else {
    Serial.print(F("Error code: "));
    Serial.println(httpResponseCode);
    return "error";
  }
}

void checkSaldo() {
  String url = String(baseUrl) + String(checkCard);
  bool success = false;
  int retryCount = 0;

  while (retryCount < MAX_RETRIES && !success) {
    logActivity("Checking balance (Attempt " + String(retryCount + 1) + ")...");
    
    HTTPClient http;
    httpClientParking(http, url);

    JsonDocument data;
    data["id_card"] = uidCard;

    String requestBody;
    serializeJson(data, requestBody);
    String response = sendHttpRequest(http, requestBody);

    if (response != "error") {
      JsonDocument doc;
      deserializeJson(doc, response);

      if (doc.containsKey("balance")) {
        success = true;
        logActivity("Balance check successful.");
        
        if (topup) {
          delay(100);
          currentState = STATE_TOPUP;
        } else {
          int balance = doc["balance"].as<int>();
          int sync = doc["sync"].as<int>();

          if (sync == 3) {
            drawJpeg("/alert.bmp", 96, 88);
            lcdShowSaldo(balance);
            msgShowSaldo(balance);

            digitalWrite(BUZZER, HIGH);
            delay(1000);
            digitalWrite(BUZZER, LOW);
            uidCard = "";
            currentState = STATE_FIRST;
          }
        }
      } else if (doc.containsKey("error")) {
        success = true; // Request succeeded but card not found
        logActivity("Card not registered: " + uidCard, "WARN");
        lcdNotRegistered();
        msgNotRegistered();

        delay(1000);
        uidCard = "";
        topup = false;
        currentState = STATE_FIRST;
      }
    } else {
      logActivity("Balance check failed (Attempt " + String(retryCount + 1) + ")", "ERROR");
    }
    
    http.end();
    
    if (!success) {
      retryCount++;
      if (retryCount < MAX_RETRIES) {
        logActivity("Retrying in " + String(RETRY_INTERVAL_MS / 1000) + "s...");
        delay(RETRY_INTERVAL_MS);
      }
    }
  }

  if (!success) {
    logActivity("Balance check failed after " + String(MAX_RETRIES) + " attempts.", "CRITICAL");
    // Handle persistent failure - reset state
    uidCard = "";
    topup = false;
    currentState = STATE_FIRST;
  }
}
