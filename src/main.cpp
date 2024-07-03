#define DEBUG 0

#define SSR1 25
#define SSR2 27

#define BTDEVICE_NAME "ESP32DMXController"
#define BAUDRATE      9600



#include <Arduino.h>
#include <SparkFunDMX.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
SparkFunDMX dmx;

uint8_t brightness = 255;



void all_dmx_off() {
  dmx.write(1, 0);
  dmx.write(2, 0);
  dmx.write(3, 0);
  dmx.write(4, 0);
  dmx.write(5, 0);
  dmx.write(6, 0);
  dmx.write(7, 0);
  dmx.write(8, 0);
  dmx.update();
}

// void process_data(String btData){
//   if (DEBUG) {
//     Serial.println("BT DATA: " + btData);
//   }
//   uint8_t dmx_channel = btData.substring(0, 1).toInt();
//   uint8_t dmx_value = btData.substring(1).toInt();
//   if (DEBUG) {
//     Serial.println("DMX CHANNEL: " + String(dmx_channel));
//     Serial.println("DMX VALUE: " + String(dmx_value));
//   }
//   dmx.write(dmx_channel, dmx_value);
//   dmx.update();
// }


void process_data(String btData){
  if (btData=="LON"){
    dmx.write(1, 255);
    dmx.write(2, 255);
    dmx.write(3, 255);
    dmx.write(4, 255);
    dmx.write(5, 255);
    dmx.write(6, 255);
    dmx.write(7, 255);
    dmx.write(8, 255);  
  } if (btData=="LOFF"){
    dmx.write(1, 0);
    dmx.write(2, 0);
    dmx.write(3, 0);
    dmx.write(4, 0);
    dmx.write(5, 0);
    dmx.write(6, 0);
    dmx.write(7, 0);
    dmx.write(8, 0);
  } if (btData=="A"){
    digitalWrite(SSR1, LOW);
    delay(5000);
    digitalWrite(SSR1, HIGH);
    delay(1000);

    // digitalWrite(SSR1, HIGH);
    // delay(5000);
    // digitalWrite(SSR1, LOW);
    // delay(1000);

  } if (btData=="Z"){
    digitalWrite(SSR2, LOW);
    delay(5000);
    digitalWrite(SSR2, HIGH);
    delay(1000);

    // digitalWrite(SSR2, HIGH);
    // delay(5000);
    // digitalWrite(SSR2, LOW);
    // delay(1000);

  } if (DEBUG){Serial.println("BTDATA" + String(btData));}

  
  dmx.update();

}

void updateBrightness(uint8_t brightness){
  dmx.write(1, brightness);
  dmx.write(2, brightness);
  dmx.write(3, brightness);
  dmx.write(4, brightness);
  dmx.write(5, brightness);
  dmx.write(6, brightness);
  dmx.write(7, brightness);
  dmx.write(8, brightness);
  dmx.update();
}

void process_bt_data(String recieved_data)
{
  uint8_t seperation_index = recieved_data.lastIndexOf("B");
  //  if (DEBUG){Serial.println("SI: " + String(seperation_index));}
  if (seperation_index == 255) {
    process_data(recieved_data);
  }
  else if (seperation_index == 0) {
    String required_data = recieved_data.substring(seperation_index + 1);
    brightness = required_data.toInt();
    updateBrightness(brightness);
    if (DEBUG) {
      Serial.println("BRIGHTNESS: " + String(brightness));
    }
  }
}

void read_bt_data()
{
  if (SerialBT.available()) {
    String recieved_data = SerialBT.readString();
    recieved_data.trim();
    process_bt_data(recieved_data);
  }
}

void setup() {
  Serial.begin(BAUDRATE);
  Serial.setTimeout(1);
  SerialBT.begin(BTDEVICE_NAME); // Bluetooth device name
  SerialBT.setTimeout(1);
  delay(1000);
  pinMode(SSR1, OUTPUT);
  pinMode(SSR2, OUTPUT);
  digitalWrite(SSR1, HIGH);
  digitalWrite(SSR2, HIGH);
  dmx.initWrite(500);

  all_dmx_off();
  
  
}

void loop() {
  read_bt_data();
  
}

