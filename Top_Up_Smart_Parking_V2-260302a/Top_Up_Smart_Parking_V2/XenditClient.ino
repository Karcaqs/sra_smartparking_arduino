// API Xendit config
const char* baseUrlXendit = "https://api.xendit.co/qr_codes";
const char* checkExpired = "/:qr_id";
const char* checkStatus = "/:qr_id/payments";

void httpClientXendit(HTTPClient& _http, const String& _url) {
  _http.begin(_url);
  _http.addHeader("Content-Type", "application/json");
  _http.addHeader("api-version", "2022-07-31");

  if (envName == "PRODUCTION") {
    _http.addHeader("Authorization", "Basic eG5kX3Byb2R1Y3Rpb25fdkZmTm8xWTE3OFY0VHZMZ2o1ZzhlTEk5dG43QnRzQkxHcXh3M1picXoxU01RT3BJSnQyMWZRSEJDemF4OWk6");  // API Key SRA Smart Production
  } else {
    _http.addHeader("Authorization", "Basic eG5kX2RldmVsb3BtZW50XzBFc3NHSEozNHljamlzaDczeEpxTzY5VmRPOHB0OFB0MFU3ajJFYzVwTVZjNE1wRjJWMzNib0o1ZE5lNmdNOg==");  // API Key SRA Smart Sandbox
  }
}

void createQR() {
  timeOrder();
  HTTPClient http;
  httpClientXendit(http, baseUrlXendit);

  JsonDocument data;
  data["reference_id"] = referenceId;
  data["type"] = "DYNAMIC";
  data["currency"] = "IDR";
  data["amount"] = amount;
  data["expires_at"] = expired;

  String requestBody;
  serializeJson(data, requestBody);

  String response = sendHttpRequest(http, requestBody);
  if (response != "error") {
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, response);

    qrId = doc["id"].as<String>();
    qrString = doc["qr_string"].as<String>();

    msgCreatePayment();

    if (qrString != "" && qrString != nullptr) {
      generateQR(qrString.c_str());
      tDown.setCounter(0, 0, 59, tDown.COUNT_DOWN, tDownComplete);
      tDown.setInterval(lcdCountimer, 1000);
      tDown.start();
      currentState = STATE_CHECK_QR;
    }
  }

  http.end();
}

void checkQR() {
  String url = String(baseUrlXendit) + String(checkExpired);
  url.replace(":qr_id", qrId);
  HTTPClient http;
  httpClientXendit(http, url);

  String response = sendHttpRequest(http);
  if (response != "error") {
    JsonDocument doc;
    deserializeJson(doc, response);

    String status = doc["status"].as<String>();
    if (status == "INACTIVE") {
      http.end();
      currentState = STATE_CHECK_PAYMENT;
    }
  }

  http.end();
}

void checkPayment() {
  String url = String(baseUrlXendit) + String(checkStatus);
  url.replace(":qr_id", qrId);
  HTTPClient http;
  httpClientXendit(http, url);

  String response = sendHttpRequest(http);
  if (response != "error") {
    JsonDocument doc;
    deserializeJson(doc, response);

    String status = doc["data"][0]["status"].as<String>();

    if (status == "SUCCEEDED") {
      statusPayment = "SUCCEEDED";
      lcdSuccess();
      msgSuccess();

      buzzerOn();
      amount = 0;
      topup = false;
      currentState = STATE_CHECK_SALDO;
    } else {
      lcdExpired();
      msgExpired();

      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);
      topup = false;
      statusPayment = "EXPIRED";
      amount = 0;
      uidCard = "";
      currentState = STATE_FIRST;
    }
  }

  http.end();
}
