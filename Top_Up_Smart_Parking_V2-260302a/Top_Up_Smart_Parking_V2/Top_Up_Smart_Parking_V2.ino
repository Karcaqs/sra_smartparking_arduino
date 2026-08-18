#include "Config.h"

void setup() {
  Serial.begin(115200);
  initConfig();
  initLittleFS();
  initPin();
  initLCD();
  initRFID();
  initWiFi();
  initAPWiFi();
  initWebServer();
  connectToWiFi();
  initTime();
  initTelegram();
}

void loop() {
  if (!telegramInitialized && WiFi.status() == WL_CONNECTED) {
    delay(2000);
    Serial.print(F("\nTest Telegram connection... "));
    myBot.begin() ? Serial.println(F("OK")) : Serial.println(F("NOK"));
    msgStartup();
    telegramInitialized = true;
    logActivity("System started and Telegram initialized.");
    reportSystemHealth();
  }

  static AppState lastState = STATE_IDLE;
  if (currentState != lastState) {
    handleStateChange(currentState);
    lastState = currentState;
  }

  unsigned long now = millis();

  // Periodic health report
  if (now - lastHealthReport >= HEALTH_REPORT_INTERVAL) {
    reportSystemHealth();
    lastHealthReport = now;
  }

  switch (currentState) {
    case STATE_FIRST:
      if (!stateEntered) {
        buzzerOn();
        amount = 0;
        lcdDashboard();
        stateEntered = true;
      }

      btn3State = digitalRead(btn3);
      if (btn3State == HIGH) {
        buzzerOn();
        stateEntered = false;
        topup = true;
        currentState = STATE_TAP_CARD;
      }

      uidCard = readUidCard();
      if (uidCard != "") {
        drawJpeg("/alert.bmp", 96, 88);
        drawJpeg("/loading.bmp", 214, 150);
        stateEntered = false;
        currentState = STATE_CHECK_SALDO;
      }
      btn3PrevState = btn3State;
      break;

    case STATE_CHECK_SALDO:
      static unsigned long lastCheckSaldoMs = 0;
      if (now - lastCheckSaldoMs >= 3000) {
        checkSaldo();
        lastCheckSaldoMs = now;
      }
      break;

    case STATE_TAP_CARD:
      if (!stateEntered) {
        lcdTapCard();
        stateEntered = true;
      }

      btn1State = digitalRead(btn1);
      if (btn1State == HIGH && btn1PrevState == LOW) {
        buzzerOn();
        stateEntered = false;
        topup = false;
        currentState = STATE_FIRST;
      }

      btn3State = digitalRead(btn3);
      if (btn3State == HIGH && btn3PrevState == LOW) {
        buzzerOn();
        stateEntered = false;
        topup = false;
        currentState = STATE_FIRST;
      }

      uidCard = readUidCard();
      if (uidCard != "") {
        stateEntered = false;
        currentState = STATE_CHECK_SALDO;
      }
      btn1PrevState = btn1State;
      break;

    case STATE_TOPUP:
      if (!stateEntered) {
        lcdTopUp();
        stateEntered = true;
      }

      btn1State = digitalRead(btn1);
      btn2State = digitalRead(btn2);
      btn3State = digitalRead(btn3);

      if (btn1State == HIGH && btn1PrevState == LOW) {
        if (amount > 5000) {
          buzzerOn();
          amount -= 5000;
          tft.setFreeFont(MYFONT28);
          tft.fillRect(100, 180, 280, 30, TFT_WHITE);
          tft.drawString("Rp. " + formatRupiah(amount), tft.width() / 2, 180);
        } else {
          buzzerOn();
          uidCard = "";
          stateEntered = false;
          currentState = STATE_FIRST;
        }
      }

      if (btn2State == HIGH && btn2PrevState == LOW) {
        if (amount < 1000000) {
          buzzerOn();
          amount += 5000;
          tft.setFreeFont(MYFONT28);
          tft.fillRect(100, 180, 280, 30, TFT_WHITE);
          tft.drawString("Rp. " + formatRupiah(amount), tft.width() / 2, 180);
        }
      }

      if (btn3State == HIGH) {
        if (amount > 0) {
          stateEntered = false;
          currentState = STATE_CREATE_QR;
          buzzerOn();
        }
      }

      delay(100);

      btn1PrevState = btn1State;
      btn2PrevState = btn2State;
      btn3PrevState = btn3State;
      break;

    case STATE_CREATE_QR:
      lcdCreateQR();
      createQR();
      break;

    case STATE_CHECK_QR:
      tDown.run();

      if (millis() - lastCheckQR > 3000) {
        lastCheckQR = millis();
        checkQR();
      }
      break;

    case STATE_CHECK_PAYMENT:
      checkPayment();
      break;

    default:
      break;
  }

  handleNewMessage();
}
