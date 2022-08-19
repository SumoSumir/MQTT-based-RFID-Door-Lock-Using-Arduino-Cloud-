# MQTT based RFID Door Lock Using Ardunino Cloud
Showcasing my IOT project which used Ardunio Cloud and MQTT communication standard to log and control the access management of a RFID based door lock.

## Arduino IoT Cloud Variables

  String entryID;
  
  String whiteList;

## PINOUT (NodeMCU 8266 ESP-12E)

  MOSI: GPIO 13 - D7
  
  MISO: GPIO 12 - D6
  
  SCK/ CLK: GPIO 14 - D5 (Slave Clock or Clock)
  
  SS/ CS/ SDA : GPIO 15 - D8 (Chip select or Slave Select)
  
  RST : GPIO 2 - D4

## Example whiteList input -> 
"00_00_00_00-Jai Vaswani;00_01_02_03-Sumir Broota"

    1) First, split based on ';'
    2) For each part of string after splitting, split based on '-'
    3) First part of this gets added to decimal_array
    4) Second part of this gets added to names_array
