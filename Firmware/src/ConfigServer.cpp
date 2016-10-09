#include <ESP8266WiFi.h>

#include "ConfigServer.h"

void Config::load() {
  if (SPIFFS.exists(CONFIG_FILE)) {
    File options = SPIFFS.open(CONFIG_FILE, "r");
    this->station = options.readStringUntil('\n');
    this->passphrase = options.readStringUntil('\n');
    this->server = options.readStringUntil('\n');
    this->brightness = options.readStringUntil('\n');
    this->timezone = options.readStringUntil('\n');

    this->station.trim();
    this->passphrase.trim();
    this->server.trim();
    this->brightness.trim();
    this->timezone.trim();
  }
}

void Config::save() {
  File options = SPIFFS.open(CONFIG_FILE, "w");
  options.println(this->station);
  options.println(this->passphrase);
  options.println(this->server);
  options.println(this->brightness);
  options.println(this->timezone);
  options.flush();
  options.close();
}

void ConfigServer::setup() {
  // init the flash file system
  SPIFFS.begin();

  mConfig->load();

  mServer->on("/stations", HTTP_GET, [this]() {
    int stationsFound = WiFi.scanNetworks();
    Serial.println("Scanning for Stations...");
    String response = "[";
    for (int index = 0; index < stationsFound; index++) {
      response += "{\"ssid\":\"";
      response += WiFi.SSID(index);
      response += "\", \"locked\":";
      response +=
          (WiFi.encryptionType(index) != ENC_TYPE_NONE) ? "true" : "false";
      response += "}";
      if (index < (stationsFound - 1)) {
        response += ",";
      }
    }
    response += "]";

    Serial.println("Stations found: " + stationsFound);
    Serial.println(response);

    this->mServer->send(200, "application/json", response);
  });

  mServer->on("/time", HTTP_GET, [this]() {

  });

  mServer->on("/time", HTTP_POST, [this]() {

  });

  mServer->on("/timezone", HTTP_GET, [this]() {
    String response = "\"" + this->mConfig->timezone + "\"";
    this->mServer->send(200, "application/json", response);
  });

  mServer->on("/timezone", HTTP_POST, [this]() {
    this->mConfig->timezone = this->mServer->arg(0);
    Serial.println("Received new Timezone");
    Serial.println("Value:" + this->mConfig->timezone);
    this->mConfig->save();
    String response = "\"" + this->mConfig->timezone + "\"";
    this->mServer->send(200, "application/json", response);
  });

  mServer->on("/brightness", HTTP_GET, [this]() {
    String response = "\"" + this->mConfig->brightness + "\"";
    this->mServer->send(200, "application/json", response);
  });

  mServer->on("/brightness", HTTP_POST, [this]() {
    this->mConfig->brightness = this->mServer->arg(0);
    Serial.println("Received new Brightness");
    Serial.println("Value:" + this->mConfig->brightness);
    this->mConfig->save();
    String response = "\"" + this->mConfig->brightness + "\"";
    this->mServer->send(200, "application/json", response);
  });

  mServer->on("/station", HTTP_GET, [this]() {
    String response = "{\"ssid\":\"" + this->mConfig->station +
                      "\", \"passphrase\":\"" + this->mConfig->passphrase +
                      "\"}";
    this->mServer->send(200, "application/json", response);
  });

  mServer->on("/station", HTTP_POST, [this]() {
    this->mConfig->station = this->mServer->arg("station");
    this->mConfig->passphrase = this->mServer->arg("passphrase");
    Serial.println("Received new WiFi Config");
    Serial.println("Station:" + this->mConfig->station);
    Serial.println("Passphrase:" + this->mConfig->passphrase);
    this->mConfig->save();
    this->mServer->send(200, "application/json", "");
    WiFi.begin(this->mConfig->station.c_str(),
               this->mConfig->passphrase.c_str());
    if (WiFi.isConnected()) {
      Serial.println("Disconnecting...");
      WiFi.disconnect();
    }
    Serial.println("Connecting...");
  });

  mServer->on("/server", HTTP_GET, [this]() {
    String response = "\"" + this->mConfig->server + "\"";
    this->mServer->send(200, "application/json", response);
  });

  mServer->on("/server", HTTP_POST, [this]() {
    this->mConfig->server = this->mServer->arg(0);
    Serial.println("Received new NTP Server");
    Serial.println("Value:" + this->mConfig->server);
    this->mConfig->save();
    this->mServer->send(200, "application/json", "");
  });

  // called when the url is not defined elsewhere
  // use it to load content from SPIFFS
  mServer->onNotFound([this]() {
    if (!this->handleFileRead(this->mServer->uri())) {
      this->mServer->send(404, "text/plain", "Not Found");
    }
  });

  mServer->begin();
}
void ConfigServer::handleClient() { mServer->handleClient(); }

String ConfigServer::getContentType(String filename) {
  if (mServer->hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  return "text/plain";
}

bool ConfigServer::handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  // handle requests to the index of the web server
  if (path.endsWith("/")) {
    path += CONFIG_SERVER_WEB_INDEX;
  }
  String contentType = getContentType(path);
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = mServer->streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
