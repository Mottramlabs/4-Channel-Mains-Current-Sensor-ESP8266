/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201136
    Auther:   David Mottram
    Updated:  29th June 2021
*/
/***************************************************************************************************************************/

#ifndef MLP201136_Lib
#define MLP201136_Lib

// check of legacy IDE
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MLP201136
{
  public:

    MLP201136(int SelA, int SelB);
    uint16_t power_sample(int mux_channel, int samples);

  private:

    int _Select_A;
    int _Select_B;

    #define Samples_Taken 200                                     // sample to be taken per channel
    int8_t ADS_Input = 0;                                         // A/D channel select
    int16_t Value[4] = {0, 0, 0, 0};                              // array for results
    const float Calibration = 1;                                  // calibration value
    const float Volts_Conv = ((2.048 / 0.512) / Calibration);     // volts conversion value A/D 12-bit
    
    #define Mux_delay 500                                         // delay to allow mux to settle
    #define AD_delay 100                                         // delay to allow mux to settle

};
#endif
