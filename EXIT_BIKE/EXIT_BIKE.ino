#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_PN532.h>
#include <esp_task_wdt.h>
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// set mac
#include "esp_wifi.h"
uint8_t newMACAddress[6] = {0xD8, 0xBC, 0x38, 0xFA, 0x82, 0xA8};  //D8:BC:38:FA:82:A8

esp_now_peer_info_t peerInfo;

// Pin untuk komunikasi I2C (IRQ dan RESET)
// #define PN532_IRQ   (16)    // Bisa diganti sesuai kebutuhan (SD)
// #define PN532_RESET (17)    // Bisa diganti sesuai kebutuhan (SC)
#define PN532_SDA   (21)
#define PN532_SLC   (22)

// Inisialisasi PN532 dengan I2C
// Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
//Adafruit_PN532 nfc(PN532_SDA, PN532_SLC);

#define PN532_IRQ (2)
#define PN532_RESET (3)
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

SoftwareSerial softSerial(/*rx =*/19, /*tx =*/18);
#define FPSerial softSerial
DFRobotDFPlayerMini myDFPlayer;

// 24:DC:C3:48:69:60 MASTER BIKE ADDRESS
// D8:BC:38:FA:82:A8

uint8_t masterAddr[] = {0x24, 0xDC, 0xC3, 0x48, 0x69, 0x60};

String prefixRemote = "[REMOTE_EXIT]";
String prefixCard = "[BIKE_EXIT]";

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

#define UP_STATE_PIN 13
#define DOWN_STATE_PIN 15
#define UP_PIN 33
#define DOWN_PIN 25
#define STOP_PIN 32
#define LOOP_ENTRY_PIN 14
#define LOOP_EXIT_PIN 27
#define FAN_PIN 5
#define DS_PIN 4

#define EXTRA_PIN 26

OneWire oneWire(DS_PIN);
DallasTemperature sensors(&oneWire);

bool isEnableEntry = true;
bool isLoopEntryOn = false;
bool isLoopExitOn = false;
bool readyToEntry = false;
bool playOnce = true;

String cardId = "";
unsigned long lastTimeReadTag = 0;
unsigned long checkingStateTime = 0;
int cek = 0;

void setup() {
  Serial.begin(115200);

  initPin();
  initDfPlayer();
  sensors.begin();

  initPN532();

  WiFi.mode(WIFI_STA);  // penting! aktifkan WiFi station
  delay(300);           // beri waktu driver nyala

  Serial.println(WiFi.macAddress());

  // set mac
  if (esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]) == ESP_OK) {
    //    Serial.println("MAC address changed successfully!");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.println(F("Failed to change MAC address"));
  }

  WiFi.disconnect();
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //  esp_now_register_send_cb(OnDataSent);
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  memcpy(peerInfo.peer_addr, masterAddr, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println(F("Failed to add peer"));
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  controlFan(true);

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

  unsigned long currentTime = millis();
  // Beri makan WDT

  esp_task_wdt_reset();
  // CHECKING GATE STATUS
  if (currentTime - checkingStateTime >= 100) {
    checkLoopDetector();
    // checkRemoteState();
    checkingStateTime = currentTime;
  }

  // NORMAL ENTRY
  if (isLoopEntryOn && isEnableEntry) {
    if (playOnce) {
      // myDFPlayer.play(3);
      uint32_t versiondata = nfc.getFirmwareVersion();
      if (versiondata) {
        myDFPlayer.play(3);
      }
      playOnce = false;
    }
    if (currentTime - lastTimeReadTag >= 1000) {
      readNFC();
      lastTimeReadTag = currentTime;
    }
  }

  if (!isEnableEntry) {
    cek++;
    if (cek > 50) {
      isEnableEntry = true;
      cek = 0;
    }
  }

  // //  CEK LOOP EXIT
  // if (!isLoopExitOn) {
  //   if (digitalRead(LOOP_EXIT_PIN) == LOW) {
  //     isLoopExitOn = true;
  //   }
  // } else {
  //   if (digitalRead(LOOP_EXIT_PIN) == HIGH) {
  //     stopGate();
  //     closeGate();
  //     isLoopExitOn = false;
  //   }
  // }

  delay(100);
}
