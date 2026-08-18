void initLCD() {
  tft.begin();
  tft.setRotation(3);
  tft.setFreeFont(MYFONT18);
  tft.setTextColor(TFT_BLACK);
  lcdHeader();
}

void lcdHeader() {
  drawJpeg("/bg.bmp", 0, 0);
  tft.setFreeFont(MYFONT18);
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("TOP UP SMART PARKING", tft.width() / 2, 15);
  tft.setTextColor(TFT_BLACK);
}

void lcdConnectingToWiFi() {
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Connecting to WiFi", tft.width() / 2, 90);
  drawJpeg("/wifi.bmp", 214, 120);
}

void lcdConnectedToWiFi() {
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Connected to WiFi", tft.width() / 2, 186);
}

void lcdInitTime() {
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Sinkronisasi Waktu dengan Server", tft.width() / 2, 230);
}

void lcdDashboard() {
  lcdHeader();

  tft.setFreeFont(MYFONT24);
  tft.drawString("Untuk Cek Saldo", tft.width() / 2, 90);
  tft.drawString("Silakan Tap kartu Anda", tft.width() / 2, 120);

  tft.drawString("Untuk Top Up Saldo", tft.width() / 2, 190);
  tft.drawString("Silakan Tekan Tombol OK", tft.width() / 2, 220);
}

void lcdShowSaldo(int balance) {
  tft.setFreeFont(MYFONT24);
  tft.drawString("Saldo Anda", tft.width() / 2, 138);
  tft.setFreeFont(MYFONT28);
  tft.drawString("Rp. " + formatRupiah(balance), tft.width() / 2, 180);
}

void lcdCountimer() {
  tft.fillRect(95, 250, 100, 30, TFT_BG);
  tft.setFreeFont(MYFONT28);
  tft.drawString(tDown.getCurrentTime(), 145, 250);
  tft.setFreeFont(MYFONT24);
}

void lcdSuccess() {
  lcdHeader();
  drawJpeg("/alert.bmp", 96, 88);
  drawJpeg("/success.bmp", 214, 108);
  tft.setFreeFont(MYFONT24);
  tft.drawString("Berhasil", tft.width() / 2, 172);
  tft.setFreeFont(MYFONT18);
  tft.drawString("Pembayaran Berhasil", tft.width() / 2, 210);
}

void lcdExpired() {
  lcdHeader();
  drawJpeg("/alert.bmp", 96, 88);
  drawJpeg("/failed.bmp", 214, 108);
  tft.setFreeFont(MYFONT24);
  tft.drawString("Gagal", tft.width() / 2, 172);
  tft.setFreeFont(MYFONT18);
  tft.drawString("Pembayaran Kadaluarsa", tft.width() / 2, 210);
}

void lcdNotRegistered() {
  lcdHeader();
  drawJpeg("/alert.bmp", 96, 88);
  drawJpeg("/failed.bmp", 214, 108);
  tft.setFreeFont(MYFONT24);
  tft.drawString("Mohon Maaf", tft.width() / 2, 172);
  tft.setFreeFont(MYFONT18);
  tft.drawString("Kartu Anda tidak terdaftar", tft.width() / 2, 210);
}

void lcdTopUp() {
  lcdHeader();
  drawJpeg("/alert.bmp", 96, 88);
  tft.setFreeFont(MYFONT24);
  tft.drawString("Jumlah Top Up", tft.width() / 2, 130);
}

void lcdTapCard() {
  lcdHeader();

  tft.setFreeFont(MYFONT24);
  tft.drawString("Silakan Tap Kartu Anda", tft.width() / 2, 162);
}

void lcdCreateQR() {
  lcdHeader();
  drawJpeg("/qr.bmp", 278, 45);

  tft.setFreeFont(MYFONT18);
  tft.drawString("Lakukan", 145, 80);
  tft.drawString("Pembayaran", 145, 104);
  tft.drawString("dengan memindai", 145, 128);
  tft.drawString("QRIS di samping ->", 145, 152);
  tft.setFreeFont(MYFONT28);
  tft.drawString("Rp. " + formatRupiah(amount), 145, 200);
}

void drawJpeg(const char *filename, int16_t x, int16_t y) {

  if ((x >= tft.width()) || (y >= tft.height())) return;

  fs::File bmpFS;

  bmpFS = LittleFS.open(filename, "r");

  if (!bmpFS) {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h, row, col;
  uint8_t r, g, b;

  uint32_t startTime = millis();

  if (read16(bmpFS) == 0x4D42) {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0)) {
      y += h - 1;

      bool oldSwapBytes = tft.getSwapBytes();
      tft.setSwapBytes(true);
      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {

        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t *bptr = lineBuffer;
        uint16_t *tptr = (uint16_t *)lineBuffer;
        for (uint16_t col = 0; col < w; col++) {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        tft.pushImage(x, y--, w, 1, (uint16_t *)lineBuffer);
      }
      tft.setSwapBytes(oldSwapBytes);
    } else {
      Serial.println(F("BMP format not recognized."));
    }
  }
  bmpFS.close();
}

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();  // LSB
  ((uint8_t *)&result)[1] = f.read();  // MSB
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();  // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();  // MSB
  return result;
}
