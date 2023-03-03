/*
   Copyright (c) 2015, Freescale Semiconductor, Inc.
   Copyright 2016-2022 NXP
   All rights reserved.

   SPDX-License-Identifier: BSD-3-Clause
*/
#pragma once

/*!
    @defgroup nxp NXP Driver
    NXP Driver types
    @{
*/

/*!
      @defgroup nxp_enums NXP Enumerations
      Public enumeration types
      @{
*/

/*! @brief List of PWM submodules */
typedef enum _pwm_submodule {
  kPWM_Module_0 = 0U, /*!< Submodule 0 */
  kPWM_Module_1,      /*!< Submodule 1 */
  kPWM_Module_2,      /*!< Submodule 2 */
  kPWM_Module_3       /*!< Submodule 3 */
} pwm_submodule_t;

/*! @brief List of PWM channels in each module */
typedef enum _pwm_channels {
  kPWM_PwmB = 0U,
  kPWM_PwmA,
  kPWM_PwmX
} pwm_channels_t;

/*! @brief List of PWM value registers */
typedef enum _pwm_value_register {
  kPWM_ValueRegister_0 = 0U, /*!< PWM Value0 register */
  kPWM_ValueRegister_1,      /*!< PWM Value1 register */
  kPWM_ValueRegister_2,      /*!< PWM Value2 register */
  kPWM_ValueRegister_3,      /*!< PWM Value3 register */
  kPWM_ValueRegister_4,      /*!< PWM Value4 register */
  kPWM_ValueRegister_5       /*!< PWM Value5 register */
} pwm_value_register_t;

/*! @brief List of PWM value registers mask */
enum _pwm_value_register_mask {
  kPWM_ValueRegisterMask_0 = (1U << 0), /*!< PWM Value0 register mask */
  kPWM_ValueRegisterMask_1 = (1U << 1), /*!< PWM Value1 register mask */
  kPWM_ValueRegisterMask_2 = (1U << 2), /*!< PWM Value2 register mask */
  kPWM_ValueRegisterMask_3 = (1U << 3), /*!< PWM Value3 register mask */
  kPWM_ValueRegisterMask_4 = (1U << 4), /*!< PWM Value4 register mask */
  kPWM_ValueRegisterMask_5 = (1U << 5)  /*!< PWM Value5 register mask */
};

/*! @brief PWM clock source selection.*/
typedef enum _pwm_clock_source {
  kPWM_BusClock = 0U,  /*!< The IPBus clock is used as the clock */
  kPWM_ExternalClock,  /*!< EXT_CLK is used as the clock */
  kPWM_Submodule0Clock /*!< Clock of the submodule 0 (AUX_CLK) is used as the source clock */
} pwm_clock_source_t;

/*! @brief PWM prescaler factor selection for clock source*/
typedef enum _pwm_clock_prescale {
  kPWM_Prescale_Divide_1 = 0U, /*!< PWM clock frequency = fclk/1 */
  kPWM_Prescale_Divide_2,      /*!< PWM clock frequency = fclk/2 */
  kPWM_Prescale_Divide_4,      /*!< PWM clock frequency = fclk/4 */
  kPWM_Prescale_Divide_8,      /*!< PWM clock frequency = fclk/8 */
  kPWM_Prescale_Divide_16,     /*!< PWM clock frequency = fclk/16 */
  kPWM_Prescale_Divide_32,     /*!< PWM clock frequency = fclk/32 */
  kPWM_Prescale_Divide_64,     /*!< PWM clock frequency = fclk/64 */
  kPWM_Prescale_Divide_128     /*!< PWM clock frequency = fclk/128 */
} pwm_clock_prescale_t;

