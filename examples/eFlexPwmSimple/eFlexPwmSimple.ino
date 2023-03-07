/*
  eFlexPwm Simple Example

  This example generates 3 pairs of PWM signals on a Teensy 4.1 board
  Each signal pair corresponds to the PWMA output and the PWMB (A's complement) 
  output of an eFlexPWM submodule.
  In this example, sub-modules 0, 2 and 3 of PWM2 are used. On the Teensy 4.1 board, 
  this corresponds to the following pins:
  | PWM | SubModule | Pol | Teensy | Native |
  |-----|-----------|-----|--------|--------|
  | 2   | 0         | A   | 4      | EMC_06 |
  | 2   | 0         | B   | 33     | EMC_07 |
  | 2   | 2         | A   | 6      | B0_10  |
  | 2   | 2         | B   | 9      | B0_11  |
  | 2   | 3         | A   | 36     | B1_02  |
  | 2   | 3         | B   | 37     | B1_03  |
  The frequency is fixed by the constant PwmFreq, here at 18 kHz
  On sub-module 0, the duty cycle changes by 5 in 5 from 0 to 100% every 100 PWM periods.
  The duty cycle of sub-module 2 is equal to that of sub-module 0 divided by 2.
  The duty cycle of sub-module 3 is equal to that of sub-module 0 divided by 4.

  This example displays a message on the Serial link (USB CDC), and any error messages: 

  eFlexPwm Simple Example
  Submodules successfuly started
*/
#include <Arduino.h>
#include <eFlexPwm.h>

//   Definitions
// ----------------------------------------------------------------------------
// avoid systematically prefixing objects with the namespace
using namespace eFlex;

//   Variables
// ----------------------------------------------------------------------------
/* PWM frequence in hz. */
const uint32_t PwmFreq = 18000;
const float32_t DeadTimeNs = 50.0;    // deadtime in nanoseconds

// My eFlexPWM submodules (Hardware > PWM2: SM[0], SM[2], SM[3])
SubModule Sm20 (4, 33);
SubModule Sm22 (6, 9);
SubModule Sm23 (36, 37);
/* 
  all the sub-modules are part of the same timer: PWM2, 
  Tm2 simplifies access to the functions that concern all the sub-modules
*/
Timer &Tm2 = Sm20.timer();

// the duty cycle in %
uint8_t dutyCyclePercent = 0;

//   Code
// ----------------------------------------------------------------------------
void setup() {

  // initialize LED digital pin as an output.
  pinMode (LED_BUILTIN, OUTPUT);

  digitalWrite (LED_BUILTIN, HIGH);
  Serial.println ("eFlexPwm Simple Example");
  // start of PWM configuration -----------------------------------------------
  /* Submodule configuration, default values are:

    config->debugModeEnabled = false;
    config->enableWait = false;
    config->reloadSelect = kPWM_LocalReload;
    config->clockSource = kPWM_BusClock;
    config->prescale = kPWM_Prescale_Divide_1;
    config->initializationControl = kPWM_Initialize_LocalSync;
    config->forceTrigger = kPWM_Force_Local;
    config->reloadFrequency = kPWM_LoadEveryOportunity;
    config->reloadLogic = kPWM_ReloadImmediate;
    config->pairOperation = kPWM_Independent;
    config->pwmFreqHz = 5000;
    config->mode = kPWM_CenterAligned;
  */
  Config myConfig;

  /* Use full cycle reload */
  myConfig.setReloadLogic (kPWM_ReloadPwmFullCycle);
  /* PWM A & PWM B form a complementary PWM pair */
  myConfig.setPairOperation (kPWM_ComplementaryPwmA);
  myConfig.setPwmFreqHz (PwmFreq);

  /* Initialize submodule 0 */
  if (Sm20.configure (myConfig) != true) {
    Serial.println ("Submodule 0 initialization failed");
    exit (EXIT_FAILURE);
  }

  /* Initialize submodule 2, make it use same counter clock as submodule 0. */
  myConfig.setClockSource (kPWM_Submodule0Clock);
  myConfig.setPrescale (kPWM_Prescale_Divide_1);
  myConfig.setInitializationControl (kPWM_Initialize_MasterSync);

  if (Sm22.configure (myConfig) != true) {
    Serial.println ("Submodule 2 initialization failed");
    exit (EXIT_FAILURE);
  }

  /* Initialize submodule 3 the same way as submodule 2 */
  if (Sm23.configure (myConfig) != true) {
    Serial.println ("Submodule 3 initialization failed");
    exit (EXIT_FAILURE);
  }

  /* Signal and pin configuration, parameters that can be changed :
      dutyCyclePercent :  Initial PWM pulse width, value should be between 0 to 100
                                    0=inactive signal(0% duty cycle)...
                                    100=always active signal (100% duty cycle)
                          default value: 50%
      level            :  PWM output active level select, default value: kPWM_HighTrue
      deadtimeValue    :  The deadtime value; only used if channel pair is
                                    operating in complementary mode
                          default value: 0
      faultState       :  PWM output fault status, default value: kPWM_PwmFaultState0
      pwmchannelenable :  Enable PWM output, default value: true
  */

  /*
    Set deadtime count, we set this to about 1000ns for all submodules pins
    You can also, modify it for all the pins of a sub-module, eg. :

      Sm20.setupDeadtime (deadTimeVal);

    or even for a particular pin, eg. :

      Sm20.setupDeadtime (deadTimeVal, ChanA);
  */
  uint16_t deadTimeVal = ( (uint64_t) Tm2.srcClockHz() * DeadTimeNs) / 1000000000;
  Tm2.setupDeadtime (deadTimeVal);

  // synchronize registers and start all submodules
  if (Tm2.begin() != true) {
    Serial.println ("Failed to start submodules");
    exit (EXIT_FAILURE);
  }
  else {

    Serial.println ("Submodules successfuly started");
  }
  // end of PWM setup
  digitalWrite (LED_BUILTIN, LOW);
}

void loop() {
  // turn the LED on (HIGH is the voltage level)
  digitalWrite (LED_BUILTIN, HIGH);

  dutyCyclePercent += 5;

  /* Update duty cycles for all 3 PWM signals */
  Sm20.updateDutyCyclePercent (dutyCyclePercent, ChanA);
  Sm22.updateDutyCyclePercent (dutyCyclePercent >> 1, ChanA);
  Sm23.updateDutyCyclePercent (dutyCyclePercent >> 2, ChanA);

  /* Set the load okay bit for all submodules to load registers from their buffer */
  Tm2.setPwmLdok();

  if (dutyCyclePercent >= 100) {
    
    dutyCyclePercent = 5;
  }

  // turn the LED off by making the voltage LOW
  digitalWrite (LED_BUILTIN, LOW);
  // Delay at least 100 PWM periods
  delayMicroseconds ( (1000000U / PwmFreq) * 100);
}
