/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201136
    Auther:   David Mottram
    Updated:  29th June 2021
*/
/***************************************************************************************************************************/

#include "Arduino.h"
#include "MLP201136.h"

// https://www.arduino.cc/en/Reference/Wire
#include <Wire.h>

// ------------------------------------------------------------------------------------------------------------------------
// constructor
// ------------------------------------------------------------------------------------------------------------------------
MLP201136::MLP201136(int SelA, int SelB) {

  // local variables
  _Select_A = SelA;
  _Select_B = SelB;

  pinMode(_Select_A, OUTPUT); digitalWrite(_Select_A, LOW);
  pinMode(_Select_B, OUTPUT); digitalWrite(_Select_B, LOW);

} // end of function


/*
  take several samples from a selected mux input. Then return the difference between the min and max samples
  providing a peak voltage sample.
*/
uint16_t MLP201136::power_sample(int mux_channel, int samples) {

  // select mux and continuous conversion
  switch (mux_channel) {
    case (0):
      digitalWrite(_Select_A, LOW);
      digitalWrite(_Select_B, LOW);
      break;
    case (1):
      digitalWrite(_Select_A, LOW);
      digitalWrite(_Select_B, HIGH);
      break;
    case (2):
      digitalWrite(_Select_A, HIGH);
      digitalWrite(_Select_B, LOW);
      break;
    case (3):
      digitalWrite(_Select_A, HIGH);
      digitalWrite(_Select_B, HIGH);
      break;
  } //end selection

  // delay to clear the mux
  delayMicroseconds(Mux_delay);

  uint16_t Max = 0x0000;
  uint16_t Min = 0xFFFF;
  uint16_t Sample = 0;

  // take many samples to find the max and min values
  for (int zz = 0; zz < samples; zz++) {

    Sample = analogRead(A0);

	// delay after A/D read
	delayMicroseconds(AD_delay);

    // find new min-max samples
    if (Sample > Max) {
      Max = Sample;
    } // end if
    if (Sample < Min) {
      Min = Sample;
    } // end if

  } // end sample loop

  // over range check, return 0xFFFF if over
  if (Max < 0x7FFF || Min < 0x0001) {
    Sample = Max - Min;

    // return value as mV RMS
    return ((Sample * Volts_Conv) * 0.3535);

    // return RAW A/D values
    //return Sample;

  } // end if
  else {
    Sample = 0xFFFF;
  } // end else

} // end of function