/*! @brief Options that can trigger a PWM FORCE_OUT */
typedef enum _pwm_force_output_trigger {
  kPWM_Force_Local = 0U,   /*!< The local force signal, CTRL2[FORCE], from the submodule is used to force updates */
  kPWM_Force_Master,       /*!< The master force signal from submodule 0 is used to force updates */
  kPWM_Force_LocalReload,  /*!< The local reload signal from this submodule is used to force updates without regard to
                                the state of LDOK */
  kPWM_Force_MasterReload, /*!< The master reload signal from submodule 0 is used to force updates if LDOK is set */
  kPWM_Force_LocalSync,    /*!< The local sync signal from this submodule is used to force updates */
  kPWM_Force_MasterSync,   /*!< The master sync signal from submodule0 is used to force updates */
  kPWM_Force_External,     /*!< The external force signal, EXT_FORCE, from outside the PWM module causes updates */
  kPWM_Force_ExternalSync  /*!< The external sync signal, EXT_SYNC, from outside the PWM module causes updates */
} pwm_force_output_trigger_t;

/*! @brief PWM channel output status */
typedef enum _pwm_output_state {
  kPWM_HighState = 0, /*!< The output state of PWM channel is high */
  kPWM_LowState,      /*!< The output state of PWM channel is low */
  kPWM_NormalState,   /*!< The output state of PWM channel is normal */
  kPWM_InvertState,   /*!< The output state of PWM channel is invert */
  kPWM_MaskState      /*!< The output state of PWM channel is mask */
} pwm_output_state_t;

/*! @brief PWM counter initialization options */
typedef enum _pwm_init_source {
  kPWM_Initialize_LocalSync = 0U, /*!< Local sync causes initialization */
  kPWM_Initialize_MasterReload,   /*!< Master reload from submodule 0 causes initialization */
  kPWM_Initialize_MasterSync,     /*!< Master sync from submodule 0 causes initialization */
  kPWM_Initialize_ExtSync         /*!< EXT_SYNC causes initialization */
} pwm_init_source_t;

/*! @brief PWM load frequency selection */
typedef enum _pwm_load_frequency {
  kPWM_LoadEveryOportunity = 0U, /*!< Every PWM opportunity */
  kPWM_LoadEvery2Oportunity,     /*!< Every 2 PWM opportunities */
  kPWM_LoadEvery3Oportunity,     /*!< Every 3 PWM opportunities */
  kPWM_LoadEvery4Oportunity,     /*!< Every 4 PWM opportunities */
  kPWM_LoadEvery5Oportunity,     /*!< Every 5 PWM opportunities */
  kPWM_LoadEvery6Oportunity,     /*!< Every 6 PWM opportunities */
  kPWM_LoadEvery7Oportunity,     /*!< Every 7 PWM opportunities */
  kPWM_LoadEvery8Oportunity,     /*!< Every 8 PWM opportunities */
  kPWM_LoadEvery9Oportunity,     /*!< Every 9 PWM opportunities */
  kPWM_LoadEvery10Oportunity,    /*!< Every 10 PWM opportunities */
  kPWM_LoadEvery11Oportunity,    /*!< Every 11 PWM opportunities */
  kPWM_LoadEvery12Oportunity,    /*!< Every 12 PWM opportunities */
  kPWM_LoadEvery13Oportunity,    /*!< Every 13 PWM opportunities */
  kPWM_LoadEvery14Oportunity,    /*!< Every 14 PWM opportunities */
  kPWM_LoadEvery15Oportunity,    /*!< Every 15 PWM opportunities */
  kPWM_LoadEvery16Oportunity     /*!< Every 16 PWM opportunities */
} pwm_load_frequency_t;

/*! @brief List of PWM fault selections */
typedef enum _pwm_fault_input {
  kPWM_Fault_0 = 0U, /*!< Fault 0 input pin */
  kPWM_Fault_1,      /*!< Fault 1 input pin */
  kPWM_Fault_2,      /*!< Fault 2 input pin */
  kPWM_Fault_3       /*!< Fault 3 input pin */
} pwm_fault_input_t;

/*! @brief List of PWM fault disable mapping selections */
typedef enum _pwm_fault_disable {
  kPWM_FaultDisable_0 = (1U << 0), /*!< Fault 0 disable mapping */
  kPWM_FaultDisable_1 = (1U << 1), /*!< Fault 1 disable mapping */
  kPWM_FaultDisable_2 = (1U << 2), /*!< Fault 2 disable mapping */
  kPWM_FaultDisable_3 = (1U << 3)  /*!< Fault 3 disable mapping */
} pwm_fault_disable_t;

