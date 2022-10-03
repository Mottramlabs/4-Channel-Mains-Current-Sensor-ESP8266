## These instructions are for a beginner to set up a ML201136 board on a 3 phase supply, using the components below

From this forum comes the source of the setup information: https://community.home-assistant.io/t/another-power-monitoring-solution-uk-mottramlabs-ct-clamp-board/338139


## Components
 - 1x ML201136 (Blue Board)
- 4x SCT013 clamp meters
- 1x ESP8266 (Wemos D1 Pro)
- 1x 12V 2A Power supply


## Assembly and configuration

Bear in mind that this is the first time I've used ESPHome so your experience may exceed mine, also I may have a few glaring holes in my setup, but that will hopefully be fixed upon creating more or receiving advice.

1. Plug the Wemos into the board, and power it via the 12V cable.
2. Plug a USB cable into the Wemos, and your computer.
3. Install ESPHome using the documentation here: https://www.home-assistant.io/integrations/esphome/
4. Adding a device to ESPHome can be done a few ways, I'd recommend getting familiar with the official documentation before continuing here: https://esphome.io/guides/getting_started_hassio.html
5. At the end of this, you should have a board name, and it should be on your wireless network with a secure setup.
6. You may have another way of doing this, but I use my Windows PC to browse to the SMB share, if you've not set this up, you can add it via Add-ins: https://github.com/home-assistant/addons/tree/master/samba
7. Use Windows Explorer to open Home Assistant from Windows, find 'config', then 'esphome'. 
8. A .yaml file will be there with the name you just gave it.
9. Open it, and paste the text from the configuration file below what's already there.
   - a. I've omitted the 'ota:', 'api:' and 'wifi' parts from the .yaml, ensure you keep those from your own.
   - b. I do have a way of getting the current voltage which differs greatly in my area. This will be adjusted at a later date, but I'm using a measured static one for now. Note where 'ic1Volts' is, and only adjust it now if you're confident you know it.
10. When you're happy with it, save it, and go back to Home Assistant under Settings | Add-ons | ESPHome | <device name> | drop down 'Validate' and if okay 'Install' | use 'Wirelessly' if it's already set up. This takes a long time to finish.
11. Assuming it all works, go back in to ESPHome, open the WebUI, and click 'Visit'.
12. It should open a window with a load of lovely information. You're interested in ct1Amps to ct4Amps which should all be reading 0.0A
13. You will now need to set up a test environment, and tune the clamp meters to your home setup.


## Assembly of a test environment

Create a test environment which allows you to measure the current on a known load. I didn't have a known load, so I used all my test meters in a circuit.

In this example I wasn't confident that my clamp multi meter was working correctly, so I used a belt and braces approach, if you're happy yours is working (turns out mine was fine) then skip the part exposing the cables on either end, and simply remove the insulation from a disposable extension cable, and attach it to the live or neutral along with two other clamps.

To follow my example, consider the fact that you'll need  something which breaks out both a 240V load such as a heater, and a 240V supply such as a computer power cable to exposed wires. If you're not comfortable doing this, don't attempt it.

