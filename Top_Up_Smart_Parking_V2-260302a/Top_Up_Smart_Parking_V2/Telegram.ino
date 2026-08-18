void initTelegram() {
  client.setCACert(telegram_cert);
  myBot.setUpdateTime(2000);
  myBot.setTelegramToken(BOT_TOKEN);
  myBot.setFormattingStyle(AsyncTelegram2::FormatStyle::MARKDOWN);
  myBot.addSentCallback(messageSent);
}

void messageSent(bool sent) {
  if (sent) {
    Serial.println(F("Last message was delivered"));
  } else {
    Serial.println(F("Last message was NOT delivered"));
  }
}

String escapeMarkdown(String text) {
  String escaped = text;
  escaped.replace("_", "\\_");
  escaped.replace("[", "\\[");
  escaped.replace("]", "\\]");
  escaped.replace("(", "\\(");
  escaped.replace(")", "\\)");
  escaped.replace("~", "\\~");
  escaped.replace(">", "\\>");
  escaped.replace("#", "\\#");
  escaped.replace("+", "\\+");
  escaped.replace("-", "\\-");
  escaped.replace("=", "\\=");
  escaped.replace("|", "\\|");
  escaped.replace("{", "\\{");
  escaped.replace("}", "\\}");
  escaped.replace(".", "\\.");
  escaped.replace("!", "\\!");
  return escaped;
}

void msgStartup() {
  String msg = "🚀 *Device Started Up*\n";
  msg += "🆔 ID: `Top Up Smart Parking " + mac + "`\n";
  msg += "📶 WiFi: *" + WiFi.SSID() + "*\n";
  msg += "🌐 IP: `" + WiFi.localIP().toString() + "`\n";
  msg += "🕒 Time: `" + rtc2.getTime("%Y-%m-%d %H:%M:%S") + "`";
  myBot.sendTo(CHAT_ID, escapeMarkdown(msg));
}

void msgNotRegistered() {
  String msg = "🚫 *Kartu Tidak Terdaftar*\n";
  msg += "💳 UID : `" + uidCard + "`\n";
  msg += "🔎 Status : `Ditolak - Tidak terdaftar di sistem`";
  myBot.sendTo(CHAT_ID, escapeMarkdown(msg));
}

void msgShowSaldo(int balance) {
  String msg = "💳 *Cek Saldo*\n";
  msg += "🆔 UID : `" + uidCard + "`\n";
  msg += "💰 Saldo : *Rp " + formatRupiah(balance) + "*";
  myBot.sendTo(CHAT_ID, escapeMarkdown(msg));
}

void msgCreatePayment() {
  String msg = "🆕 *Create Top Up*\n";
  msg += "🆔 ID Pembayaran : `" + qrId + "`\n";
  msg += "💳 UID : `" + uidCard + "`\n";
  msg += "💰 Nominal : *Rp " + formatRupiah(amount) + "*\n";
  msg += "🔗 Status : `Menunggu Pembayaran`";
  myBot.sendTo(CHAT_ID, escapeMarkdown(msg));
}

void msgSuccess() {
  String msg = "✅ *Pembayaran Berhasil*\n";
  msg += "🆔 ID Pembayaran : `" + qrId + "`\n";
  msg += "💳 UID : `" + uidCard + "`\n";
  msg += "💰 Nominal : *Rp " + formatRupiah(amount) + "*\n";
  msg += "📦 Status : `Saldo Bertambah`";
  myBot.sendTo(CHAT_ID, escapeMarkdown(msg));
}

void msgExpired() {
  String msg = "⌛ *Pembayaran Expired*\n";
  msg += "🆔 ID Pembayaran : `" + qrId + "`\n";
  msg += "💳 UID : `" + uidCard + "`\n";
  msg += "💰 Nominal : *Rp " + formatRupiah(amount) + "*\n";
  msg += "❌ Status : `Gagal / Timeout`";
  myBot.sendTo(CHAT_ID, escapeMarkdown(msg));
}

void handleNewMessage() {
  TBMessage msg;

  if (myBot.getNewMessage(msg)) {
    String message = "\n-----------------------------\n";
    message += msg.text;
    Serial.println(message);

    if (msg.text.indexOf("/ping") > -1) {
      myBot.sendTo(CHAT_ID, "🟢 *Device is Online*\n📱 Status OK");
    }
    else if (msg.text.indexOf("/reboot") > -1) {
      ESP.restart();
    }
  }
}
