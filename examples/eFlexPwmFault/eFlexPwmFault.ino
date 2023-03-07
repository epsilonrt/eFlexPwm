/*
  eFlexPwm Fault Example

  This is the same example as SIMPLE with fault detection.

  A low level on one of the selected FAULTx inputs triggers an immediate deactivation
  of the timers and an interruption.
  The outputs are reactivated if the user sends the character C on Serial.

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

  eFlexPwm Fault Example
  Submodules successfuly started
  FAULT1 triggered !
  All Faults cleared.
*/
#include <Arduino.h>
#include <eFlexPwm.h>
#include <util/atomic.h>
#ifdef DEBUG
#include <TeensyDebug.h>
#pragma GCC optimize ("O0")
#define WAIT_FOR_DEBUGGER_SECONDS 120
#define WAIT_FOR_SERIAL_SECONDS 20
#endif

//   Definitions
// ----------------------------------------------------------------------------
//#define WAIT_SERIAL_CONNECT
//#define DUMP_PWMCONFIG

// avoid systematically prefixing objects with the namespace
using namespace eFlex;

//   Variables
// ----------------------------------------------------------------------------
/* PWM frequence in hz. */
const uint32_t PwmFreq = 18000;
const uint32_t DeadTimeNs = 50;    // deadtime in nanoseconds
const uint8_t FaultPin[] = {0, 1, 2, 3};

// My eFlexPWM submodules (Hardware > PWM2: SM[0], SM[2], SM[3])
SubModule Sm20 (4, 33);
SubModule Sm22 (6, 9);
SubModule Sm23 (36, 37);
Timer &Tm2 = Sm20.timer();

// the duty cycle in %
uint8_t dutyCyclePercent = 0;

volatile int faultIrqSource = -1;

//   Code
// ----------------------------------------------------------------------------
void setupDebug();

void IsrPwm2Fault() {
  digitalWriteFast(LED_BUILTIN, HIGH);

  uint32_t statusFlags = Sm20.statusFlags();
  uint32_t mask = kPWM_Fault0Flag;

  for (uint8_t i = 0; i < 4; i++) {
    if (statusFlags & mask) {

      faultIrqSource = i;
      break;
    }
    mask <<= 1;
  }
  Sm20.clearStatusFlags (mask);
  digitalWriteFast(LED_BUILTIN, LOW);
}

// ----------------------------------------------------------------------------
/*
  TODO
  Ne fonctionne pas. Dès que l'on active la gestion des fautes avec Timer::setupFaults(), 
  les broches d'entrées FAULTx sont vues à l'état haut ce qui est confirmé par les bits 
  FFPIN dans le registre FSTS, pourtant les bits FFLAG restent à l'état bas. 
  Cela ne change pas même si on route les entrées FAULTx vers un niveau haut ou bas 
  avec XBAR. 
  Le seule chose qui désactive l'erreur au démarrage est la configuration setFaultLevel(true) 
  mais dans ce cas, il n'est pas possible de faire passer le timer en erreur.
 */
void setupFault() {
  FaultConfig MyFaultConfig;

  MyFaultConfig.setFaultLevel (false); // if false, failed !
  // Tm2.setupFaultState(kPWM_PwmFaultState1);
  Tm2.setupFaultInputFilter(MyFaultConfig);

  for (uint8_t i = 0; i < 4; i++) {

    if (Tm2.setupFaults (i, MyFaultConfig) != true) {
      Serial.printf ("Unable to setup FAULT%d\n", i);
      exit (EXIT_FAILURE);
    }
  }
#if 0
  /* Set PWM fault disable mapping for submodule 0/2/3 */
  Sm20.setupFaultDisableMap (ChanA, kPWM_faultchannel_0,
                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
  Sm22.setupFaultDisableMap (ChanA, kPWM_faultchannel_0,
                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
  Sm23.setupFaultDisableMap (ChanA, kPWM_faultchannel_0,
                             kPWM_FaultDisable_0 | kPWM_FaultDisable_1 | kPWM_FaultDisable_2 | kPWM_FaultDisable_3);
#endif
}

// ----------------------------------------------------------------------------
void setup() {

  // initialize LED digital pin as an output.
  pinMode (LED_BUILTIN, OUTPUT);

  digitalWrite (LED_BUILTIN, HIGH);
  setupDebug();

  for (uint8_t i = 0; i < 4; i++) {
    /*
      Configure all fault inputs and connect them to GPIO pins
    */
    if (Tm2.xbaraConnect (i, FaultPin[i], INPUT_PULLUP) != true) {
      Serial.printf ("Unable to connect FAULT%d\n", i);
      exit (EXIT_FAILURE);
    }
  }

  // start of PWM configuration -----------------------------------------------
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

  setupFault();

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

#if 1
  // Fault interrupts are common to all submodules
  Sm20.enableInterrupts (kPWM_Fault0InterruptEnable | kPWM_Fault1InterruptEnable | kPWM_Fault2InterruptEnable | kPWM_Fault3InterruptEnable);
  attachInterruptVector (IRQ_FLEXPWM2_FAULT, &IsrPwm2Fault);
  Sm20.clearStatusFlags (kPWM_Fault0Flag | kPWM_Fault1Flag | kPWM_Fault2Flag | kPWM_Fault3Flag);
  NVIC_ENABLE_IRQ (IRQ_FLEXPWM2_FAULT);
#endif

  #ifdef DUMP_PWMCONFIG
  Tm2.printAllRegs();
  #endif
  // end of PWM setup
  digitalWrite (LED_BUILTIN, LOW);
}

void loop() {

  int faultSource;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    faultSource = faultIrqSource;
  }

  if (faultSource >= 0) {
    Serial.printf ("FAULT%d triggered !\n", faultSource);
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      faultIrqSource = -1;
    }
  }

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

  // Delay at least 100 PWM periods
  delayMicroseconds ( (1000000U / PwmFreq) * 100);
}

void setupDebug() {
  // Debugger (gdb) support
  #ifdef DEBUG
  {
    bool debug_avail = false;
    bool serial_avail = false;

    SerialUSB1.begin (19200);

    for (uint8_t i = 0; i < WAIT_FOR_DEBUGGER_SECONDS; i++) {

      digitalWrite (LED_BUILTIN, HIGH);
      delay (800);
      digitalWrite (LED_BUILTIN, LOW);
      delay (200);

      // Debugger will use second USB Serial; this line is not need if using menu option
      if (SerialUSB1) {

        debug_avail = true;
        break;
      }

      if (Serial) {

        serial_avail = true;
        break;
      }
    }

    if (debug_avail) {

      debug.begin (SerialUSB1);
      delay (100);
      halt_cpu(); /* stop on startup; if not, Teensy keeps running and you
                 have to set a breakpoint or use Ctrl-C. */
      delay (100);
      debug.printf ("Debugger is connected. Connect USB serial monitor now ...\n");
    }

    // Serial wait block
    for (uint16_t i = 0; i < (WAIT_FOR_SERIAL_SECONDS * 10); i++) {

      if ( (Serial) || (serial_avail)) {

        Serial.println ("Serial ready.");
        break;
      }
      else {

        delay (100);
        digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
      }
    }
  }
  #else
  #ifdef WAIT_SERIAL_CONNECT
  while (!Serial)
    ; // wait for serial port to connect.
  Serial.println ("eFlexPwm Fault Example");
  #endif
  #endif
}