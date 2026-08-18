AsyncWebServer server(80);

void initWebServer() {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"rawliteral(
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <body style="text-align:center;font-family:Arial;">
      <h2>Smart Switch</h2>

      <button onclick="sendCommand('/on')">ON</button>
      <button onclick="sendCommand('/off')">OFF</button>
      <button onclick="sendCommand('/ping')">PING</button>

      <br><br>
      <div id="status"></div>

      <script>
        function sendCommand(url){
          fetch(url)
            .then(res => res.text())
            .then(data => {
              alert(data);
            });
        }

        setInterval(()=>{
          fetch('/status')
            .then(res=>res.text())
            .then(data=>document.getElementById('status').innerHTML=data);
        },1000);
      </script>
    </body>
    </html>
    )rawliteral";

    request->send(200, "text/html", html);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleON();
    request->send(200, "text/plain", "Relay ON");
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleOFF();
    request->send(200, "text/plain", "Relay OFF (Auto ON 15s)");
  });

  server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request) {
    handlePing();
    request->send(200, "text/plain", "Device Online");
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    String status = relayState ? "Relay: ON" : "Relay: OFF";
    request->send(200, "text/plain", status);
  });

  server.begin();
}