/*! @brief List of PWM fault channels */
typedef enum _pwm_fault_channels {
  kPWM_faultchannel_0 = 0U,
  kPWM_faultchannel_1
} pwm_fault_channels_t;

/*! @brief PWM capture edge select */
typedef enum _pwm_input_capture_edge {
  kPWM_Disable = 0U,   /*!< Disabled */
  kPWM_FallingEdge,    /*!< Capture on falling edge only */
  kPWM_RisingEdge,     /*!< Capture on rising edge only */
  kPWM_RiseAndFallEdge /*!< Capture on rising or falling edge */
} pwm_input_capture_edge_t;

/*! @brief PWM output options when a FORCE_OUT signal is asserted */
typedef enum _pwm_force_signal {
  kPWM_UsePwm = 0U,     /*!< Generated PWM signal is used by the deadtime logic.*/
  kPWM_InvertedPwm,     /*!< Inverted PWM signal is used by the deadtime logic.*/
  kPWM_SoftwareControl, /*!< Software controlled value is used by the deadtime logic. */
  kPWM_UseExternal      /*!< PWM_EXTA signal is used by the deadtime logic. */
} pwm_force_signal_t;

/*! @brief Options available for the PWM A & B pair operation */
typedef enum _pwm_chnl_pair_operation {
  kPWM_Independent = 0U,  /*!< PWM A & PWM B operate as 2 independent channels */
  kPWM_ComplementaryPwmA, /*!< PWM A & PWM B are complementary channels, PWM A generates the signal */
  kPWM_ComplementaryPwmB  /*!< PWM A & PWM B are complementary channels, PWM B generates the signal */
} pwm_chnl_pair_operation_t;

/*! @brief Options available on how to load the buffered-registers with new values */
typedef enum _pwm_register_reload {
  kPWM_ReloadImmediate = 0U,     /*!< Buffered-registers get loaded with new values as soon as LDOK bit is set */
  kPWM_ReloadPwmHalfCycle,       /*!< Registers loaded on a PWM half cycle */
  kPWM_ReloadPwmFullCycle,       /*!< Registers loaded on a PWM full cycle */
  kPWM_ReloadPwmHalfAndFullCycle /*!< Registers loaded on a PWM half & full cycle */
} pwm_register_reload_t;

/*! @brief Options available on how to re-enable the PWM output when recovering from a fault */
typedef enum _pwm_fault_recovery_mode {
  kPWM_NoRecovery = 0U,        /*!< PWM output will stay inactive */
  kPWM_RecoverHalfCycle,       /*!< PWM output re-enabled at the first half cycle */
  kPWM_RecoverFullCycle,       /*!< PWM output re-enabled at the first full cycle */
  kPWM_RecoverHalfAndFullCycle /*!< PWM output re-enabled at the first half or full cycle */
} pwm_fault_recovery_mode_t;

