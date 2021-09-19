# 4-Channel-Mains-Current-Sensor-ESP8266
A 4-Channel mains sensor with an option for either a Wemos D1 Mini or an ESP-12 WiFi module. This board uses the ESP8266's on-board A/D (12-bit) with an external multiplexer to select the channel to be read. 

###### Firmware Version 1

Using the ESP8266's own A/D (10-bit) to measure the output of the current transformer, this version measure the peak values of the mains sine wave then calculating the RMS voltage value.

###### Firmware Version 2

This is based of the EmonLib https://github.com/openenergymonitor/EmonLib library. This works different from version 1 and as some digital filtering which improves resolution at lower power levels.

### PCB Layout

![](https://github.com/Mottramlabs/4-Channel-Mains-Current-Sensor-ESP8266/blob/main/Pictures/PIX201136.jpg)
