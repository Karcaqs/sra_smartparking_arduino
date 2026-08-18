void initConfig() {
  loadEnvironment();
  loadWiFiConfig(SSID_STA, PASSWORD_STA);
}

void saveWiFiConfig(String ssid, String pass) {
  prefs.begin("config", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.end();
}

void loadWiFiConfig(String &ssid, String &pass) {
  prefs.begin("config", true);
  // ssid = prefs.getString("ssid", "Tselhome-4D33");
  // pass = prefs.getString("pass", "70761510");
  ssid = "Tselhome-4D34";
  pass = "70761510";
  prefs.end();
}

void saveEnvironment(const String &env) {
  prefs.begin("config", false);
  prefs.putString("environment", env);
  prefs.end();
}

void loadEnvironment() {
  prefs.begin("config", true);
  envName = prefs.getString("environment", "PRODUCTION");
  prefs.end();
}
