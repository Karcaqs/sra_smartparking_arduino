#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <esp_task_wdt.h>

esp_now_peer_info_t peerInfo;

// D8:BC:38:FB:D1:90 MASUK
// 24:DC:C3:48:5D:6C KELUAR
// 10:06:1C:65:A2:18 CONTROL GATE

uint8_t portalMasukAddress[] = {0x3C, 0x8A, 0x1F, 0x63, 0x7B, 0xFC};
uint8_t portalKeluarAddress[] = {0x24, 0xDC, 0xC3, 0x48, 0x5D, 0x6C};
uint8_t controlGateAddress[] = {0x10, 0x06, 0x1C, 0x65, 0xA2, 0x18};

// PAYLOAD MOTOR MASUK DAN KELUAR
typedef struct struct_bike_entry_exit {
  String prefix;
  char uidCard[20];
  float temp;
} struct_bike_entry;
struct_bike_entry_exit bikeEntryExitData;

// PAYLOAD RESPONSE COM KE PORTAL MASUK/KELUAR
typedef struct struct_response_bike {
  String prefix;
  char uidCard[20];
  bool state;
  int resCode;
} struct_response_bike;
struct_response_bike responseBikeData;

// PAYLOAD STATUS REMOT, JIKA PORTAL DI AKTIFKAN DENGAN REMOT
typedef struct struct_remote_bike {
  String prefix;
  float temp;
} struct_remote_bike;
struct_remote_bike remoteBikeData;

//PAYLOAD CONTROL GATE
typedef struct struct_control_gate {
  int resCode;
} struct_control_gate;
struct_control_gate controlGateData;


String prefixRemotEntry = "[REMOTE_ENTRY]";
String prefixRemotExit = "[REMOTE_EXIT]";
String prefixEntry = "[ENTRY]";
String prefixExit = "[CAR_EXIT]";
String prefixControl = "[RESET]";

void setup() {
  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Register peer portal masuk
  memcpy(peerInfo.peer_addr, portalMasukAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Register peer portal keluar
  memcpy(peerInfo.peer_addr, portalKeluarAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Register peer control gate
  memcpy(peerInfo.peer_addr, controlGateAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(onDataReceive);
  //  esp_now_register_recv_cb(receiveRemoteData);

  // Inisialisasi Task Watchdog Timer dengan timeout 5 detik
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 5000,  // 5 detik timeout
    .idle_core_mask = (1 << portNUM_PROCESSORS) - 1, // Aktifkan pada semua core
    .trigger_panic = true // Trigger panic saat timeout
  };
  esp_task_wdt_init(&wdt_config);
  esp_task_wdt_add(xTaskGetCurrentTaskHandle());
}

void loop() {

  // Beri makan WDT
  esp_task_wdt_reset();

  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.println("Data dari PC: " + receivedData);
    const String responseBikeEntryPrefix = "[RESPONSE_ENTRY]";
    const String responseBikeExitPrefix = "[RESPONSE_EXIT]";

    if (receivedData.startsWith(responseBikeEntryPrefix)) {
      sendingBikeEntryResponse(receivedData.substring(responseBikeEntryPrefix.length()));
    } else if (receivedData.startsWith(responseBikeExitPrefix)) {
      sendingBikeExitResponse(receivedData.substring(responseBikeExitPrefix.length()));
    }
  }

}