/*! @brief List of PWM interrupt options */
typedef enum _pwm_interrupt_enable {
  kPWM_CompareVal0InterruptEnable = (1U << 0),  /*!< PWM VAL0 compare interrupt */
  kPWM_CompareVal1InterruptEnable = (1U << 1),  /*!< PWM VAL1 compare interrupt */
  kPWM_CompareVal2InterruptEnable = (1U << 2),  /*!< PWM VAL2 compare interrupt */
  kPWM_CompareVal3InterruptEnable = (1U << 3),  /*!< PWM VAL3 compare interrupt */
  kPWM_CompareVal4InterruptEnable = (1U << 4),  /*!< PWM VAL4 compare interrupt */
  kPWM_CompareVal5InterruptEnable = (1U << 5),  /*!< PWM VAL5 compare interrupt */
  kPWM_CaptureX0InterruptEnable   = (1U << 6),  /*!< PWM capture X0 interrupt */
  kPWM_CaptureX1InterruptEnable   = (1U << 7),  /*!< PWM capture X1 interrupt */
  kPWM_CaptureB0InterruptEnable   = (1U << 8),  /*!< PWM capture B0 interrupt */
  kPWM_CaptureB1InterruptEnable   = (1U << 9),  /*!< PWM capture B1 interrupt */
  kPWM_CaptureA0InterruptEnable   = (1U << 10), /*!< PWM capture A0 interrupt */
  kPWM_CaptureA1InterruptEnable   = (1U << 11), /*!< PWM capture A1 interrupt */
  kPWM_ReloadInterruptEnable      = (1U << 12), /*!< PWM reload interrupt */
  kPWM_ReloadErrorInterruptEnable = (1U << 13), /*!< PWM reload error interrupt */
  kPWM_Fault0InterruptEnable      = (1U << 16), /*!< PWM fault 0 interrupt */
  kPWM_Fault1InterruptEnable      = (1U << 17), /*!< PWM fault 1 interrupt */
  kPWM_Fault2InterruptEnable      = (1U << 18), /*!< PWM fault 2 interrupt */
  kPWM_Fault3InterruptEnable      = (1U << 19)  /*!< PWM fault 3 interrupt */
} pwm_interrupt_enable_t;

/*! @brief List of PWM status flags */
typedef enum _pwm_status_flags {
  kPWM_CompareVal0Flag = (1U << 0),  /*!< PWM VAL0 compare flag */
  kPWM_CompareVal1Flag = (1U << 1),  /*!< PWM VAL1 compare flag */
  kPWM_CompareVal2Flag = (1U << 2),  /*!< PWM VAL2 compare flag */
  kPWM_CompareVal3Flag = (1U << 3),  /*!< PWM VAL3 compare flag */
  kPWM_CompareVal4Flag = (1U << 4),  /*!< PWM VAL4 compare flag */
  kPWM_CompareVal5Flag = (1U << 5),  /*!< PWM VAL5 compare flag */
  kPWM_CaptureX0Flag   = (1U << 6),  /*!< PWM capture X0 flag */
  kPWM_CaptureX1Flag   = (1U << 7),  /*!< PWM capture X1 flag */
  kPWM_CaptureB0Flag   = (1U << 8),  /*!< PWM capture B0 flag */
  kPWM_CaptureB1Flag   = (1U << 9),  /*!< PWM capture B1 flag */
  kPWM_CaptureA0Flag   = (1U << 10), /*!< PWM capture A0 flag */
  kPWM_CaptureA1Flag   = (1U << 11), /*!< PWM capture A1 flag */
  kPWM_ReloadFlag      = (1U << 12), /*!< PWM reload flag */
  kPWM_ReloadErrorFlag = (1U << 13), /*!< PWM reload error flag */
  kPWM_RegUpdatedFlag  = (1U << 14), /*!< PWM registers updated flag */
  kPWM_Fault0Flag      = (1U << 16), /*!< PWM fault 0 flag */
  kPWM_Fault1Flag      = (1U << 17), /*!< PWM fault 1 flag */
  kPWM_Fault2Flag      = (1U << 18), /*!< PWM fault 2 flag */
  kPWM_Fault3Flag      = (1U << 19)  /*!< PWM fault 3 flag */
} pwm_status_flags_t;

/*! @brief List of PWM DMA options */
typedef enum _pwm_dma_enable {
  kPWM_CaptureX0DMAEnable = (1U << 0), /*!< PWM capture X0 DMA */
  kPWM_CaptureX1DMAEnable = (1U << 1), /*!< PWM capture X1 DMA */
  kPWM_CaptureB0DMAEnable = (1U << 2), /*!< PWM capture B0 DMA */
  kPWM_CaptureB1DMAEnable = (1U << 3), /*!< PWM capture B1 DMA */
  kPWM_CaptureA0DMAEnable = (1U << 4), /*!< PWM capture A0 DMA */
  kPWM_CaptureA1DMAEnable = (1U << 5)  /*!< PWM capture A1 DMA */
} pwm_dma_enable_t;

