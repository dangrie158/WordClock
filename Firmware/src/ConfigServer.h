#include "Arduino.h"
#include <ESP8266WebServer.h>
#include <FS.h>

#ifndef _CONFIG_SERVER_H_
#define _CONFIG_SERVER_H_

#define CONFIG_SERVER_WEB_INDEX "index.html"
#define CONFIG_FILE "optionsfile"

class Config {
public:
  void load();
  void save();

  String station;
  String passphrase;
  String server;
  String brightness;
  String timezone;
};

class ConfigServer {
private:
  ESP8266WebServer *mServer;
  Config *mConfig;
  String getContentType(String filename);
  bool handleFileRead(String path);

public:
  explicit ConfigServer(int port)
      : mServer(new ESP8266WebServer(80)), mConfig(new Config()){};
  ~ConfigServer() {
    delete mServer;
    mConfig->save();
    delete mConfig;
  };

  void setup();
  void handleClient();
  inline Config *getConfig() { return mConfig; }
};

#endif //_CONFIG_SERVER_H_
