void initTelegram() {
  client.setCACert(telegram_cert);

  myBot.setUpdateTime(1000);
  myBot.setTelegramToken(BOT_TOKEN);
  myBot.setFormattingStyle(AsyncTelegram2::FormatStyle::MARKDOWN);

  Serial.print("\nTest Telegram connection... ");
  myBot.begin() ? Serial.println(F("OK")) : Serial.println(F("NOK"));

  myReplyKbd.addButton(ON);
  myReplyKbd.addButton(OFF);
  myReplyKbd.addButton(PING);
  myReplyKbd.enableResize();
}

String escapeMarkdown(String text) {
  String escaped = text;
  escaped.replace("_", "\\_");
  // escaped.replace("*", "\\*");
  escaped.replace("[", "\\[");
  escaped.replace("]", "\\]");
  escaped.replace("(", "\\(");
  escaped.replace(")", "\\)");
  escaped.replace("~", "\\~");
  escaped.replace("`", "\\`");
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

void handleNewMessage() {
  TBMessage msg;

  if (myBot.getNewMessage(msg)) {
    MessageType msgType = msg.messageType;
    String msgText = msg.text;
    int64_t userid = msg.chatId;

    switch (msgType) {
      case MessageText:
        Serial.print("\nText message received: ");
        Serial.println(msgText);

        if (msgText.equalsIgnoreCase("/start")) {
          myBot.sendMessage(msg, "⬇️ Gunakan Keyboard di bawah ini ⬇️", myReplyKbd);
        }
        break;

      case MessageReply:
        if (msgText.equalsIgnoreCase(ON)) {
          handleON();
        } else if (msgText.equalsIgnoreCase(OFF)) {
          handleOFF();
        } else if (msgText.equalsIgnoreCase(PING)) {
          handlePing();
        }
        break;

      default:
        break;
    }
  }
}

void handleStartUp() {
  myBot.sendTo(CHAT_ID, escapeMarkdown("Smart Switch Siap ✅"));
}