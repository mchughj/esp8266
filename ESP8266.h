/*
 * A library to make it easier to use the ESP8266 module when that 
 * module is connected, and directly controlled via the AT command set,
 * to an arduino. 
 *
 * Presumes that the Arduino is using the single hardware serial 
 * port to communicate with a host machine for diagnosing/debugging/or
 * whatever and this version of the library uses AltSoftSerial to 
 * communicate with the ESP8266 serial.
 *
*/

#include <Arduino.h>  // for type definitions

#ifndef ESP8266_h
#define ESP8266_h

#include <AltSoftSerial.h>

typedef int (*DataCallback)(char *);
typedef void (*ConnectCallback)(void);

enum WifiMode {
  WIFI_MODE_STA = 1,
  WIFI_MODE_AP,
  WIFI_MODE_APSTA
};

enum wifiErrors {
  WIFI_ERR_NONE = 0,
  WIFI_ERR_AT,
  WIFI_ERR_RESET,
  WIFI_ERR_CONNECT,
  WIFI_ERR_LINK  
};

enum DebugLevel {
  DEBUG_LEVEL_NONE = 0,
  DEBUG_LEVEL_ERROR_MESSAGES,
  DEBUG_LEVEL_ALL_DATA
};

class ESP8266
{
  public:
    /*
     * Construct the ESP8266 instance.  The mode indicates which wifiMode 
     * the chip should be set into.  There are three valid values for the 
     * debug
     * Constructor - set link mode, baudrate and debugLevel. 
     */
    ESP8266(WifiMode mode = WIFI_MODE_STA, long baudrate = 9600, DebugLevel debugLevel = DEBUG_LEVEL_NONE);
    
    // init / connect / disconnect access point
    int initializeWifi(DataCallback dcb, ConnectCallback ccb);
    int connectWifi(char *ssid, char *password);
    bool disconnectWifi();
    
    // server
    bool startServer(int port = 8000, long timeout = 300);
    
    // client
    bool startClient(char *ip, int port, long timeout = 300);
    
    // discovery beacon
    bool enableBeacon(char *device);
    bool disableBeacon();
    
    // send data across the link
    bool send(char *data);
    
    // process wifi messages - MUST be called from main app's loop
    void run();

    // informational
    char *ip();
    int scan(char *out, int max);
  private:
    void clearResults();
    bool sendData(int chan, char *data);
    bool setLinkMode(int mode);
    bool startUDPChannel(int chan, char *address, int port);
    void processWifiMessage();
    bool getIP();
    bool getBroadcast();
    void debug(char *msg);
    bool searchResults(char *target, long timeout, int dbg = 0);

};

#endif


