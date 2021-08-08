#include <ESP8266WiFi.h>              // needed for EPS8266

// I/O items
#define Network_LED 14
#define Run_LED 16
#define Speaker 13
#define Test 12

// heartbeat timer, time between printed results
#define Heatbeat    500                          // heartbeat timer value, in mS
long LastMsg = 0;                                 // message sent counter

// sound library
#include <ESP8266_Tones.h>
ESP8266_Tones Mytone(Speaker);

// Mux connections
#define Select_A 4
#define Select_B 5

// library for the MLP201136 PCB
#include "MLP201136.h"
// make an instance of MLP201136
MLP201136 My_PCB(Select_A, Select_B);

#define Samples_Taken 200                         // sample to be taken per channel
int8_t ADS_Input = 0;                             // A/D channel select
int16_t Value[4] = {0, 0, 0, 0};                  // array for results


void setup() {

  // start the serial connection
  Serial.begin(115200); Serial.println(""); Serial.println(""); Serial.println("Up and Clackin!");
  Serial.println(__FILE__);

  // setup I/O
  pinMode(Run_LED, OUTPUT); digitalWrite(Run_LED, HIGH);
  pinMode(Network_LED, OUTPUT); digitalWrite(Network_LED, HIGH);
pinMode(Test, OUTPUT); digitalWrite(Test, LOW);

  Mytone.Tone_Up(Speaker);

  // reset heartbeat timer
  LastMsg = millis();

} // end of setup


void loop() {

  // read A/D values and store in array Value[]
  digitalWrite(Run_LED, LOW);
  digitalWrite(12, HIGH);
  Value[ADS_Input] = My_PCB.power_sample(ADS_Input, Samples_Taken);
  digitalWrite(12, LOW);
  digitalWrite(Run_LED, HIGH);

  // inc ready for next A/D channel
  ADS_Input++;
  if (ADS_Input > 3) {
    ADS_Input = 0;
  } // end if

  // only used to make the LED flash visable
  delay(10);

  // headbeat timed out?
  if (millis() - LastMsg > Heatbeat) {

    LastMsg = millis();

    // print the values
    digitalWrite(Network_LED, HIGH);

    Mytone.Click_x(Speaker);

    String Report = String(Value[0]) + "mV, " + String(Value[1]) + "mV, " + String(Value[2]) + "mV, " + String(Value[3]) + "mV";
    Serial.println(Report);

    // only used to make the LED flash visable
    delay(10);

    digitalWrite(Network_LED, LOW);

  } // end of heartbeat

} // end of loop
