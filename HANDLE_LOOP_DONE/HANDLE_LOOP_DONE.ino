#include <esp_now.h>
#include <WiFi.h>
#include <esp_task_wdt.h>

// set mac
#include "esp_wifi.h"  
uint8_t newMACAddress[6] = {0x10, 0x06, 0x1C, 0x65, 0xA2, 0x18};  // Must be unicast + locally administered

// Pin relay
const int relay1 = 19;
const int relay2 = 18;
const int relay3 = 5;
const int relay4 = 4;

// Pin push button
const int button1 = 32;
const int button2 = 33;
const int button3 = 25;
const int button4 = 26;

// Variable to store button states
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

bool enableButton3 = true;
bool enableButton4 = true;
bool enableButton1 = true;
bool enableButton2 = true;
bool enableDown1 = false;
bool enableDown2 = false;

unsigned long wsb = 0;

// ESP NOW
esp_now_peer_info_t peerInfo;

uint8_t masterCar[] = { 0x24, 0xDC, 0xC3, 0x48, 0x6A, 0x24 };
uint8_t masterBike[] = { 0x24, 0xDC, 0xC3, 0x48, 0x69, 0x60 };

typedef struct struct_response_bike {
  String prefix;
  char uidCard[20];
  bool state;
  int resCode;
} struct_response_bike;
struct_response_bike responseBikeData;

typedef struct struct_control_gate {
  int resCode;
} struct_control_gate;
struct_control_gate commandMaster;

void setup() {
  // Inisialisasi pin relay sebagai output
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  // Inisialisasi pin push button sebagai input
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);

  // Mematikan semua relay di awal (relay active-high)
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

  // ESP NOW
  WiFi.mode(WIFI_STA);  // penting! aktifkan WiFi station
  delay(300);           // beri waktu driver nyala

  Serial.println(WiFi.macAddress());

  // set mac
  if (esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]) == ESP_OK) {
    Serial.println("MAC address changed successfully!");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.println("Failed to change MAC address");
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Register master portal mobil
  memcpy(peerInfo.peer_addr, masterCar, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Register master porat motor
  memcpy(peerInfo.peer_addr, masterBike, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(onDataReceive);

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
  // Feed watchdog
  esp_task_wdt_reset();

  // Baca status tombol
  buttonState2 = digitalRead(button2);  //loop mobil masuk
  buttonState3 = digitalRead(button3);  //loop keluar

  // Reset controle loop
  if (millis() - wsb >= 15000) {
    enableButton3 = true;
    enableButton2 = true;
  }

  // Tutup Palang Masuk
  if (enableButton2) {
    if (buttonState2 == HIGH) {
      Serial.println("2 MASIH HIGH");
      enableDown1 = true;
    } else {
      if (enableDown1) {
        Serial.println("2 SUDAH LOW");
        closeGate();
        enableDown1 = false;
        sendingDisReset();
      }
    }
  }

  // Tutup Palang Keluar
  if (enableButton3) {
    if (buttonState3 == HIGH) {
      Serial.println("3 MASIH HIGH");
      enableDown2 = true;
    } else {
      if (enableDown2) {
        Serial.println("3 SUDAH LOW");
        closeGate();
        enableDown2 = false;
        sendingDisReset();
      }
    }
  }

  delay(100);
}
