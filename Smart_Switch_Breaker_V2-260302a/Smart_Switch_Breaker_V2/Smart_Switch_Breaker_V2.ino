#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <AsyncTelegram2.h>
#include <ESPAsyncWebServer.h>

extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMQTT_ESP32.h>

// === KONFIGURASI PIN ===
#define RELAY_PIN 8

// === WIFI AUTHENTICATION ===
#define WIFI_SSID "Tselhome-4D33"
#define WIFI_PASSWORD "70761510"

// === MQTT CONFIG ===
#define MQTT_HOST "broker.emqx.io"  // Broker address
#define MQTT_PORT 1883
const char* mqtt_topic_cmd = "esp32c3/switch/cmd";
const char* mqtt_topic_status = "esp32c3/switch/status";

// === TELEGRAM CONFIG ===
const char* BOT_TOKEN = "7834428789:AAEbUPTAlrpHW9j5vTVvcK1eKpWe-5nERoU";  // Ganti dengan token bot Anda
int64_t CHAT_ID = -4863963323;
const char* ON = "✅ ON";
const char* OFF = "⛔️ OFF";
const char* PING = "📶 PING";

// === MQTT CLIENT ===
AsyncMqttClient mqttClient;

// === TELEGRAM CLIENT ===
WiFiClientSecure client;
AsyncTelegram2 myBot(client);
ReplyKeyboard myReplyKbd;

// === STATUS RELAY ===
bool relayState = false;
unsigned long relayOffTimestamp = 0;
bool isTimerRunning = true;

TimerHandle_t mqttReconnectTimer;

void printSeparationLine() {
  Serial.println("************************************************");
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  delay(500);

  WiFi.onEvent(WiFiEvent);

  connectToWifi();

  initWebServer();
}

void loop() {

  if (isTimerRunning && (millis() - relayOffTimestamp >= 15000)) {
    digitalWrite(RELAY_PIN, LOW);
    relayState = true;
    isTimerRunning = false;
    Serial.println("Relay otomatis menyala kembali setelah 15 detik ✅");
    mqttClient.publish(mqtt_topic_status, 0, false, "Relay otomatis menyala kembali setelah 15 detik ✅");
    myBot.sendTo(CHAT_ID, "Relay otomatis menyala kembali setelah 15 detik ✅");
  }

  handleNewMessage();
}
