// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = ""; //fill with your details

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onEntryIDChange();
void onWhiteListChange();

String entryID;
String whiteList;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(entryID, READWRITE, ON_CHANGE, onEntryIDChange);
  ArduinoCloud.addProperty(whiteList, READWRITE, ON_CHANGE, onWhiteListChange, 10);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);