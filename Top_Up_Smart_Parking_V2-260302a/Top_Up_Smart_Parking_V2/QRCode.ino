void generateQR(const char* qr) {
  const int qrSize = 61;     // QR Code size (must be odd)
  const int qrVersion = 11;  // QR Code version (1-40)

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(qrVersion)];
  qrcode_initText(&qrcode, qrcodeData, qrVersion, 0, qr);

  int moduleSize = 3;
  int offsetX = 287;
  int offsetY = 100;

  for (int y = 0; y < qrSize; y++) {
    for (int x = 0; x < qrSize; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        tft.fillRect(offsetX + x * moduleSize, offsetY + y * moduleSize, moduleSize, moduleSize, TFT_BLACK);
      } else {
        tft.fillRect(offsetX + x * moduleSize, offsetY + y * moduleSize, moduleSize, moduleSize, TFT_WHITE);
      }
    }
  }
}