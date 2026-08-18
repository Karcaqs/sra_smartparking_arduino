#pragma once

//#define PRODUCTION true
String envName;

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <HTTPClient.h>
#include <AsyncTelegram2.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>
#include <qrcodenews.h>
#include "time.h"
#include "esp_sntp.h"
#include <ESP32Time.h>
#include <Wire.h>
#include "Countimer.h"
#include "Font.h"
#include <Adafruit_PN532.h>
#include <Ticker.h>
#include <ESPAsyncWebServer.h>

#define PN532_IRQ (2)
#define PN532_RESET (3)

#define TFT_BG 0xC75F

const char* BOT_TOKEN = "7771959604:AAFxgg4Uao9jVFoAECoPq0W_xBNm-ddfnNU";
int64_t CHAT_ID = 6385088034;

Preferences prefs;

AsyncWebServer server(80);

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

TFT_eSPI tft = TFT_eSPI();

WiFiClientSecure client;
AsyncTelegram2 myBot(client);

ESP32Time rtc;
ESP32Time rtc1(60);
ESP32Time rtc2(28800);

Countimer tDown;

enum AppState {
  STATE_FIRST,
  STATE_CHECK_SALDO,
  STATE_TAP_CARD,
  STATE_TOPUP,
  STATE_CREATE_QR,
  STATE_CHECK_QR,
  STATE_CHECK_PAYMENT,
  STATE_UPDATE_SALDO,
  STATE_IDLE
};

const char* appStateToString(AppState state) {
  switch (state) {
    case STATE_FIRST:          return "STATE_FIRST";
    case STATE_CHECK_SALDO:    return "STATE_CHECK_SALDO";
    case STATE_TAP_CARD:       return "STATE_TAP_CARD";
    case STATE_TOPUP:          return "STATE_TOPUP";
    case STATE_CREATE_QR:      return "STATE_CREATE_QR";
    case STATE_CHECK_QR:       return "STATE_CHECK_QR";
    case STATE_CHECK_PAYMENT:  return "STATE_CHECK_PAYMENT";
    case STATE_UPDATE_SALDO:   return "STATE_UPDATE_SALDO";
    case STATE_IDLE:           return "STATE_IDLE";
    default:                   return "UNKNOWN_STATE";
  }
}

AppState currentState = STATE_FIRST;
bool stateEntered = false;

unsigned long stateStartTime = 0;
const unsigned long STATE_TIMEOUT = 15000;

String uidCard = "";
String mac;
String idDevice = "topup-saldo";

bool topup = false;

const int btn1 = 34;
const int btn2 = 32;
const int btn3 = 25;
const int BUZZER = 17;

long amount = 0;
String referenceId;
String qrId = "";
String qrString = "";
String status = "";
String statusPayment = "";

long timestamp = 0;
long timesExpired = 0;
String expired = "";

const char* PASSWORD_AP = "SuksesSelalu";
String SSID_STA;
String PASSWORD_STA;

unsigned long lastCheckQR = 0;

bool telegramInitialized = false;

bool btn1State = LOW;
bool btn1PrevState = LOW;
bool btn2State = LOW;
bool btn2PrevState = LOW;
bool btn3State = LOW;
bool btn3PrevState = LOW;

// Retry Settings
const int MAX_RETRIES = 3;
const int RETRY_INTERVAL_MS = 3000;

// Monitoring Settings
unsigned long lastHealthReport = 0;
const unsigned long HEALTH_REPORT_INTERVAL = 300000; // 5 minutes