1. Carefully remove about 10CM of the insulation from the middle of a computer power lead, exposing the live and neutral.
2. Clamp the four clamps over the live and neutral leads, two on the live, two on the neutral (doesn't matter which, both will provide the same readings).
3. Remove the end of the cable which plugs into the computer, and expose the cables. Attach them to a 'chocolate block' terminal connector, any will do as long as the cables fit and they exceed 10A. You'll need four sections of it.
4. The live from the power cables goes into block 1, the other side of which is clamped the red cable, which goes to the 10A connector on your multi meter (if you're using a clamp meter this will differ).
   - a. The black cable from the multi meter comes out and is attached to block 2, which goes to the live cable of your load. I used a small space heater which put out 6A.
   - b.  The neutrals of the load and supply are connected via block 3, as are the earths in block 4, although not strictly necessary in this temporary setup.
5. If you have a clamp multi meter but like me don't fully trust it, you can put it over either test lead, or next to any of the clamps assuming you have enough room.
6. Check the connections are correct, then re-check them following all the cables and exposed parts, if any of them could be accidentally touched, re-do it.
7. Turn it on and employ safety squints. Also ensure one hand is in a pocket whenever feasible.
8. Note the Amps for the multi meter, if you have more than one, get an average, but rely more heavily on one that completes a circuit than a clamp meter. Hopefully the device you used to draw the current is high enough to be over 0.x Amps, or the readings will be off (the higher the current, the more likely it is that your tuning will match real-life currents of 10+Amps.
   - a. For my example I used a small space heater rated at 1500W (6.25A at 240v), that's a theoretical maximum, in reality it'll only peak temporarily at that current, so let it 'settle' on a current before taking it. Also ensure that you keep referring to it if it's a bit up and down.


## Tuning the clamps

1. Measure the voltage using a multi meter, and put it in the .yaml you adjusted earlier next to 'ic1Volts:'
2. Save it and note the following, as you'll need to repeat it:
a. Under Settings | Add-ons | ESPHome | Open Web UI drop the three dots under your device, and click 'Validate', if it passes (no red), click 'install | Wirelessly' otherwise go back to the .yaml and fix it before repeating the process.
3. Edit the .yaml again, and scroll down to 'sensor:'. Under there, are four entries starting 'platform: ct_clamp', look for the 'filters:' section.
   - a. For the first sensor ensure it's all set up in the test environment, and beneath the '- 0 -> 0' entry there should be an example of how it's set up for a different sensor. ' - 0.0xx -> 5.7xx' the left number refers to the measured current on the sensor, you can find this number in one of two places ('Visit' or 'Logs') I'd recommend using 'Logs' as it's not moving up the screen all the time.
   - b. Under Settings | Add-ons | ESPHome | Logs | wait for the cyan text to show a few entries. Look for 'CT1' as the first clamp meter, then scroll up to stop it being difficult to read.
   - c. There should be two entries with 'ct1Amps', you're looking for 'Raw AC Value'. Something like '0.033A'.
   - d. Take another note of the 'actual' current from your multi meter again, and ensure it's showing a steady value, i.e. 5.25A
4. You now have two values to overwrite in the .yaml, the left number is the 'Raw AC Value', and the right is the multi meter measured value. So overwrite the decimal values with these values. i.e. ' - 0.033 -> 5.25'.
   - a. If you're lucky enough to have multiple settings on the load you're measuring, you can switch to a higher or lower load, to test you're getting a variable Raw AC value after switching.
5. Go back to the logs, and scroll to the bottom so it starts auto-scrolling again, now you're looking for the second 'ct1Amps' line which shows '[sensor:xxx]: 'ct1Amps': Sending state x.xx A with 5 decimals of accuracy', hopefully it'll be in the region of '5.25 A'. If not, adjust the second number (after the '->') until it does, but take care to note that the output may vary somewhat, meaning you may be adjusting for an 'old' value from the multi meter, the process is imperfect, so you may have to settle for a 'near enough' number to what's showing on your multi meter.
6. Process to make the change to the device: 
   - a. Make the change to the filters value in the .yaml. 
   - b. Save it. 
   - c. Under Settings | Add-ons | ESPHome | <device name> | drop down 'Validate' and if okay 'Install'.
       - i. It's possible to bypass the Validate step after a few goes, if it stays okay.
7. If you're lucky all clamps will have the same values, but it's unlikely. Once you're happy all four clamps are set up correctly (all showing near enough the same current as the multi meter), it's time to move on to the 'Total Amps'
   - a. In the .yaml, scroll down to 'Total Amps', and look for the 'lambda' line.
   - b. If your setup is like mine, you're only looking to add the three phases together to get an idea of the total watts used by all three phases, so the following line will add them all:
` lambda: return id(ct1Amps).state + id(ct2Amps).state + id(ct3Amps).state;`
   - c. The Total Watts and kWh entries rely on this, so don't need altering.
8. Follow the same process to save and update the device.


## Physical assembly for three phase setup

1. With the board and all the components together, put the clamps over all three phases, noting which port each phase is in, I labelled the cables on both ends, and noted which phase each one was attached to (1 = Red, 2 = White, 3 = Blue).
   - a. The fourth can be used to put on any other input or output you'd like, I already had a Solar production one, so I used one (ct4Amps) for a high current device.

Home Assistant setup for useable graphs

##### Assumptions: You already have a solar reading from your home solar. (You could use the fourth clamp for this).

I will add to this if I make any further adjustments, but for now, assuming you have either default_config: or energy: in your configuration.yaml, you'll see an 'Energy' dashboard, which may be all you need.
I've set up a separate 'Power' view to get a bit more granular with what I'm seeing. You may notice that simply adding three history graphs showing mains usage for the three phases shows a huge spike on one of them (or all three if you're on 3 phase solar) during the day. As it's doesn't distinguish between generating and drawing current, you'll need to do something to get a Net Power Consumption number.
In this example I only have one phase attached to my solar.

1. Edit your configuration.yaml found in \\homeassistant\config, and add the following lines beneath an existing 'template:' heading (create one if it doesn't exist):
```
  - sensor:
      - name: "Net Power Consumption"
        unit_of_measurement: "W"
        state: >
          {{int(states.sensor.mainspower_ct1_watts.state) + int(states.sensor.mainspower_ct2_watts.state) + int(states.sensor.mainspower_ct3_watts.state) - int(states.sensor.fronius_ac_power.state) }}
```
2. This will add the three phases together, then remove the solar power. My example pulls my fronius inverter Power, but you could in theory use the fourth clamp for this.
3. Save, then restart HA.
4. You should now see a sensor when editing the dashboard which shows 'Net Power Consumption', add a History Graph Card Configuration, and you'll see a W line showing if you're generating or using power.
