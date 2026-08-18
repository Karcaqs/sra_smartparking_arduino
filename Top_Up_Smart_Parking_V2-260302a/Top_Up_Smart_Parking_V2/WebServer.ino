void initWebServer() {
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "OK");
    // schedule restart asynchronously
    static Ticker restartTicker;
    restartTicker.once_ms(1000, []() {
      ESP.restart();
    });
  });
  server.on("/saveWiFi", HTTP_POST, [](AsyncWebServerRequest * request) {
    String ssid, pass;

    if (request->hasParam("ssid", true)) {
      ssid = request->getParam("ssid", true)->value();
      ssid.trim();
    }
    if (request->hasParam("pass", true)) {
      pass = request->getParam("pass", true)->value();
      pass.trim();
    }

    if (ssid.length() == 0) {
      request->send(400, "text/plain", "SSID wajib diisi");
      return;
    }

    saveWiFiConfig(ssid, pass);

    request->send(200, "text/plain", "Data disimpan. ESP akan restart...");
    static Ticker restartTicker1;
    restartTicker1.once_ms(1000, []() {
      ESP.restart();
    });
  });
  server.on("/saveEnvironment", HTTP_POST, [](AsyncWebServerRequest * request) {
    if (request->hasParam("envName", true)) {
      envName = request->getParam("envName", true)->value();
      envName.trim();
    }

    if (envName.length() == 0) {
      request->send(400, "text/plain", "Environment wajib diisi");
      return;
    }

    saveEnvironment(envName);

    request->send(200, "text/plain", "Environment disimpan. ESP akan restart...");
    static Ticker restartTicker2;
    restartTicker2.once_ms(1000, []() {
      ESP.restart();
    });
  });

  server.begin();
}
