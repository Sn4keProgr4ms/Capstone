/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/Desktop/Remys_Labs/Capstone/src/Capstone.ino"
#include <MQTT.h>
#include <oled-wing-adafruit.h>
#include <LIS3DH.h>

void setup();
void loop();
void funcDisplay();
#line 5 "/Users/admin/Desktop/Remys_Labs/Capstone/src/Capstone.ino"
SYSTEM_THREAD(ENABLED);

void callback(char *topic, byte *payload, unsigned int length);
MQTT client("lab.thewcl.com", 1883, callback);

String argon2 = "BAR/argon2/RSSI";
String argon3 = "BAR/argon3/RSSI";

int argon2_distance = 1000;
int argon3_distance = 1000;

OledWingAdafruit display;
LIS3DHSPI accel(SPI, D3, WKP);
SerialLogHandler logHandler;

void connectedCallback(const BlePeerDevice &peer, void *context);
void disconnectedCallback(const BlePeerDevice &peer, void *context);

BleUuid serviceUuid("afe7acc5-33a9-478f-bbe1-8944aa08e884");

void setup()
{

  display.setup();

  client.subscribe(argon2);
  client.subscribe(argon3);

  BLE.on();

  BLE.onConnected(connectedCallback, NULL);
  BLE.onDisconnected(disconnectedCallback, NULL);

  BleAdvertisingData advData;
  advData.appendServiceUUID(serviceUuid);
  BLE.advertise(&advData);

  LIS3DHConfig config;
  config.setAccelMode(LIS3DH::RATE_100_HZ);
  accel.setup(config);
}

void loop()
{
  display.loop();

  if (client.isConnected())
  {
    client.loop();
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe(argon2);
    client.subscribe(argon3);
  }
  
  funcDisplay();

  LIS3DHSample sample;
  if (accel.getSample(sample))
  {
    display.println("A2 dist : A3 dist");
    display.printf("%d,%d", argon2_distance, argon3_distance);
    display.display();
  }



}

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String s = p;
  double value = s.toFloat();

  String callbackTopic = topic;

  if (callbackTopic.compareTo(argon2))
  {
    argon2_distance = value;
  }
  else
  {
    argon3_distance = value;
  }
}

void connectedCallback(const BlePeerDevice &peer, void *context)
{
  Log.info("connected");
}

void disconnectedCallback(const BlePeerDevice &peer, void *context)
{
  Log.info("disconnected");
}

void funcDisplay()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