/*! @brief List of PWM capture DMA enable source select */
typedef enum _pwm_dma_source_select {
  kPWM_DMARequestDisable = 0U, /*!< Read DMA requests disabled */
  kPWM_DMAWatermarksEnable,    /*!< Exceeding a FIFO watermark sets the DMA read request */
  kPWM_DMALocalSync,           /*!< A local sync (VAL1 matches counter) sets the read DMA request */
  kPWM_DMALocalReload          /*!< A local reload (STS[RF] being set) sets the read DMA request */
} pwm_dma_source_select_t;

/*! @brief PWM FIFO Watermark AND Control */
typedef enum _pwm_watermark_control {
  kPWM_FIFOWatermarksOR = 0U, /*!< Selected FIFO watermarks are OR'ed together */
  kPWM_FIFOWatermarksAND      /*!< Selected FIFO watermarks are AND'ed together */
} pwm_watermark_control_t;

/*! @brief PWM operation mode */
typedef enum _pwm_mode {
  kPWM_SignedCenterAligned = 0U, /*!< Signed center-aligned */
  kPWM_CenterAligned,            /*!< Unsigned cente-aligned */
  kPWM_SignedEdgeAligned,        /*!< Signed edge-aligned */
  kPWM_EdgeAligned               /*!< Unsigned edge-aligned */
} pwm_mode_t;

/*! @brief PWM output pulse mode, high-true or low-true */
typedef enum _pwm_level_select {
  kPWM_HighTrue = 0U, /*!< High level represents "on" or "active" state */
  kPWM_LowTrue        /*!< Low level represents "on" or "active" state */
} pwm_level_select_t;

/*! @brief PWM output fault status */
typedef enum _pwm_fault_state {
  kPWM_PwmFaultState0 =
    0U,              /*!< Output is forced to logic 0 state prior to consideration of output polarity control. */
  kPWM_PwmFaultState1, /*!< Output is forced to logic 1 state prior to consideration of output polarity control. */
  kPWM_PwmFaultState2, /*!< Output is tristated. */
  kPWM_PwmFaultState3  /*!< Output is tristated. */
} pwm_fault_state_t;

/*! @brief PWM reload source select */
typedef enum _pwm_reload_source_select {
  kPWM_LocalReload = 0U, /*!< The local reload signal is used to reload registers */
  kPWM_MasterReload      /*!< The master reload signal (from submodule 0) is used to reload */
} pwm_reload_source_select_t;

/*! @brief PWM fault clearing options */
typedef enum _pwm_fault_clear {
  kPWM_Automatic = 0U, /*!< Automatic fault clearing  */
  kPWM_ManualNormal,   /*!< Manual fault clearing with no fault safety mode */
  kPWM_ManualSafety    /*!< Manual fault clearing with fault safety mode */
} pwm_fault_clear_t;

/*! @brief Options for submodule master control operation */
typedef enum _pwm_module_control {
  kPWM_Control_Module_0 = (1U << 0), /*!< Control submodule 0's start/stop,buffer reload operation */
  kPWM_Control_Module_1 = (1U << 1), /*!< Control submodule 1's start/stop,buffer reload operation */
  kPWM_Control_Module_2 = (1U << 2), /*!< Control submodule 2's start/stop,buffer reload operation */
  kPWM_Control_Module_3 = (1U << 3)  /*!< Control submodule 3's start/stop,buffer reload operation */
} pwm_module_control_t;

/**   @} */ // end of nxp_enums

/*! @brief Structure for the user to define the PWM signal characteristics */
typedef struct _pwm_signal_param {
  pwm_channels_t pwmChannel; /*!< PWM channel being configured; PWM A or PWM B */
  uint8_t dutyCyclePercent;  /*!< PWM pulse width, value should be between 0 to 100
                                    0=inactive signal(0% duty cycle)...
                                    100=always active signal (100% duty cycle)*/
  pwm_level_select_t level;  /*!< PWM output active level select */
  uint16_t deadtimeValue;    /*!< The deadtime value; only used if channel pair is operating in complementary mode */
  pwm_fault_state_t faultState; /*!< PWM output fault status */
  bool pwmchannelenable;        /*!< Enable PWM output */
} pwm_signal_param_t;

/*!
   @brief PWM config structure

   This structure holds the configuration settings for the PWM peripheral. To initialize this
   structure to reasonable defaults, call the PWM_GetDefaultConfig() function and pass a
   pointer to your config structure instance.

   The config struct can be made const so it resides in flash
*/
typedef struct _pwm_config {
  bool debugModeEnabled; /*!< true: PWM continues to run in debug mode;
                               false: PWM is paused in debug mode */
  #if !defined(FSL_FEATURE_PWM_HAS_NO_WAITEN) || (!FSL_FEATURE_PWM_HAS_NO_WAITEN)
  bool enableWait;                         /*!< true: PWM continues to run in WAIT mode;
                                                  false: PWM is paused in WAIT mode */
  #endif                                       /* FSL_FEATURE_PWM_HAS_NO_WAITEN */
  pwm_init_source_t initializationControl; /*!< Option to initialize the counter */
  pwm_clock_source_t clockSource;          /*!< Clock source for the counter */
  pwm_clock_prescale_t prescale;           /*!< Pre-scaler to divide down the clock */
  pwm_chnl_pair_operation_t pairOperation; /*!< Channel pair in indepedent or complementary mode */
  pwm_register_reload_t reloadLogic;       /*!< PWM Reload logic setup */
  pwm_reload_source_select_t reloadSelect; /*!< Reload source select */
  pwm_load_frequency_t reloadFrequency;    /*!< Specifies when to reload, used when user's choice
                                                  is not immediate reload */
  pwm_force_output_trigger_t forceTrigger; /*!< Specify which signal will trigger a FORCE_OUT */
} pwm_config_t;

/*! @brief Structure for the user to configure the fault input filter. */
typedef struct _pwm_fault_input_filter_param {
  uint8_t faultFilterCount;  /*!< Fault filter count */
  uint8_t faultFilterPeriod; /*!< Fault filter period;value of 0 will bypass the filter */
  bool faultGlitchStretch;   /*!< Fault Glitch Stretch Enable: A logic 1 means that input
                                    fault signals will be stretched to at least 2 IPBus clock cycles */
} pwm_fault_input_filter_param_t;

/*! @brief Structure is used to hold the parameters to configure a PWM fault */
typedef struct _pwm_fault_param {
  pwm_fault_clear_t faultClearingMode;   /*!< Fault clearing mode to use */
  bool faultLevel;                       /*!< true: Logic 1 indicates fault;
                                                false: Logic 0 indicates fault */
  bool enableCombinationalPath;          /*!< true: Combinational Path from fault input is enabled;
                                                false: No combination path is available */
  pwm_fault_recovery_mode_t recoverMode; /*!< Specify when to re-enable the PWM output */
} pwm_fault_param_t;

/*!
   @brief Structure is used to hold parameters to configure the capture capability of a signal pin
*/
typedef struct _pwm_input_capture_param {
  bool captureInputSel;           /*!< true: Use the edge counter signal as source
                                         false: Use the raw input signal from the pin as source */
  uint8_t edgeCompareValue;       /*!< Compare value, used only if edge counter is used as source */
  pwm_input_capture_edge_t edge0; /*!< Specify which edge causes a capture for input circuitry 0 */
  pwm_input_capture_edge_t edge1; /*!< Specify which edge causes a capture for input circuitry 1 */
  bool enableOneShotCapture;      /*!< true: Use one-shot capture mode;
                                         false: Use free-running capture mode */
  uint8_t fifoWatermark;          /*!< Watermark level for capture FIFO. The capture flags in
                                         the status register will set if the word count in the FIFO
                                         is greater than this watermark level */
} pwm_input_capture_param_t;

/** @} */ // end of nxp
